#include <avr/io.h>
#include <avr/interrupt.h>
#include "include/LcdController.h"

// Definieer pinnen voor knop en LED
#define BUTTON_PIN  (1 << PD2)  // Pin 2 op Port D
#define LED_PIN     (1 << PB0)  // Pin 0 op Port B

// Definieer variabelen voor timing
volatile uint16_t timer_ticks = 0;
volatile uint16_t button_down_ticks = 0;
volatile uint8_t morse_buffer = 0;
volatile uint8_t morse_length = 0;
volatile uint8_t word_flag = 0;

// Functie om morsecode te decoderen naar letters
char decode_morse(uint8_t code) {
	switch(code) {
		case 0b01:
		 lcd_display_text("*");
		 return '*';   // *
		case 0b11: return '-';   // -
		// Voeg hier de rest van de morsecode toe
		default: return '?';     // Onbekende code
	}
}

// Timer interrupt service routine
ISR(TIMER1_COMPA_vect) {
	timer_ticks++;
}

// Externe interrupt service routine voor knop
ISR(INT0_vect) {
	if (bit_is_clear(PIND, PD2)) {
		// Knop is ingedrukt
		button_down_ticks = timer_ticks;
		} else {
		// Knop is losgelaten
		uint16_t duration = timer_ticks - button_down_ticks;
		
		if (duration >= 7000) { // 7 seconden
			// Leeg morsebuffer, dit geeft een nieuw woord aan
			morse_buffer = 0;
			morse_length = 0;
			word_flag = 1;
			} else if (duration >= 3000) { // 3 seconden
			// Voeg spaties toe aan morsebuffer om nieuwe letter aan te geven
			if (morse_length > 0) {
				morse_buffer <<= 1;
				morse_length++;
			}
			} else if (duration >= 1000) { // 1 seconde
			// Voeg punt toe aan morsebuffer
			morse_buffer <<= 1;
			morse_buffer |= 1;
			morse_length++;
			} else { // Korter dan 1 seconde
			// Voeg streepje toe aan morsebuffer
			morse_buffer <<= 1;
			morse_buffer |= 1;
			morse_buffer <<= 1; // Add extra 0 for encoding "-"
			morse_length++;
		}
	}
	button_down_ticks = 0;
}

int main(void) {
	lcd_init();
	lcd_display_text("hallo ");
	
	
	while(1){
		lcd_display_text("hallo ");
	}
	
	
	// Initialiseer timer
	TCCR1B |= (1 << WGM12);     // CTC mode
	OCR1A = 15625;              // Vergelijkswaarde voor 1 seconde bij prescaler 1024
	TIMSK |= (1 << OCIE1A);    // Activeer compare match interrupt
	TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler op 1024 inschakelen
	sei();                      // Activeer globale interrupts

	// Initialiseer knop
	EICRA |= (1 << ISC01);      // Externe interrupt trigger op falling edge
	EIMSK |= (1 << INT0);       // Activeer externe interrupt 0

	// Initialiseer pinnen
	DDRB |= LED_PIN;            // Zet LED pin als output
	DDRD &= ~BUTTON_PIN;        // Zet knop pin als input
	PORTD |= BUTTON_PIN;        // Activeer pull-up weerstand op knop pin

	while (1) {
		// Decodeer morsebuffer naar letter
		if (morse_length > 0 && !word_flag) {
			char letter = decode_morse(morse_buffer);
			if (letter != '?') {
				// Doe iets met de gedecodeerde letter, zoals LED aan/uit zetten
				if (letter == '*') {
					PORTB ^= LED_PIN; // Toggle LED
				}
				// Optioneel: voeg ruisonderdrukking toe
				morse_buffer = 0; // Reset morsebuffer na verwerking
				morse_length = 0;
			}
		}
		// Reset woordvlag na verwerking van woord
		if (word_flag) {
			word_flag = 0;
		}
		// Voeg eventueel een kleine vertraging toe om de CPU te ontlasten
	}

	return 0;
}
