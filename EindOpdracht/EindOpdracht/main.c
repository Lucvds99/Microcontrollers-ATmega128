/*
 * Eindopdracht.c
 *
 * Created: 15/03/2024 10:43:35
 * Author : teunl
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "include/lcdController.h"

#define SECOND_ON_PRE_1024 9766

void wait(int ms);
char word_buffer;
uint16_t morse_units = 0;
volatile uint8_t READING_INPUT = 0;

ISR(INT0_vect)
{
	READING_INPUT = (PIND & 1);
	if(READING_INPUT == 0 )
	{
		//get morse value and use morse map to determine Letter
		char count[16];
		lcd_clear_display();
		sprintf(count, "%d", morse_units); // Convert integer to string
		lcd_display_text(count);
		morse_units = 1;
		PORTB = (0 << 0) ; // Toggle the LED
	}else
	{
		TIFR = (1 << OCF1A ) ; // clear the CTC flag ( writing a logic one to the set flag clears it)
		PORTB = (1 << 0) ; // Toggle the LED
	}
	
}

int main(void)
{
	DDRB |= (1 << 0) | (1 << 1) ; // Set LED as output
	TCCR1B |= (1 << WGM12 ); // Configure timer 1 for CTC mode
	OCR1A = (SECOND_ON_PRE_1024); // Set CTC compare value to 1Hz at 1 MHz AVR clock , with a prescaler of 64
	TCCR1B |= ((1 << CS10 ) | (0 << CS11 )) | (1 << CS12) ; // Start timer at Fcpu /1024
	
	// Initialiseer knop
	EICRA |= (1 << ISC00);      // Externe interrupt trigger op any digital change
	EIMSK |= (1 << INT0);       // Activeer externe interrupt 0
	sei();
	
	
	
	
	lcd_init();
	lcd_clear_display();
	for (;;)
	{
		if (READING_INPUT){
			if ( TIFR & (1 << OCF1A ))
			{
				morse_units ++;	
				PORTB ^= (1 << 0) ; // Toggle the LED
				TIFR = (1 << OCF1A ) ; // clear the CTC flag ( writing a logic one to the set flag clears it)	
			}
			
		}
		
	}
	
	
	
}
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );            // library function (max 30 ms at 8MHz)
	}
}

