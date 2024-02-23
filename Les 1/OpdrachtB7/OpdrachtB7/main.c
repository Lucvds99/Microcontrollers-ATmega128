#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#define button(number) (1 << number)


int main(void)
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRD = 0x00;
	
	DDRC = 0xFF;
	int state = 0;
	while (1)
	{
		_delay_ms(500);
		switch(state){
			case 0:
			PORTA=0xFF;
			PORTB=0xFF;
			PORTC=0xFF;
			if(PIND & button(6)){
				state = 1;
			}
			if (PIND & button(5))
			{
				state = 2;
			}
			break;
			case 1:
			PORTA=0x00;
			PORTB=0xFF;
			PORTC=0xFF;
			if (PIND & button(5))
			{
				state = 2;
			}
			break;
			case 2:
			PORTA=0x00;
			PORTB=0x00;
			PORTC=0xFF;
			if (PIND & button(5))
			{
				state = 3;
			}
			break;
			case 3:
			PORTA=0x00;
			PORTB=0x00;
			PORTC=0x00;
			if (PIND & button(5)||PIND & button(6))
			{
				state = 4;
			}
			break;
			default:
			PORTA=0xF0;
			PORTB=0xF0;
			PORTC=0xF0;
			if (PIND & button(7))
			{
				state = 0;
			}
			break;
		}
	}
	return 0;
}