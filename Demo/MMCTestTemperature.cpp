#include "mbed.h"
#include "MMC5603NJ.h"
MMC5603NJ mmc(p9, p10);
//Test to see the current temperature read in celsius and prints to console.
int main() {
    int temp = mmc.getTemp();
    printf("The current temp is %d C\n\r", temp);
    return;
}
