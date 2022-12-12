#include "mbed.h"
#include "MMC5603NJ.h"
MMC5603NJ mmc(p9, p10);
// Sets the bitrate for continuous measurement mode.
// N.B.:Using bitrate values in the range of 200+ causes issues with reading
// on the mbed LPC 1768.
void setRate(unsigned char bitrate) {
    mmc.setBitRate(bitrate);
}
//Samples using the continuous mode once, then disables continuous mode.
void testContMode() {
    mmc.enable();
    mmc.getAxis();
    printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
    mmc.disable();
    return;
}
//Samples using the continuous measurement mode indefinitely.
void contSampleForever() {
    mmc.enable();
    while(1) {
        mmc.getAxis();
        printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
    }
}
//Samples the XYZ axes twice, once with getAxis and again with getX, getY, and
//getZ functions, and prints the values to console.
void testSingleSampleModes() {
    mmc.getAxis();
    printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
    printf("X: %f Y: %f Z: %f\n\r", mmc.getX(), mmc.getY(), mmc.getZ());
    return;
}
//Samples XYZ axes via both single-input methods forever and prints to console.
void singleSampleForever() {
    while(1) {
        mmc.getAxis();
        printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
        mmc.getX();
        mmc.getY();
        mmc.getZ();
        printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
    }
}
//Test to see the current temperature read in celsius and prints to console.
void testTemperature() {
    int temp = mmc.getTemp();
    printf("The current temp is %d C\n\r", temp);
    return;
}
//Sends the product ID to console. Should always be 16.
//Useful for confirming that you are communicating with the device.
void testPID() {
    int id = mmc.PID();
    printf("The returned PID is %d\n\r", id);
    return;
}
int main() {
    //setRate(1);
    while(1) {
        //testContMode();
        //wait(0.2);
        testSingleSampleModes();
        testTemperature();
        testPID();
        //singleSampleForever();
        //contSampleForever();
    }
}