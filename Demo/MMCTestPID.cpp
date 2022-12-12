#include "mbed.h"
#include "MMC5603NJ.h"
MMC5603NJ mmc(p9, p10);
//Sends the product ID to console. Should always be 16.
//Useful for confirming that you are communicating with the device.
int main() {
    int id = mmc.PID();
    printf("The returned PID is %d\n\r", id);
    return;
}
