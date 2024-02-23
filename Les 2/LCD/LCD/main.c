/*
 * main.c
 *
 * Created: 09/02/2024 15:10:53
 *  Author: Luc
 */ 
#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcdController.h"

void setup(){
	
}

int main(){
	lcd_init();
	lcd_display_text("hey");
	lcd_set_cursor(0);
	while(1){
		for (int i = 0; i<32; i++)
		{
			lcd_set_cursor(i);
			_delay_ms(250);
			lcd_display_text("hey");
			_delay_ms(250);
		}
	}
	return 0;
}
