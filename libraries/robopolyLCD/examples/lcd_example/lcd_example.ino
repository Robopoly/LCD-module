/***************************************************************************************
 *
 * Title: lcd_example    
 * Description: Print some string and numbers to the robopoly's LCD module
 *
 ***************************************************************************************/
#include <prismino.h>
#include <Wire.h>
#include "robopolyLCD.h"


void setup()
{
  Wire.begin();    // start I2C communication in master mode
}

void loop()
{
  clearLCD();
  printLCD("Hello World!", START_2ND_LINE);  // print a text in second line
  delay(2000);
  clearLCD();
  printLCDint(42, START);  // print an int on first line
  // turn LED off
  delay(2000);
}

