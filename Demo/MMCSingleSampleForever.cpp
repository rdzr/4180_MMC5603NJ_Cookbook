#include "mbed.h"
#include "MMC5603NJ.h"
MMC5603NJ mmc(p9, p10);
//Samples XYZ axes via both single-input methods forever and prints to console.
int main() {
    while(1) {
        mmc.getAxis();
        printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
        mmc.getX();
        mmc.getY();
        mmc.getZ();
        printf("X: %f Y: %f Z: %f\n\r", mmc.x, mmc.y, mmc.z);
    }
}
