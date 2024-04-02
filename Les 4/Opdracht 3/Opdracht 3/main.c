#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)    (1 << (x))

 void adcInit( void )
 {
	 ADMUX = 0b01100011;            // AREF=VCC, result left adjusted, channel1 at pin PF1, ADLAR = 1
	 ADCSRA = 0b10000110;        // ADC-enable, no interrupt, not start, free running off, division by 64
 }


 // Main program: ADC at PF1
 int main( void )
 {
	 DDRF = 0x00;                // set PORTF for input (ADC)
	 DDRA = 0xFF;                // set PORTA for output
	 DDRB = 0x00;                // set PORTB for input
	 adcInit();                    // initialize ADC

	 while (1)
	 {
		 // ADC Start Conversion
		 if (PINB & BIT(0)){
			 ADCSRA |= (1<<ADSC);
		 }

		 // Wait for conversion to complete
		 while (ADCSRA & (1<<ADSC));

		 //PORTA = ADCL;
		 PORTA = ADCH;
	 }
 }

