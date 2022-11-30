/* 
 * Header file for MMC5603NJ Breakout board
 * For more information view the datasheet at
 * https://media.digikey.com/pdf/Data%20Sheets/MEMSIC%20PDFs/MMC5603NJ_RevB_7-12-18.pdf
 */

#ifndef MMC5603NJ
#define MMC5603NJ

#include "mbed.h"
#include "MotionSensor.h"

// define registers
#define MMC_XOUT0 0x00
#define MMC_XOUT1 0x01
#define MMC_Y0UT0 0x02
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

/**
 * MMC5603NJ Class to read data from the device
 *
 */
class MMC5603NJ : public MotionSensor
{
public:
    /**
     * Main constructor
     * @param sda SDA pin
     * @param sdl SCL pin
     */
    MMC5603NJ(PinName sda, PinName scl);
    void enable(void);
    void disable(void);
    uint32_t sampleRate(uint32_t fequency);
    uint32_t whoAmI(void);
    uint32_t dataReady(void);
    void getX(int16_t * x);
    void getY(int16_t * y);
    void getZ(int16_t * z);
    void getX(float * x);
    void getY(float * y);
    void getZ(float * z);
    void getAxis(MotionSensorDataCounts &data);
    void getAxis(MotionSensorDataUnits &data);
    void readRegs(int addr, uint8_t * data, int len);
  
private:
  I2C m_i2c;
  char m_addr;
  int16_t getMagAxis(uint8_t addr);
  void writeRegs(uint8_t * data, int len);

};
#endif
