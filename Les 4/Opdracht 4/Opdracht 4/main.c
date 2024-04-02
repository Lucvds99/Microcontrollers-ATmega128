#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "lcdController.h"

#define BIT(x) (1 << (x))


// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}


// Initialize ADC: 10-bits (left justified), free running
void adcInit( void )
{
	ADMUX = 0b11100011;			// AREF=VCC, result left adjusted, channel1 at pin PF1, ADLAR = 1
	ADCSRA = 0b10000110;		// ADC-enable, no interrupt, not start, free running off, division by 64
}


// Main program: ADC at PF1
int main( void )
{
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	DDRB = 0x00;				// set PORTB for input
	adcInit();					// initialize ADC
	
	DDRD = 0xFF;

	lcd_init();

	lcd_write_cmd(0x01);
	lcd_set_cursor(42);
	lcd_display_text("brood");
	
	char buffer[20];

	while (1)
	{
		// ADC Start Conversion
		//if (PINB & BIT(0)){
		lcd_write_cmd(0x01);
		ADCSRA |= (1<<ADSC);
		sprintf(buffer, "%d", ADCH);
		lcd_set_cursor(2);
		lcd_display_text(buffer);
		wait(75);
		
		
		//}

		// Wait for conversion to complete
		while (ADCSRA & (1<<ADSC));

		//PORTA = ADCL;
		PORTA = ADCH;
	}

}