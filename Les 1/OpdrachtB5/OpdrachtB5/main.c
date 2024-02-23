#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
	int animation[] = {0b00000000,
		0b00011000,0b00111100,
		0b01111110,0b11111111,
		0b01111111,0b00111111,
		0b00011111,0b00001111,
		0b00000111,0b00000011,
		0b00000001,0b00000011,
		0b00000111,0b00001111,
		0b00011111,0b00111111,
		0b01111111,0b11111111,
		0b11111110,0b11111100,
		0b11111000,0b11110000,
		0b11100000,0b11000000,
		0b10000000,0b11000000,
		0b11100000,0b11110000,
		0b11111000,0b11111100,
		0b11111110,0b11111111,
		0b01111110,0b00111100,
		0b00011000,0b00000000
	};
	DDRD = 0xFF;
	while (1)
	{
		for (int i = 0; i < sizeof(animation); i++)
		{
			
			PORTD = animation[i];
			_delay_ms(150);
		}
	}
	return 0;
}