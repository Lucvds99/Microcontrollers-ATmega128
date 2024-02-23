#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void variableDelay(int delay);

int main(void)
{
	int flipmask = 0b00000001;
	int delay= 1000;

	DDRD = 0xFF;
	DDRC = 0b11111110;
	while (1)
	{
		
		if (PINC & 0b00000001)
		{
			if (delay == 1000)
			{
				delay = 250;
			}
			else
			{
				delay = 1000;
			}
		}
		PORTD ^= flipmask;
		variableDelay(delay);
	}
	return 0;
}

void variableDelay(int delay)
{
	if(delay == 250){
		_delay_ms(250);
	}
	if(delay == 1000){
		_delay_ms(1000);
	}
}