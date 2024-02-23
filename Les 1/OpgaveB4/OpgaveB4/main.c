#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
	DDRD = 0xFF;
	while (1)
	{
		for (int i = 0; i < 8; i++)
		{
			PORTD ^= 1 << i;
			_delay_ms(50);
		}
		
	}
	return 0;
}