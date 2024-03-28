/*
 * Eindopdracht.c
 *
 * Created: 15/03/2024 10:43:35
 * Author : teunl
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "include/lcdController.h"

char buffer = 0b00000000; // Initieel byte met alle bits op 0



#define SECOND_ON_PRE_1024 9766

void wait(int ms);
char word_buffer;
uint16_t morse_units = 0;
volatile uint8_t READING_INPUT = 0;

typedef struct {
	char morseByte; // 8-bit representation of Morse code
	char character; // Corresponding character
} MorseMapping;

MorseMapping morseMap[26] = {
	{0b00000111, 'A'},
	{0b11010101, 'B'},
	{0b11011101, 'C'},
	{0b00110101, 'D'},
	{0b00000001, 'E'},
	{0b01011101, 'F'},
	{0b00111101, 'G'},
	{0b01010101, 'H'},
	{0b00000101, 'I'},
	{0b01111111, 'J'},
	{0b00110111, 'K'},
	{0b01110101, 'L'},
	{0b00001111, 'M'},
	{0b00001101, 'N'},
	{0b00111111, 'O'},
	{0b01111101, 'P'},
	{0b11110111, 'Q'},
	{0b00011101, 'R'},
	{0b00010101, 'S'},
	{0b00000011, 'T'},
	{0b00010111, 'U'},
	{0b01010111, 'V'},
	{0b00011111, 'W'},
	{0b11010111, 'X'},
	{0b11011111, 'Y'},
	{0b11110101, 'Z'}
};

// Function to convert a character to its Morse code byte representation
char charToMorseByte(char c, MorseMapping morseMap[]) {
	// Iterate through the Morse mapping array to find the corresponding Morse byte
	for (int i = 0; i < 26; i++) {
		if (morseMap[i].character == c) {
			return morseMap[i].morseByte;
		}
	}
	return 0; // Return 0 if character not found (not in Morse mapping)
}

// Function to convert a Morse code byte representation to its corresponding character
char morseByteToChar(char morseByte, MorseMapping morseMap[]) {
	// Iterate through the Morse mapping array to find the corresponding character
	for (int i = 0; i < 26; i++) {
		if (morseMap[i].morseByte == morseByte) {
			return morseMap[i].character;
		}
	}
	return '\0'; // Return null character if Morse byte not found (not in Morse mapping)
}

char *generateByteFromTime(int time, char *buffer) {
		// Als de tijd kleiner is dan 3 (uur of minuut)
		if (time < 3) {
			*buffer = (*buffer << 2) | 0b01; // Shift eerst de buffer byte naar links en voeg vervolgens 01 toe
			//*buffer = '.';
			} else {
			*buffer = (*buffer << 2) | 0b11; // Shift eerst de buffer byte naar links en voeg vervolgens 11 toe
			//*buffer = '-';
		}
		return (char *)buffer;
	}
void resetBuffer(char *buffer) {
	*buffer = 0b00000000; // Set the buffer byte to 0b00000000
}

ISR(INT0_vect)
{
	READING_INPUT = (PIND & 1);
	if(READING_INPUT == 0 )
	{
		char count [16];
		//get morse value and use morse map to determine Letter
		lcd_clear_display();
		generateByteFromTime(morse_units, &buffer);
		sprintf(count, "%d", morse_units);
		lcd_display_text(count);
		PORTB = (0 << 0) ; // Turn the LED off
	}else
	{
		TIFR = (1 << OCF1A ) ; // clear the CTC flag ( writing a logic one to the set flag clears it)
		PORTB = (1 << 0) ; // Turn the LED on
		morse_units = 1;
	}
	
}

int main(void)
{
	DDRB |= (1 << 0) | (1 << 1) ; // Set LED as output
	TCCR1B |= (1 << WGM12 ); // Configure timer 1 for CTC mode
	OCR1A = (SECOND_ON_PRE_1024/2); // Set CTC compare value to 1Hz at 1 MHz AVR clock , with a prescaler of 64
	TCCR1B |= ((1 << CS10 ) | (0 << CS11 )) | (1 << CS12) ; // Start timer at Fcpu /1024
	
	// Initialiseer knop
	EICRA |= (1 << ISC00);      // Externe interrupt trigger op any digital change
	EIMSK |= (1 << INT0);       // Activeer externe interrupt 0
	sei();
	
	
	
	
	lcd_init();
	lcd_clear_display();
	for (;;)
	{
		if ( TIFR & (1 << OCF1A ))
		{
			morse_units ++;
			if (READING_INPUT)
			{
				PORTB ^= (1 << 0) ; // Toggle the LED
				TIFR = (1 << OCF1A ) ; // clear the CTC flag ( writing a logic one to the set flag clears it)
			}else
			{
				if(morse_units == 7)
				{
					lcd_display_text(" ");
					morse_units = 1;
				}else if (morse_units == 3)
				{
					lcd_display_text(morseByteToChar(buffer, morseMap) + NULL);
					resetBuffer(&buffer);
					morse_units = 1;
				}
				
			}
			
		}
		
		
	}
	
	
	
}
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );            // library function (max 30 ms at 8MHz)
	}
}

