#ifndef ROBOPOLYLCD_H
#define ROBOPOLYLCD_H

#include "Arduino.h"
#include "Wire.h"

/* Screen Addresses*/
#define START			0x00
#define START_2ND_LINE	0x40

/* Functions */

void printLCD(char* string, int position);
void printLCDint(int number, int position);
void clearLCD(void);

#endif
