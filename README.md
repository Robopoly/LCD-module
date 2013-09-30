# LCD screen

This document explains the assembly and usage of the LCD screen driver developed by robopoly. This module is driven by I2C protocol.

In order to make it work, you have to solder it, to burn the firmware and to include the library in your project.

The module is still in development; certain functions are yet to be implemented. Specifically:

* Text shift by timer
* Led blink by timer
* Backlight functions
* Contrast regulation by PWM
* Allow to print text where the cursor currently is

## Assembly

The module is composed by:

* LCD screen
* PCB
* 16 male pins
* 16 female pins
* micro match female
* SPI connector (female)
* 3x LEDs (LED1, LED2, PWR)
* 3x 1kOhm resistances (R1, R2, R3)
* 3x 10kOhm resistances (R5, R6, R7)
* 2x 100nF capacitors (C1, C2)
* AtTiny2313A uController

The total price of those components is 10CHF.

## Programming

Burn the .hex code with AVRdude and USBtinyISP by connecting it to the SPI port.

## Use

Installing the library is like with any [Arduino library](http://arduino.cc/en/Guide/Libraries), just copy _/library_ directory to your sketchbook directory.

### Function list

`void printLCD(char* string, int position);`

Send a string of maximum 80 characters to the LCD screen. You can also choose where to print it on the LCD screen:

* Positions form 0x00 to 0x27 are on first line (only displayed until 0x0F)
* Positions form 0x40 to 0x67 are on second line (only displayed until 0x4F)

`void printLCDint(int number, int position);`

Send an int (16 bit) to the lcd screen.

`void clearLCD(void);`

Erase everything.

## Examples

An example programs is also provided to show how to use these functions. See them under _File -> Examples -> robopolyLCD_ in Arduino IDE.

## Licence

This software is published under [LGPL](http://www.gnu.org/licenses/lgpl.html).