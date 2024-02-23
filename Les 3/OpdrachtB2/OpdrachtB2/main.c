#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

	ISR(TIMER2_COMP_vect)
	{
		if (OCR2 == 194)
		{
			PORTD |= (1<<7);
			OCR2 = 116;
		}
		else
		{
			PORTD &= ~(1<<7);
			OCR2 = 194;
		}
	}

int main(void)
{

	
	TCCR2 = 0b00011101; // configure normal mode for the TCCR2 with external clock
	TCNT2 = 0x00;
	OCR2 = 194;
	DDRD = 0xFF;
	TIMSK |= (1<<7);
	sei();
	

	for (;;)
	{
		
	}
	return 0;
}