#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

#define BIT(x)		( 1<<x )
#define DDR_SPI		DDRB			// spi Data direction register
#define PORT_SPI	PORTB			// spi Output register
#define SPI_SCK		1				// PB1: spi Pin System Clock
#define SPI_MOSI	2				// PB2: spi Pin MOSI
#define SPI_MISO	3				// PB3: spi Pin MISO
#define SPI_SS		0				// PB0: spi Pin Slave Select

// wait(): busy waiting for 'ms' millisecond
// used library: util/delay.h
void wait(int ms)
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms(1);
	}
}

void spi_masterInit(void)
{
	DDR_SPI = 0xff;							// All pins output: MOSI, SCK, SS, SS_display as output
	DDR_SPI &= ~BIT(SPI_MISO);				// 	except: MISO input
	PORT_SPI |= BIT(SPI_SS);				// SS_ADC == 1: deselect slave
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);	// or: SPCR = 0b11010010;
	// Enable spi, MasterMode, Clock rate fck/64, bitrate=125kHz
	// Mode = 0: CPOL=0, CPPH=0;
}


// Write a byte from master to slave
void spi_write( unsigned char data )
{
	SPDR = data; // Load byte to Data register --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); 			// Wait for transmission complete
}

// Write a byte from master to slave and read a byte from slave
// nice to have; not used here
char spi_writeRead( unsigned char data )
{
	SPDR = data;// Load byte to Data register --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); 			// Wait for transmission complete
	data = SPDR;   // New received data (eventually, MISO) in SPDR
	return data;							// Return received byte
}

// Select device on pinnumer PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
	PORTB &= ~BIT(chipNumber);
}

// Deselect device on pinnumer PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{

	PORTB |= BIT(chipNumber);
}

void spi_writeWord ( unsigned char adress, unsigned char data ){
	spi_slaveSelect(0);				// Select display chip
	spi_write(adress); 				// Register select
	spi_write(data); 				// 	set data value
	spi_slaveDeSelect(0);			// Deselect display chip
}

// Initialize the driver chip (type MAX 7219)
void displayDriverInit()
{
	spi_writeWord(0x09, 0xFF); // Register 09: Decode Mode | -> 1's = BCD mode for all digits
	
	spi_writeWord(0x0A, 0x0F); // Register 0A: Intensity | -> Level 4 (in range [1..F])

	spi_writeWord(0x0B, 0x04); // Register 0B: Scan-limit | -> 1 = Display digits 0..1

	spi_writeWord(0x0C, 0x01); // Register 0B: Shutdown register | -> 1 = Normal operation
}

// Set display on ('normal operation')
void displayOn()
{
	spi_writeWord(0x0C, 0x01); // Register 0B: Shutdown register | -> 1 = Normal operation
}

// Set display off ('shut down')
void displayOff()
{
	spi_writeWord(0x0C, 0x00); // Register 0B: Shutdown register | -> 1 = Normal operation
}

void writeLedDisplay( int value ){
	if (value >= 0){
		for (char i =1; i<=4; i++)
		{
			int digit = value % 10;
			spi_writeWord(i, digit); // digit adress: (digit place) | digit value: 0
			value = value/10;
		}
	}
	else if (value < 0 && value > -1000){
		for (char i =1; i<=3; i++)
		{
			int digit = (value * -1) % 10;
			spi_writeWord(i, digit); // digit adress: (digit place) | digit value: 0
			value = value/10;
		}
		spi_writeWord(4, 10);
	}
}

int main()
{
	// inilialize
	DDRB=0x01;					  	// Set PB0 pin as output for display select
	spi_masterInit();              	// Initialize spi module
	displayDriverInit();            // Initialize display chip

	// clear display (all zero's)
	for (char i =1; i<=4; i++)
	{
		spi_writeWord(i, 0); // digit adress: (digit place) | digit value: 0
	}
	wait(1000);

	// write 4-digit data
	writeLedDisplay(6757);
	wait(1000);



	return (1);
}