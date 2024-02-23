#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
	DDRC = 0;
	DDRD = 0xFF;
	int flipMask = 0b10000000;
	while (1)
	{
		if (PINC & 0b00000001)
		{
			PORTD ^= flipMask;
			_delay_ms(500);
		}
		else{
			PORTD = 0;
		}
	}
	return 0;
}