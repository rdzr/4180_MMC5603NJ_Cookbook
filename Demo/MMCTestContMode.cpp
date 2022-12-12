#include "mbed.h"
#include "MMC5603NJ.h"
MMC5603NJ mmc(p9, p10);
//Samples using the continuous mode once, then disables continuous mode.
// N.B.:Using bitrate values in the range of 200+ causes issues with reading
// on the mbed LPC 1768.
int main() {
    unsigned char rate = 50;
    mmc.setBitRate(rate);
    mmc.enable();
    mmc.getAxis();
    printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
    mmc.disable();
    return;
}
