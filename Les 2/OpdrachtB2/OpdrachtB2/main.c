#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
void Next_Light_State();

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

ISR( INT1_vect ) {
	Next_Light_State();
}

ISR( INT2_vect ) {
	Next_Light_State();
}

int main( void ) {
	// Init I/O
	DDRD = 0xF0;			// PORTD(7:4) output, PORTD(3:0) input
	DDRA = 0xFF;
	// Init Interrupt hardware
	EICRA |= 0x2C;			// INT2 falling edge, INT1 rising edge
	EIMSK |= 0x06;			// Enable INT1 & INT2

	sei();

	while (1) {
		
	}

	return 1;
}
void Next_Light_State(){
	static int state = 0;

	
	if(state <8)
	{
		PORTA ^= 1 << state;
		state ++;
		_delay_ms(10);
	}else
	{
		state = 0;
	}
}