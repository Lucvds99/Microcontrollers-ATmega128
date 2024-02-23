#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)(1 << (x))

int main( void ) {
	DDRB = 0xFF;
	int numbers[16] = {
		0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
		0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, ~0x8E
	};
	int error = 0xF9;
	int index = 0;
	while (1) {
		if (PIND & BIT(0))
		{
			index++;
		}
		if (PIND & BIT(1))
		{
			index--;
		}
		if (PIND & BIT(0) && PIND & BIT(1))
		{
			index = 0;
		}
		
		if (index>15)
		{
			PORTB = error;
		}else
		{
			PORTB = numbers[index];
		}
		_delay_ms(100);
	}

	return 1;
}