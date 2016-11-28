#include "servo.h"

#include "avr_helper.h"

#ifndef IS_X86
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#endif

const uint8_t WRITE_DATA_INSTRUCTION = 0x03;
const uint8_t WRITE_REG_INSTRUCTION = 0x04;
const uint8_t ACTION_INSTRUCTION = 0x05;


const uint8_t ANGLE_LIMIT_ADDRESS = 0x06;
const uint8_t TORQUE_ENABLE_ADDRESS = 0x18;
const uint8_t GOAL_POSITION_ADDRESS = 0x1E;
const uint8_t ROTATION_SPEED_ADDRESS = 0x20;

const uint8_t TORQUE_ON = 0x01;

const uint8_t BROADCAST_ID = 0xFE;


const uint8_t SERVO_MAP[6][3] = {
	{1,3,5},
	{13,15,17},
	{7,9,11},
	{2,4,6},
	{14,16,18},
	{8,10,12},
};

void send_servo_command(uint8_t id, uint8_t instruction, const void* data, uint8_t data_amount)
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

void write_servo_data(uint8_t id, uint8_t address, const uint8_t* data, uint8_t data_amount)
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

void send_servo_action()
{
	send_servo_command(BROADCAST_ID, ACTION_INSTRUCTION, 0, 0);
}

void write_servo_single_byte(uint8_t id, uint8_t address, uint8_t value)
{
	write_servo_data(id, address, &value, 1);
}


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


void reset_servo_bounds(uint8_t id)
{
	uint8_t command[4] = {0x00, 0x00, 0x03, 0xff};

	write_servo_data(id, ANGLE_LIMIT_ADDRESS, command, 4);
}

void enable_servo_torque(uint8_t id)
{
	write_servo_single_byte(id, TORQUE_ENABLE_ADDRESS, TORQUE_ON);
}


void set_servo_angle(uint8_t id, uint16_t angle)
{
	uint8_t command[2] = {(uint8_t)(angle), (uint8_t)(angle >> 8)};

	write_servo_data(id, GOAL_POSITION_ADDRESS, command, 2);
}
void set_servo_rotation_speed(uint8_t id, uint16_t angle)
{
	uint8_t command[2] = {(uint8_t)(angle), (uint8_t)(angle >> 8)};

	write_servo_data(id, ROTATION_SPEED_ADDRESS, command, 2);
}

void init_all_servos()
{
	for(uint8_t i = 1; i < 19; ++i)
	{
		enable_servo_torque(i);
		_delay_ms(1);
		reset_servo_bounds(i);
		_delay_ms(1);
		set_servo_rotation_speed(i, 0x0100);
		_delay_ms(1);
	}
}


void set_leg_angles(enum Leg leg_index, uint16_t* angles)
{
	const uint8_t* ids = SERVO_MAP[leg_index];

	for (uint8_t i = 0; i < 3; ++i) 
	{
		set_servo_angle(ids[i], angles[i]);
		_delay_ms(5);
	}
}


