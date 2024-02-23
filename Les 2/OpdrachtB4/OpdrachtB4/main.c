#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)(1 << (x))

void wait(int delay){
	for(int i = 0; i < delay; i++){
		_delay_ms(1);
	}
}

typedef struct{
	unsigned char data;
	unsigned int delay;
}PATTERN_STRUCT;

PATTERN_STRUCT LightPattern[] = { {0x01, 100}, {0x02, 100}, {0x04, 100}, {0x08, 100}, {0x10, 100}, {0x20, 100}, {0x40, 100},
{0x01, 50}, {0x03, 50}, {0x07, 50}, {0x0F, 50}, {0x1F, 50}, {0x3F, 50}, {0x7F, 50},
{0x7F, 300}, {0x7E, 100}, {0x7C, 500}, {0x78, 100}, {0x70, 500}, {0x60, 100}, {0x40, 500}, {0x00, 100},
{0x00,0x00}};

int main( void ) {
	DDRB = 0xFF;
	
	while (1)
	{
		int index = 0;
		
		while(LightPattern[index].delay != 0){
			PORTB = LightPattern[index].data;
			wait(LightPattern[index].delay);
			index ++;
		}
	}

	return 1;
}