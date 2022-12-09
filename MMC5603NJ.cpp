
#include "MMC5603NJ.h"
#include "mbed.h"

MMC5603NJ::MMC5603NJ(PinName sda, PinName scl): m_i2c(sda, scl), m_addr(MMC_ADDR)
{
    char cmd[2];
    cmd[0] = MMC_INCTRL0;
    cmd[1] = 0x21;
    m_i2c.write(m_addr, cmd, 2);

}

void MMC5603NJ::enable(void) {
    uint8_t data[2];
    readRegs( MAG_CTRL_REG1, &data[1], 1);
    data[1] |= 0xFF;
    data[0] = MMC_ODR;
    writeRegs(data, 2);
}


void MMC5603NJ::disable(void) {
    uint8_t data[2];
    readRegs( MAG_CTRL_REG1, &data[1], 1);
    data[1] &= 0xFE;
    data[0] = MAG_CTRL_REG1;
    writeRegs(data, 2);
}


void MMC5603NJ::readRegs(int addr, uint8_t * data, int len)
{
    char cmd[1];
    cmd[0] = addr;
    m_i2c.write( m_addr, cmd, 1, true);
    m_i2c.read( m_addr, (char *) data, len);
    return;
}


void MMC5603NJ::writeRegs(uint8_t * data, int len) {
    m_i2c.write(m_addr, (char *)data, len);
}


uint32_t MMC5603NJ::PID() {
    uint8_t pid = 0;
    readRegs(MMC_PID, &pid, 1);
    return (uint32_t) pid;
}

uint32_t MMC5603NJ::dataReady(void) {
    uint8_t stat = 0;
    readRegs(MAG_DR_STATUS, &stat, 1);
    return (uint32_t) stat;
}

void MMC5603NJ::getX(int32_t* d) {
    *d = getMagAxis(MMC_XOUT0);
    this.x = &d;
}

void MMC5603NJ::getY(int32_t* d) {
    *d = getMagAxis(MMC_Y0UT0);
    this.y = &d;
}

void MMC5603NJ::getZ(int32_t* d) {
    *d = getMagAxis(MMC_ZOUT0);
    this.z = &d;
}

int32_t MMC5603NJ::getMagAxis(uint8_t addr) {
    int32_t acc;
    uint8_t res[7];
    readRegs(addr, res, 7);

    acc = (res[0] << 12) | (res[1] << 4) | (res[6] >> 4);

    return acc;
}

float MMC5603NJ::getTemp() {
    uint8_t res[1];
    readRegs(MMC_TOUT, res, 1);
    return ((float)res[0] * 0.8) - 75f;
}

void MMC5603NJ::getAxis() {
    int32_t t[3];
    uint8_t res[9];

    readRegs(MMC_XOUT0, res, 9);
    t[0] = (res[0] << 12) | (res[1] << 4) | (res[6] >> 4);
    t[1] = (res[2] << 12) | (res[3] << 4) | (res[7] >> 4);
    t[2] = (res[4] << 12) | (res[5] << 4) | (res[8] >> 4);
    this.x = ((float) t[0])/((float) 16384);
    this.y = ((float) t[1])/((float) 16384);
    this.z = ((float) t[2])/((float) 16384);
}
/*

// read a register per, pass first reg value, reading 2 bytes increments register
// Reads MSB first then LSB
int MAG3110::readVal(char regAddr)
{
    char cmd[2];
    int16_t t;
    cmd[0] = regAddr;
    if(_i2c.write(m_addr, cmd, 1)) {
        printf("MAG3110 write error\r\n");
        _i2c.stop();
        _i2c.start();
        }

    cmd[0] = 0x00;
    cmd[1] = 0x00;
    _i2c.read(m_addr, cmd, 2);
    t = (cmd[0] * 256) + (unsigned short) cmd[1];
    return ((int) t); //concatenate the MSB and LSB
}


float MAG3110::getHeading()
{
    int xVal = readVal(MAG_OUT_X_MSB);
    int yVal = readVal(MAG_OUT_Y_MSB);
    return (atan2((double)(yVal - _avgY),(double)(xVal - _avgX)))*180/PI;
}

void MAG3110::getValues(int *xVal, int *yVal, int *zVal)
{
    *xVal = readVal(MAG_OUT_X_MSB);
    *yVal = readVal(MAG_OUT_Y_MSB);
    *zVal = readVal(MAG_OUT_Z_MSB);
}

void MAG3110::ReadXYZ(float * mag)
{
    int x, y, z;
    x = readVal(MAG_OUT_X_MSB);
    y = readVal(MAG_OUT_Y_MSB);
    z = readVal(MAG_OUT_Z_MSB);
    mag[0] = (float) x / 10.0;
    mag[1] = (float) y / 10.0;
    mag[2] = (float) z / 10.0;
    
}

void MAG3110::ReadXYZraw(int16_t * mag_raw)
{
    mag_raw[0] = readVal(MAG_OUT_X_MSB);
    mag_raw[1] = readVal(MAG_OUT_Y_MSB);
    mag_raw[2] = readVal(MAG_OUT_Z_MSB);
}

void MAG3110::setCalibration(int minX, int maxX, int minY, int maxY )
{
    _avgX=(maxX+minX)/2;
    _avgY=(maxY+minY)/2;
}
*/




