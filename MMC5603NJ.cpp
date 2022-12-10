
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
    cmd[1] = 0x10;
    m_i2c.write(m_addr, cmd, 2);
    wait(0.1);

}
// Sets the ODR, or the measurement data rate register
// to 0b00000001 in order to begin performing continuous
// measurements roughly 75 times a second. Then it
// sends a signal to the MMC's INCTRL0 register
// to get its current value and performs a bitwise
// OR operation with 0b10000000 in order to activate the
// continuous measuremnt mode on the MMC chip.
void MMC5603NJ::enable(void) {
    char data[2];
    data[1] = 0x01;
    data[0] = MMC_ODR;
    m_i2c.write(0x60, data, 2);
    char en[2];
    readRegs( MMC_INCTRL0, &en[1], 1);
    en[1] |= 0x80;
    en[0] = MMC_INCTRL0;
    m_i2c.write(m_addr, en, 2);
}

// Sends a signal to the MMC's INCTRL0 register
// by getting its current values and then performing
// A bitwise and with 0b01111111 to set the continuous
// read bit low to disable continuous measurement mode.
void MMC5603NJ::disable(void) {
    char data[2];
    data[1] = 0x00;
    data[0] = MMC_ODR;
    m_i2c.write(m_addr, data, 2);
    char en[2];
    readRegs( MMC_INCTRL0, &en[1], 1);
    en[1] &= 0x7F;
    en[0] = MMC_INCTRL0;
    m_i2c.write(m_addr, en, 2);
}

// Wrapper function for the i2c superclass's read function.
void MMC5603NJ::readRegs(int addr, char * data, int len)
{
    char cmd[1];
    cmd[0] = addr;
    m_i2c.write( m_addr, cmd, 1, true);
    m_i2c.read( m_addr, (char *) data, len);
    return;
}

// Wrapper function for the i2c superclass's write function.
void MMC5603NJ::writeRegs(char * data, int len) {
    m_i2c.write(m_addr, (char *)data, len);
}

// Returns the product ID. Should always be the value 16.
uint32_t MMC5603NJ::PID() {
    char pid = 0;
    readRegs(MMC_PID, &pid, 1);
    return (uint32_t) pid;
}
// Gets the most recently sampled x value.
void MMC5603NJ::getX() {
    int32_t xVal = getMagAxis(MMC_XOUT0);
    if (xVal & 524288) {
        xVal |= 0x80000000;
        xVal &= 0xFFF7FFFF;
    }
    x = (float)xVal/(float)16384;
}
// Gets the most recently sampled y value.
void MMC5603NJ::getY() {
    int yVal = getMagAxis(MMC_Y0UT0);
    if (yVal & 524288) {
        yVal |= 0x80000000;
        yVal &= 0xFFF7FFFF;
    }
    y = (float)yVal/(float)16384;
}
// Gets the most recently sampled z value.
void MMC5603NJ::getZ() {
    int zVal = getMagAxis(MMC_ZOUT0);
    if (zVal & 524288) {
        zVal |= 0x80000000;
        zVal &= 0xFFF7FFFF;
    }
    z = (float)zVal/(float)16384;
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
int32_t MMC5603NJ::getMagAxis(char addr) {
    int32_t acc;
    char res[7];
    readRegs(addr, res, 7);

    acc = (res[0] << 12) | (res[1] << 4) | (res[6] >> 4);

    return acc;
}
// Gets the current operating temperature of the MMC chip.
// The given formula is -75 + (0.8 * TOUT) degrees celsius.
float MMC5603NJ::getTemp() {
    char en[2];
    readRegs( MMC_INCTRL0, &en[1], 1);
    en[1] |= 0x02;
    en[0] = MMC_INCTRL0;
    m_i2c.write(m_addr, en, 2);
    wait(0.1);
    char res[1];
    readRegs(MMC_TOUT, res, 1);
    unsigned char out = res[0];
    return ((float)out * 0.8) - 75;
}
// Samples all three coordinate axes at once.
void MMC5603NJ::getAxis() {
    int32_t t[3];
    char res[9];

    readRegs(MMC_XOUT0, res, 9);
    t[0] = (res[0] << 12) | (res[1] << 4) | (res[6] >> 4);
    t[1] = (res[2] << 12) | (res[3] << 4) | (res[7] >> 4);
    t[2] = (res[4] << 12) | (res[5] << 4) | (res[8] >> 4);
    x = ((float) t[0])/((float) 16384);
    y = ((float) t[1])/((float) 16384);
    z = ((float) t[2])/((float) 16384);
}