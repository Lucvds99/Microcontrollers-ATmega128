#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

	ISR(TIMER2_COMP_vect)
	{
		if (OCR2 == 25)
		{
			PORTD |= (1<<7);
			OCR2 = 15;
		}
		else
		{
			PORTD &= ~(1<<7);
			OCR2 = 25;
		}
	}

int main(void)
{

	
	TCCR2 = 0b00011100; // configure normal mode for the TCCR2 with external clock
	TCNT2 = 0x00;
	OCR2 = 25;
	DDRD = 0xFF;
	TIMSK |= (1<<7);
	sei();
	

	for (;;)
	{
		
	}
	return 0;
}