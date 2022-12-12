/* 
 * Header file for MMC5603NJ Breakout board
 * For more information view the datasheet at
 * https://media.digikey.com/pdf/Data%20Sheets/MEMSIC%20PDFs/MMC5603NJ_RevB_7-12-18.pdf
 */

#include "mbed.h"
#ifndef _MMC5603NJ
#define _MMC5603NJ

// define registers
#define MMC_XOUT0 0x00
#define MMC_XOUT1 0x01
#define MMC_YOUT0 0x02
#define MMC_YOUT1 0x03
#define MMC_ZOUT0 0x04
#define MMC_ZOUT1 0x05
#define MMC_XOUT2 0x06
#define MMC_YOUT2 0x07
#define MMC_ZOUT2 0x08
#define MMC_TOUT 0x09
#define MMC_STATUS1 0x18
#define MMC_ODR 0x1A
#define MMC_INCTRL0 0x1B
#define MMC_INCTRL1 0x1C
#define MMC_INCTRL2 0x1D
#define MMC_ST_X_TH 0x1E
#define MMC_ST_Y_TH 0x1F
#define MMC_ST_Z_TH 0x20
#define MMC_ST_X 0x27
#define MMC_ST_Y 0x28
#define MMC_ST_Z 0x29
#define MMC_PID 0x39
#define MMC_ADDR 0x60
//No need to define MMC_READ or MMC_WRITE because mbed's I2C API
//Automatically does bitwise operations on the address to set it
//As a read/write operation.

/**
 * MMC5603NJ Class to read data from the device
 *
 */
class MMC5603NJ
{
public:
    MMC5603NJ(PinName sda, PinName scl);
    void setBitRate(unsigned char rate);
    void enable();
    void disable(void);
    uint32_t PID(void);
    char getTemp(void);
    void getX();
    void getY();
    void getZ();
    void getAxis();
    void readRegs(int addr, char * data, int len);
    float x, y, z;
private:
    int m_addr;
    bool continuous;
    I2C m_i2c;
    int32_t getMagAxis(char addr);
    void writeRegs(char * data, int len);

};
#endif
