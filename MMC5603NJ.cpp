
#include "MMC5603NJ.h"
#include "mbed.h"

// This constructor class sets the "DO RESET"
// bit in INCTRL0 high in order for the chip to
// reset its magnetometer wafer. It takes
// 375 nanoseconds to occur so some padding is given
// afterwards for it to operate. The bit is automatically
// cleared by the device after the reset is completed.
MMC5603NJ::MMC5603NJ(PinName sda, PinName scl): m_i2c(sda, scl), m_addr(MMC_ADDR)
{
    char cmd[2];
    cmd[0] = MMC_INCTRL0;
    cmd[1] = 0x08;
    m_i2c.write(m_addr, cmd, 2);
    wait(0.1);
}
void MMC5603NJ::setBitRate(unsigned char rate) {
    char data[2];
    data[1] = rate;
    data[0] = MMC_ODR;
    m_i2c.write(m_addr, data, 2);
}
// Sends a signal to the MMC's INCTRL0 register
// to get its current value and performs a bitwise
// OR operation with 0b10000000, and then gets INCTRL2 and performs a
// bitwise OR with 0b00010000. Setting both of these register bits to 1 enables
// continuous measuremnt mode on the MMC chip.
void MMC5603NJ::enable() {
    continuous = true;
    char en[2];
    readRegs( MMC_INCTRL0, &en[1], 1);
    en[1] |= 0x80;
    en[0] = MMC_INCTRL0;
    m_i2c.write(m_addr, en, 2);
    readRegs(MMC_INCTRL2, &en[1], 1);
    en[1] |= 0x10;
    en[0] = MMC_INCTRL2;
    m_i2c.write(m_addr, en, 2);
}

// Sends a signal to the MMC's INCTRL0 register
// by getting its current values and then performing
// A bitwise and with 0b01111111, and similarly with
// INCTRL2 and 0b11101111 in order to set the continuous
// read bit low to disable continuous measurement mode.
void MMC5603NJ::disable(void) {
    continuous = false;
    char en[2];
    readRegs(MMC_INCTRL2, &en[1], 1);
    en[1] &= 0xEF;
    en[0] = MMC_INCTRL2;
    m_i2c.write(m_addr, en, 2);
}

// Returns the product ID.
// @returns the product ID, which should always be 0x10 in hex or 16 in decimal.
uint32_t MMC5603NJ::PID() {
    char pid = 0;
    readRegs(MMC_PID, &pid, 1);
    return (uint32_t) pid;
}

// Gets the current operating temperature of the MMC chip.
// The given formula is -75 + (0.8 * TOUT) degrees celsius.
// @returns the temperature of the chip in celsius
char MMC5603NJ::getTemp() {
    char en[2];
    readRegs( MMC_INCTRL0, &en[1], 1);
    // Forcing this register to exactly 0b00000010 is terrible coding practice
    // and it should be a bitwise OR with 0x10, but that doesn't work.
    // Doing as such causes very strange undefined behavior to occur.
    en[1] = 0x02;
    en[0] = MMC_INCTRL0;
    m_i2c.write(m_addr, en, 2);
    char status = 0;
    while ((status >> 7 == 0)) {
        readRegs(MMC_STATUS1, &status, 1);
        wait(0.2);
        m_i2c.write(m_addr, en, 2);
    }
    char res[1];
    readRegs(MMC_TOUT, res, 1);
    unsigned char ures = (unsigned char)res[0];
    float out = (float)ures;
    out = (out * 0.8) - 75;
    return out;
}

// Gets the most recently sampled x value.
float MMC5603NJ::getX() {
    int32_t xVal = getMagAxis(MMC_XOUT0) - 524288;
    x = ((float)xVal/(float)16384);
    return x;
}
// Gets the most recently sampled y value.
float MMC5603NJ::getY() {
    int yVal = getMagAxis(MMC_YOUT0) - 524288;
    y = ((float)yVal/(float)16384);
    return y;
}
// Gets the most recently sampled z value.
float MMC5603NJ::getZ() {
    int zVal = getMagAxis(MMC_ZOUT0) - 524288;
    z = ((float)zVal/(float)16384);
    return z;
}

// Samples all three coordinate axes at once and stores in the x/y/z floats.
void MMC5603NJ::getAxis() {
    int32_t t[3];
    char res[9];
    if(continuous) {
        
        readRegs(MMC_XOUT0, res, 9);
        t[0] = (res[0] << 12) | (res[1] << 4) | (res[6] >> 4);
        t[1] = (res[2] << 12) | (res[3] << 4) | (res[7] >> 4);
        t[2] = (res[4] << 12) | (res[5] << 4) | (res[8] >> 4);
        t[0] -= 524288;
        t[1] -= 524288;
        t[2] -= 524288;
        x = ((float) t[0])/((float) 16384);
        y = ((float) t[1])/((float) 16384);
        z = ((float) t[2])/((float) 16384);
    } else {
        t[0] = getMagAxis(MMC_XOUT0) - 524288;
        t[1] = getMagAxis(MMC_YOUT0) - 524288;
        t[2] = getMagAxis(MMC_ZOUT0) - 524288;
        x = ((float) t[0])/((float) 16384);
        y = ((float) t[1])/((float) 16384);
        z = ((float) t[2])/((float) 16384);
    }
}

// Wrapper function for the i2c superclass's read function.
// @param addr The address being read
// @param data Pointer to a char value
void MMC5603NJ::readRegs(int addr, char * data, int len)
{
    char cmd[1];
    cmd[0] = addr;
    m_i2c.write( m_addr, cmd, 1, true);
    m_i2c.read( m_addr, (char *) data, len);
    return;
}

// Gets a specific axis value based on the coordinate
// of the first OUT0 bit.
// Technically there are 3 possible output modes:
// 16 bit, 18 bit, and 20 bit.
// However, given a 3.3V power supply from an mbed
// this should always output in 20 bit mode.
// The only real difference between the modes
// is that the 16 and 18 bit modes have less decimal
// accuracy compared to the 20 bit mode.
// @param addr the specified address being sampled
// @returns the 20-bit integer value for X, Y, or Z.
int32_t MMC5603NJ::getMagAxis(char addr) {
    char en[2];
    readRegs( MMC_INCTRL0, &en[1], 1);
    en[1] = 0x01;
    en[0] = MMC_INCTRL0;
    m_i2c.write(m_addr, en, 2);
    char status = 0;
    while ((status & 0x40) == 0) {
        readRegs(MMC_STATUS1, &status, 1);
    }
    int32_t acc;
    char res[7];
    readRegs(addr, res, 7);
    acc = (res[0] << 12) | (res[1] << 4) | (res[6] >> 4);
    return acc;
}

// Wrapper function for the i2c superclass's write function.
// @param data Pointer to an char array of X length, where data[0] is the
// register that is being written to.
// @param len How many bytes are being written, excluding the specified register
void MMC5603NJ::writeRegs(char * data, int len) {
    m_i2c.write(m_addr, (char *)data, len);
}