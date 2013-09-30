#include "robopolyLCD.h"

/* Commands */
#define CLEAR       0x01
#define PRINT       0x02
#define CONTRAST    0x03





void printLCD(char* string, int position){
  int i;
  
  Wire.beginTransmission(0x06);   // start transmit to device #6
  Wire.write(0);                  // write from buffer beginning
  Wire.write(PRINT);              //  command is to print
  for(i=0; i<80; i++)             // can't send more than 80 char
  {
    if(string[i] == '\0')        // if end of line, quit
      break;
    Wire.write(string[i]);
  }
  Wire.endTransmission();    // stop transmitting
  Wire.beginTransmission(0x06);
  Wire.write(81);      
  if((position < 0) || (position > 0x67))             
  	position = 0;
  Wire.write(position);				// write position
  Wire.write(1);					// write for end of transmission
  Wire.endTransmission();
}

void printLCDint(int number, int position){
	char digits[5];	// max int for a int 16bit unsigned is 65536 something
	sprintf(digits,"%d",number);
	printLCD(digits, position);
}

void clearLCD(void){
  Wire.beginTransmission(0x06);   // start transmit to device #6
  Wire.write(0);                  // write from buffer beginning
  Wire.write(CLEAR);              //  command is to print
  Wire.endTransmission();    // stop transmitting
  
  Wire.beginTransmission(0x06);	// write for end of transmission
  Wire.write(82);                  
  Wire.write(1);
  Wire.endTransmission();
}
