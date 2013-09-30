#ifndef LCD_BETA3_H
#define LCD_BETA3_H


void shift_screen(int shift, int r_l);	// left shift = 0
int get_adress(void);
void backlight_on(void);
void backlight_off(void);
void show_screen(void);
void hide_screen(void);

void initialise_LCD(void);
void send_character_to_LCD(char character);
void clear_screen(void);
void back_to_home(void);
void set_display_position(int position);
void send_instruction(int instruction);
int get_data(int flag); 
void enable(void);  //recieve instruction
void wait_not_busy(void);

void set_data_line(uint8_t data);
void clear_buffer(void);


#endif