#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	signed int counter = 0;
	int flipMask = 0b01100000;
	PORTD = 1 << 5;
	while (1)
	{
		PORTD ^= flipMask;
		_delay_ms(500);
	}
	return 0;
}