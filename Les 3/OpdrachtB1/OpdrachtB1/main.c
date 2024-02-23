/*
 * OpdrachtB1.c
 *
 * Created: 23/02/2024 10:39:22
 * Author : Luc
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcdController.h"


int main(void)
{
	
	
	DDRA = 0xFF;
	PORTA = 0x00;
	
	TCCR2 = 0b00000111; // configure normal mode for the TCCR2 with external clock
	TCNT2 = 0x00;
	
	lcd_init();
	char buffer[20]; // Buffer to hold the converted string
	for (;;)
	{
		PORTA = TCNT2;
		lcd_write_cmd(0x01);
		sprintf(buffer, "%d", TCNT2); // Convert integer to string
		
		lcd_display_text(buffer);
		_delay_ms(100);
	}
	return 0;
}


