#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LcdController.h"


#define LCD_E 	3
#define LCD_RS	2

void lcd_strobe_lcd_e(void);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);


void lcd_init(void)
{

	DDRC = 0xFF;
	PORTC = 0x00;
	
	 //Step 2 (table 12), set interface data-lenght, and amount of lines
	PORTC = 0x28; //4-bit mode, 2 lines
	lcd_strobe_lcd_e();
	// Step 3, clear display
	lcd_write_cmd(0x01);

	// Step 4 (table 12), turn on display
	lcd_write_cmd(0x0C);

	// Set cursor
	lcd_write_cmd(0x0F);
}

void lcd_display_text(char *str)
{
	for(;*str; str++){
		lcd_write_data(*str);
	}
}

void lcd_clear_display(){
	lcd_write_cmd(0x01);
}

void lcd_set_cursor(int position)
{
	lcd_write_cmd(0x80 + position);
}


void lcd_strobe_lcd_e(void) {
	PORTC |= (1<<LCD_E);	// E high
	_delay_ms(1);			// nodig
	PORTC &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);			// nodig?
}


void lcd_write_cmd(unsigned char byte) {
	// First nibble.
	PORTC = byte & 0xf0;
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	lcd_strobe_lcd_e();
}
void lcd_write_data(unsigned char byte) {
	// First nibble.
	PORTC = byte & 0xf0;
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}