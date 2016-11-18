#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdlib.h>

#include "macros.h"


#define DD_MOSI 6

//0 Indexed
#define PIN_RX_TOGGLE 2

#define DIRECTION_TX 1
#define DIRECTION_RX 0

//const uint32_t CPU_FREQ = 16000000;

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

void usart_init(uint32_t baud)
{
	//TXD+DD output RX input
	//DDRD = 0b11111110;
	//DDRD = 0b11111110;

	//UBRR0H = ((CPU_FREQ / 16 + baud / 2) / baud - 1) >> 8;
	//UBRR0L = ((CPU_FREQ / 16 + baud / 2) / baud - 1);
	UBRR0H = 0;
	UBRR0L = 0;

	//Enable receive + transmit
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	//Set frame format
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

void uart_wait()
{
	//Wait for buffer to be empty
	while(!(UCSR0A & (1<<UDRE0)))
	{
		
	}
}

void usart_transmit(uint8_t data)
{
	//Set output
	//PORTD = 0b00000000;

	//Wait for buffer to be empty
	uart_wait();
	
	UDR0 = data;
}

uint8_t usart_receive()
{
	//Wait for data to arrive
	while(!(UCSR0A & (1<<RXC0)))
	{
		int a = 0;
	}

	return UDR0;
}

void send_servo_command(uint8_t id, uint8_t instruction, void* data, uint8_t data_amount)
{
	//Enable uart tx, disable rx
	clear_bit(UCSR0C, RXEN0);
	//set_bit(UCSR0C, TXEN0);

	//Set the direction of the trirstate gate
	clear_bit(PORTD, PIN_RX_TOGGLE);

	uint8_t length = data_amount + 2;
	PORTD = PORTD & 0b11111011;
	usart_transmit(0xff);
	usart_transmit(0xff);
	usart_transmit(id);
	usart_transmit(length);
	usart_transmit(instruction);

	uint8_t checksum = id+length + instruction;

	for(uint8_t i = 0; i < data_amount; ++i)
	{
		uint8_t data_int = ((uint8_t*)data)[i];
		usart_transmit(data_int);

		checksum += data_int;
	}

	checksum = ~checksum;

	usart_transmit(checksum);
	
	uart_wait();
	
	//Reset the direction of the tristate gate
	//set_bit(PORTD, PIN_RX_TOGGLE);
}

//Remember to deallocate the parameters when they go out of scope
typedef struct
{
	uint8_t id;
	uint8_t length;
	uint8_t error;

	uint8_t parameter_amount;
	uint8_t* parameters;

	uint8_t checksum;
} ServoReply;

ServoReply receive_servo_reply()
{
	ServoReply servo_reply;

	//Receive the 2 start bytes. These are ignored for now
	//TODO: Ensure that they are 0xff
	usart_receive(); 
	usart_receive();

	servo_reply.id = usart_receive();
	servo_reply.length = usart_receive();
	servo_reply.error = usart_receive();

	malloc(sizeof(ServoReply) * servo_reply.length);

	for(uint8_t i = 0; i < servo_reply.length; ++i)
	{
		servo_reply.parameters[i] = usart_receive();
	}

	//TODO: Check the checksum
	servo_reply.checksum = usart_receive();

	return servo_reply;
}



int main(void)
{
	//spi_slave_init();
	

	DDRD = 0xfE;
	//
	usart_init(1000000);
	
	set_bit(PORTD, 2);
	//PORTD = 0b00000100;
	
	while(1)
	{
		//Enable uart tx, disable rx
		clear_bit(UCSR0C, RXEN0);
		//set_bit(UCSR0C, TXEN0);

		//Set the direction of the trirstate gate
		clear_bit(PORTD, PIN_RX_TOGGLE);

		usart_transmit(0xff);
		usart_transmit(0xff);
		usart_transmit(0x01);
		usart_transmit(0x07);
		usart_transmit(0x03);
		usart_transmit(0x1E);
		usart_transmit(0x00);
		usart_transmit(0x02);
		usart_transmit(0x00);
		usart_transmit(0x02);
		usart_transmit(0xD3);
		uart_wait();

		_delay_ms(100);
	}
}

