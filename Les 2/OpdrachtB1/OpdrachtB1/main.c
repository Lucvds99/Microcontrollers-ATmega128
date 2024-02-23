#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

ISR( INT0_vect ) {
	PORTD |= (1<<5);
}

ISR( INT1_vect ) {
	PORTD &= ~(1<<5);
}

int main( void ) {
	// Init I/O
	DDRD = 0xF0;			// PORTD(7:4) output, PORTD(3:0) input
	
	// Init Interrupt hardware
	EICRA |= 0x0B;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0x03;			// Enable INT1 & INT0
	
	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();
	
	while (1) {
		PORTD ^= (1<<7);	// Toggle PORTD.7
		wait( 500 );
	}
	
	return 1;
}