/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>
#include "usiTwiSlave.h"
#include "LCDtiny.h"

/* Pin information
 
 - contrast , 0 to view                                         --> PB2
 - Resgister Select (RS) , 1 for characters, 0 for commands     --> PB1
 - Read Write (R/W) , 0 to write, 1 to read                     --> PB0
 - Enable (0 -> 1 you can write until fall to 0)                --> PB6
 - Backlight , 1: on , 0:off                                    --> PD6
 - D0                                                           --> PD5
 - D1                                                           --> PD4
 - D2                                                           --> PD3
 - D3                                                           --> PD2
 - D4                                                           --> PA0
 - D5                                                           --> PA1
 - D6                                                           --> PD1
 - D7                                                           --> PD0
 
 LEDs
 
 - B3
 - B4
 
 */ 


/* I2C slave address */
#define ADDRESS 0x06

/* Commands */
#define CLEAR       0x01
#define PRINT       0x02
#define CONTRAST    0x03
#define CURSOR      0x04



int main(void)
{
    uint8_t counter = START_DATA;
    uint8_t executed = 0;
    
    asm("cli");
    /* set I/O directions */
    DDRB |= 0b01011111; 
    DDRD |= 0b01111111;
    DDRA |= 0b00000011;

    usiTwiSlaveInit(ADDRESS);
    asm("sei");
    
    initialise_LCD();
    clear_screen();
    clear_buffer();
    

    while(1)
    {
        /*_delay_ms(10);            // two leds blinking
        PORTB &= 0b11100111;
        _delay_ms(1000);
        PORTB |= 0b00011000;
        _delay_ms(1000);*/
        
       /* if(rxbuffer[81])
            PORTB |= 0b00011000;
        else
            PORTB &= 0b11100111; */
      

        
        if(rxbuffer[END_TRANSMISSION])
        {
            switch(rxbuffer[COMMANDS]) 
            {
                case CLEAR:
                    clear_screen();
                    executed = 1;
                    break;
            
                case PRINT:
                    set_display_position(rxbuffer[POSITION]);
                    while(rxbuffer[counter] != '\0') 
                    {
                        send_character_to_LCD((char)rxbuffer[counter]);
                        counter++;
                    }
                    counter = START_DATA;
                    executed = 1;
                    break;
           
                case CONTRAST:
                    if(rxbuffer[1])
                        show_screen();
                    else
                        hide_screen();
                    executed = 1;
                    break;
            }
        }
        
        if(executed)
        {
            clear_buffer();
            executed = 0;
        }
    }
    return 0;   
}


void shift_screen(int shift, int r_l){
    int i;
	
	PORTB &= 0xFE;	// write please
	PORTB &= 0xFD;	// send instruction
	
	for(i = 0; i < shift; i++)
	{
		_delay_ms(1);
		set_data_line(0b00011000);
		if(r_l)
			PORTD |= (1 << 3);	// if it is a right shift
		enable();
	}	
}

void initialise_LCD(void){
	_delay_ms(100);			// wait for power_up and auto-initialisation
    
	show_screen();
    
	send_instruction(0b00001101); //display on, no underline, cursor blink on 
    
	_delay_ms(1);
	
	send_instruction(0b00111000);	// 8bit, 2 lines, (5*7)
}

void send_character_to_LCD(char character){
	wait_not_busy();
    
	PORTB &= 0xFE;          // write please
    PORTB |= 0x02;	// send characters
	
	set_data_line((uint8_t)character);	//send_character
    
	enable();
}

void clear_screen(void){
	wait_not_busy();
	send_instruction(0b00000001);
}

void backlight_on(void){
    PORTD |= (1 << 6);
}

void backlight_off(void){
	PORTD &= 0xAF;
}

void show_screen(void){
	PORTB &= 0xFA;	// contrast please!
}

void hide_screen(void){
	PORTB |= (1 << 2);	// no contrast
}

void back_to_home(void){
	wait_not_busy();
	
	send_instruction(0b00000010);  //display and cursor back to home position
}

void set_display_position(int position){
	wait_not_busy();
    
	position = position | 0x80;
    
	send_instruction(position);	
}

void send_instruction(int instruction){
	
    PORTB &= 0xFE;	// write please
	PORTB &= 0xFD;	// send instruction

    
	set_data_line((uint8_t)instruction); // not sure if works... (not tested yet)
    
	enable();
}

void enable(void){
	PORTB |= (1 << 6);	// receive instruction
	PORTB &= 0xBF;
}

int get_adress(void){		
	int adress;
	adress = get_data(0);
	return adress;
}

int get_data(int flag){ 
	int adress;
    int pd, pa;

	
	PORTB |= 0x01;		// read please
	PORTB &= 0xFD;      // ready to read the busy flag
	
    DDRD &= 0xC0;			// set data lines as input before enable!!!
    DDRA &= 0xFC;
	    
	PORTB |= (1 << 6);	// receive instruction
	pa = PINA;
    pd = PIND;
	PORTB &= 0xBF;
    
    adress = ((pd & 0x01) << 7) | ((pd & 0x02)  << 5) | ((pa & 0x02) << 4) | ((pa & 0x01) << 4) | ((pd & 0x04) << 1) | ((pd & 0x08) >> 1) | ((pd & 0x10) >> 3) | ((pd & 0x20) >> 5);        // i had no choice, takes up 150 bytes :\
    
	DDRD |= 0x3F;			// re-set data lines as output
    DDRA |= 0x03;

    if(flag)
        return (adress & 0x80);
    else
        return (adress & 0x7f);	//erase busy flag

}	

void set_data_line(uint8_t data){
    /* Provides transformation from attiny pinout to LCD pinout */
    uint8_t temp;
    
    temp = data & 0x01; // first bit
    if(temp)
        PORTD |= (1 << 5);
    else
        PORTD &= ~(1 << 5);
    
    temp = (data & 0x02) >> 1; // second bit
    if(temp)
        PORTD |= (1 << 4);
    else
        PORTD &= ~(1 << 4);
    
    temp = (data & 0x04) >> 2; // third bit
    if(temp)
        PORTD |= (1 << 3);
    else
        PORTD &= ~(1 << 3);
    
    temp = (data & 0x08) >> 3; // fourth bit
    if(temp)
        PORTD |= (1 << 2);
    else
        PORTD &= ~(1 << 2);
    
    temp = (data & 0x10) >> 4; // fifth bit
    if(temp)
        PORTA |= 1;
    else
        PORTA &= ~1;
    
    temp = (data & 0x20) >> 5; // sixth bit
    if(temp)
        PORTA |= (1 << 1);
    else
        PORTA &= ~(1 << 1);
    
    temp = (data & 0x40) >> 6; // seventh bit
    if(temp)
        PORTD |= (1 << 1);
    else
        PORTD &= ~(1 << 1);
    
    temp = (data & 0x80) >> 7;// eighth bit
    if(temp)
        PORTD |= 1;
    else
        PORTD &= ~1;
}

void wait_not_busy(void){
	int busy = 1;
	while(busy)				// wait for no busy flag
	{				
		busy = get_data(1);
		
	}
}

void clear_buffer(void){
    uint8_t i;
    for(i = 0; i< RX_BUFFER_SIZE; i++)
        rxbuffer[i] = 0;
}