#include "mbed.h"
#include "MMC5603NJ.h"
MMC5603NJ mmc(p9, p10);
//Samples the XYZ axes twice, once with getAxis and again with getX, getY, and
//getZ functions, and prints the values to console.
int main() {
    mmc.getAxis();
    printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
    mmc.getX();
    mmc.getY();
    mmc.getZ();
    printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
    return;
}
