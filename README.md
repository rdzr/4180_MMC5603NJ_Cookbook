<p align="center">
  <img width="500" height="673" src="https://github.com/rdzr/4180_MMC5603NJ_Cookbook/blob/main/MMCboard.jpg">
</p>
Video Presentation link:

[4180 Final Project MMC Breakout](https://gtvault-my.sharepoint.com/personal/kcunningham35_gatech_edu/_layouts/15/stream.aspx?id=%2Fpersonal%2Fkcunningham35%5Fgatech%5Fedu%2FDocuments%2FThird%20Year%2FFall%202022%2FECE%204180%2FFinal%20Project%20Slides%2Emp4&ct=1670872417308&or=OWA-NT&cid=b07bff2d-fa36-9e94-3c51-46a70336727f&ga=1)

Video Demonstration link:

[4180 Final Project MMC Breakout Demo](https://gtvault-my.sharepoint.com/personal/kcunningham35_gatech_edu/_layouts/15/stream.aspx?id=%2Fpersonal%2Fkcunningham35%5Fgatech%5Fedu%2FDocuments%2FThird%20Year%2FFall%202022%2FECE%204180%2FFinal%20Project%20Demo%2Emp4&ct=1670872226699&or=OWA-NT&cid=6426f8e7-7105-84ab-5ba4-74cedad1d782&ga=1)
# 4180_MMC5603NJ_Cookbook
The MMC5603NJ is an ultra-small monolithic integrated 3-axis AMR magnetic sensor. It uses an I2C interface with compatility for I3C interfacing.
This repository serves as a wiki page for our magnetometer breakout boards that feature it as the primary piece for our ECE 4180 final project.

The breakout board was designed by Kallen Cunningham in Altium Designer with part models from Ultra Librarian. Prototype boards were manufactured by JLCPCB. The library and demo code was developed by Rhett Dozier in Mbed's Cloud Compiler.
# Table of Contents
* [Chip overview](#chip-overview)

* [Breakout board overview](#breakout-board-overview)

* [Parts needed](#parts-needed)

* [File Contents](#file-contents)

* [Usage examples](#usage-examples)

# Chip Overview

The MMC5603NJ is an ultra small (0.8 x 0.8 x 0.4 mm!) BGA package magnetometer chip that uses I2C interfacing to measure magnetic fields in the form of X, Y, and Z outputs.
The chip self-degausses and has on-board SET and RESETs. The magnetometer has a wide range of single-input DC power, ranging from 1.2 to 3.6 volts, and
the sampling rate of the chip automatically scales based on the power input.

The main feature of the chip is the ability to read the XYZ vectors by detecting the vector offsets of magneto-resistant wafers within the chip whenever they're
disturbed by an external magnetic field. Of course, a very strong magnetic field can cause issues with the measurements by distorting these wafers to the point they
can't go back to their default position, which is why the MEMSIC magnetic sensor on the chip has a magnetic strap that resets the wafers with a high current magnetic pulse.

For the XYZ vector measurements, the device has 3 modes of output: 20-bit, 18-bit, and 16-bit. All 3 modes use the respective out0 and out1 channels which are each 8 bits,
but the 18 and 20 bit modes use an additional out2 channel. Specifically, the 18-bit measurement mode uses the 2 MSBs and the 20-bit uses the 4 MSBs of the channel with the
remaining LSBs going unused. There is also an 8-bit TOut channel that lets you measure the operating temperature of the chip as a char value, with each bit equaling ~0.8 C.

The other channels that exist are Status1, ODR (output data rate), Internal Control 0, 1, and 2, Selftest values for the X, Y, and Z vectors, 
as well as selftest thresholds. Finally, there is a product ID address that should always transmit the value 0x10 or 16.


# Breakout Board Overview
<p align="center">
  <img width="200" height="200" src="https://github.com/rdzr/4180_MMC5603NJ_Cookbook/blob/main/MMCbreakout.png">
</p>
The breakout board includes 4 labeled pins (Vcc, Gnd, SDA, SCL) and pads for the MMC5603NJ chip, power filtering capacitor, and I2C pull-up resistors. Also included in the repository are Gerber/NC Drill and BOM/Pick and Place outputs confirmed to work with JLCPCB fabrication and PCB assembly services. It is reccomended that all boards produced include the capacitor soldered on, but the pull-up resistors can be left off and external resistors used instead to lower costs.

# Parts needed
* MMC5603NJ: https://www.digikey.com/en/products/detail/memsic-inc/MMC5603NJ/10452796
* Mbed LPC1768: https://www.sparkfun.com/products/9564
* M/M Jumper Wires: https://www.sparkfun.com/products/12795
* Resistors (various, needs to total up to 2.7k Ohms): https://www.sparkfun.com/products/10969

# File Contents
The header file MMC5603NJ.h and C++ file MMC5603NJ.cpp are the components necessary to the import the MMC5603NJ's capabilities to an Mbed project.
Including these in the Mbed's directory as well as an include and a class constructor using the appropriate I2C sda and scl pin lines allows you to interact with
the MMC5603NJ breakout board. For example,

```
#include "mbed.h"
#include "MMC5603NJ.h"

MMC5603NJ mmc(p9, p10);
```
Allows you to create a MMC5603NJ object for the Mbed LPC1768, using pin 9 as the sda and pin 10 as the scl.
# Usage Examples
Included with the wiki page is MMCDemo.cpp, a file meant to be ran on the Mbed to demonstrate various capabilities of the MMC5603NJ breakout board. It contains the following functions:

setRate(char rate) to change the output data rate, or ODR, for the sampling speed of continuous measurement mode.

testContMode() and contSampleForever() to test the MMC's continuous measurement mode

testSingleSampleModes() and singleSampleForever() to test the MMC's single-measurement mode

testTemperature() to test the temperature measurement of the MMC

testPID() as a way to check the device's product ID. Useful for debugging the SDA/SCL clock lines since it should always return 0x10, or 16 in decimal form.
