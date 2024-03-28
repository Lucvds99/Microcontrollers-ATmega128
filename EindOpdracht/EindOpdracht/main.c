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
#define TIMER_COMP_VALUE SECOND_ON_PRE_1024/2
#define DEBOUNCE_DELAY_MS 10 // Adjust this value as needed

volatile uint8_t READING_INPUT = 0;
volatile uint8_t BUTTON_PRESSED = 0;

void wait(int ms);
char word_buffer;
uint16_t morse_units = 0;


typedef struct {
	char morseByte; // 8-bit representation of Morse code
	char *character; // Corresponding character
} MorseMapping;

MorseMapping morseMap[26] = {
    {0b00000111, "a"},
    {0b11010101, "b"},
    {0b11011101, "c"},
    {0b00110101, "d"},
    {0b00000001, "e"},
    {0b01011101, "f"},
    {0b00111101, "g"},
    {0b01010101, "h"},
    {0b00000101, "i"},
    {0b01111111, "j"},
    {0b00110111, "k"},
    {0b01110101, "l"},
    {0b00001111, "m"},
    {0b00001101, "n"},
    {0b00111111, "o"},
    {0b01111101, "p"},
    {0b11110111, "q"},
    {0b00011101, "r"},
    {0b00010101, "s"},
    {0b00000011, "t"},
    {0b00010111, "u"},
    {0b01010111, "v"},
    {0b00011111, "w"},
    {0b11010111, "x"},
    {0b11011111, "y"},
    {0b11110101, "z"}
};


// Function to convert a character to its Morse code byte representation
char charToMorseByte(char *c, MorseMapping morseMap[]) {
	// Iterate through the Morse mapping array to find the corresponding Morse byte
	for (int i = 0; i < 26; i++) {
		if (morseMap[i].character == c) {
			return morseMap[i].morseByte;
		}
	}
	return 0; // Return 0 if character not found (not in Morse mapping)
}

// Function to convert a Morse code byte representation to its corresponding character
char *morseByteToChar(char morseByte, MorseMapping morseMap[]) {
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
		if (time < 2) {
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
	_delay_ms(DEBOUNCE_DELAY_MS);
	uint8_t buttonState = PIND & 1;
	
	// If the button is pressed and was previously released (debouncing)
	if (buttonState == 0 && BUTTON_PRESSED == 1) 
	{
		BUTTON_PRESSED = 0; // Mark button as pressed
		READING_INPUT = 0; // Indicate that input is being read
		
		//get morse value and use morse map to determine Letter
		generateByteFromTime(morse_units, &buffer);
		PORTB = 0; // Turn the LED off
	} else if (buttonState == 1 && BUTTON_PRESSED == 0) 
	{
		BUTTON_PRESSED = 1; // Mark button as released
		READING_INPUT = 1; // Indicate that input is being read
		
		morse_units = 0; // Reset morse units counter
		//PORTB = 1; // Turn LED on
	}
	
	
	//TIFR = (1 << OCF1A ) ; // clear the CTC flag ( writing a logic one to the set flag clears it)
	
}

int main(void)
{
	DDRB = 0xFF ; // Set LED as output
	TCCR1B |= (1 << WGM12 ); // Configure timer 1 for CTC mode
	OCR1A = (TIMER_COMP_VALUE); // Set CTC compare value to 1Hz at 1 MHz AVR clock , with a prescaler of 64
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
			
			TIFR = (1 << OCF1A ) ; // clear the CTC flag ( writing a logic one to the set flag clears it)
			printf("hey");
			if (READING_INPUT)
			{
				
				PORTB |= (1 << morse_units) ; // Toggle the LED
				
			}else
			{
				if(morse_units == 12)
				{
					lcd_display_text(" ");
				
				}else if (morse_units == 6)
				{
					lcd_display_text(morseByteToChar(buffer, morseMap));
					resetBuffer(&buffer);
				
				}
			}
		morse_units ++;				
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

