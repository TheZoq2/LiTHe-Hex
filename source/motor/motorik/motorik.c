//#define F_CPU 16000000UL

#include "avr_helper.h"

#ifndef IS_X86
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#endif

#include "macros.h"

#include "uart_lib.h"

#include <stdint.h>
#include <stdlib.h>



#define DD_MOSI 6

//0 Indexed
#define PIN_RX_TOGGLE 2

#define DIRECTION_TX 1
#define DIRECTION_RX 0


const uint8_t WRITE_DATA_INSTRUCTION = 0x03;
const uint8_t WRITE_REG_INSTRUCTION = 0x04;
const uint8_t ACTION_INSTRUCTION = 0x05;

const uint8_t TORQUE_ENABLE_ADDRESS = 0x18;
const uint8_t GOAL_POSITION_ADDRESS = 0x1E;

//const uint32_t CPU_FREQ = 16000000;

void send_servo_command(uint8_t id, uint8_t instruction, void* data, uint8_t data_amount)
{
	//Enable uart tx, disable rx
	//clear_bit(UCSR0C, RXEN0);
	//set_bit(UCSR0C, TXEN0);

	//Set the direction of the trirstate gate
	clear_bit(PORTD, PIN_RX_TOGGLE);

	uint8_t length = data_amount + 2;
	//PORTD = PORTD & 0b11111011;
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

void write_servo_data(uint8_t id, uint8_t address, uint8_t* data, uint8_t data_amount)
{
	uint8_t new_data_amount = data_amount + 1;
	
	uint8_t* new_data = malloc(new_data_amount);

	new_data[0] = address;

	for(uint8_t i = 0; i < data_amount; i++)
	{
		new_data[i+1] = data[i];
	}

	send_servo_command(id, WRITE_REG_INSTRUCTION, (void*)new_data, new_data_amount);

	free(new_data);
}

void write_2_bytes_to_servo(uint8_t id, uint8_t addres, uint8_t b1, uint8_t b2)
{
	uint8_t data[2] = {b1, b2};
	write_servo_data(id, addres, data, 2);
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
	

	set_ddr(DDRD, 0xfE);
	//
	usart_init();
	
	_delay_ms(100);
	
	
	uint8_t servo_id = 0x01;

	
	uint8_t command[2] = {0x01};
	write_servo_data(servo_id, TORQUE_ENABLE_ADDRESS, command, 1);

	{
		uint8_t command[4] = {0x01, 0xff, 0x00, 0x02};

		write_servo_data(servo_id, GOAL_POSITION_ADDRESS, command, 4);
	}
	
	
	while(1)
	{
		send_servo_command(servo_id, ACTION_INSTRUCTION, 0, 0);
		
		_delay_ms(100);
	}
}

