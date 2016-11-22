#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

#define DD_MOSI 6

void spi_slave_init(void)
{
	//Mosi output, everything else input
	DDRB = (1<<DD_MOSI);
	//Enable spi
	SPCR = (1 << SPE);
}

uint8_t spi_slave_receive(void)
{
	//Wait for transmission
	while(!(SPSR & (1<<SPIF)))
		;
	
	//Return the data
	return SPDR;
}

void usart_init(uint16_t baud)
{
	//Set baudrate
	UBRR0H = (uint8_t)(baud>>8);
	UBRR0L = (uint8_t)baud;

	//Enable receive + transmit
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	//Set frame format
	UCSR0C = (1<<USBS0)|(2<<UCSZ00);
}

void usart_transmit(uint8_t data)
{
	//Wait for buffer to be empty
	while(!(UCSR0A & (1<<UDRE0)))
	{
		
	}

	UDR0 = data;
}

int main(void)
{
	//spi_slave_init();
	
	//TXD+DD output RX input
	//DDRD = 0b11111110;
	DDRD = 0xff;

	usart_init(9600);
	
	while(1)
	{
		//Set output
		PORTD = 0b00000000;
		
		//Set output high
		//PORTD |= 0b00000010;
		
		//PORTD &= 0b11111101;
		
		usart_transmit(0x55);
	}
}

