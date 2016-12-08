#include "servo.h"

#include "avr_helper.h"

#ifndef IS_X86
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "spi.h"
#endif

const uint8_t READ_DATA_INSTRUCTION = 0x02;
const uint8_t WRITE_DATA_INSTRUCTION = 0x03;
const uint8_t WRITE_REG_INSTRUCTION = 0x04;
const uint8_t ACTION_INSTRUCTION = 0x05;


const uint8_t ANGLE_LIMIT_ADDRESS = 0x06;
const uint8_t TORQUE_ENABLE_ADDRESS = 0x18;
const uint8_t GOAL_POSITION_ADDRESS = 0x1E;
const uint8_t ROTATION_SPEED_ADDRESS = 0x20;
const uint8_t RETURN_LEVEL_ADDRESS = 0x10;
const uint8_t MOVING_ADDRESS = 0x2E;
const uint8_t CCW_COMPLIANCE_SLOPE_ADDRESS = 0x1D;
const uint8_t PRESENT_POSITION_ADDRESS = 0x24;

const uint8_t TORQUE_ON = 0x01;
const uint8_t TORQUE_OFF = 0x00;
const uint8_t ONLY_REPLY_TO_READ = 0x01;

const uint8_t BROADCAST_ID = 0xFE;

const uint8_t NUM_SERVOS = 18;

const uint16_t SERVO_TARGET_COMPLIANCE_MARGIN = 20;


const uint8_t SERVO_MAP[6][3] = {
	{1,3,5},
	{13,15,17},
	{7,9,11},
	{2,4,6},
	{14,16,18},
	{8,10,12},
};

void send_servo_command(uint8_t id, uint8_t instruction, void* data, uint8_t data_amount)
{
	spi_set_interrupts(false);
	//Set the direction of the tristate gate
	//clear_bit(PORTD, PIN_RX_TOGGLE);

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
	
	spi_set_interrupts(true);
	
	//Reset the direction of the tristate gate
	//set_bit(PORTD, PIN_RX_TOGGLE);
}

void write_servo_data(uint8_t id, uint8_t address, const uint8_t* data, uint8_t data_amount)
{
	uint8_t new_data_amount = data_amount + 1;
	
	uint8_t* new_data = (uint8_t*)malloc(new_data_amount);

	new_data[0] = address;

	for(uint8_t i = 0; i < data_amount; i++)
	{
		new_data[i+1] = data[i];
	}

	send_servo_command(id, WRITE_DATA_INSTRUCTION, (void*)new_data, new_data_amount);

	free(new_data);
}

ServoReply read_servo_data(uint8_t id, uint8_t address, uint8_t length)
{
	//Send datarequest instruction
	uint8_t* new_data = (uint8_t*)malloc(2);

	new_data[0] = address;
	new_data[1] = length;

	//_delay_us(25);

	send_servo_command(id, READ_DATA_INSTRUCTION, (void*)new_data, 2);

	free(new_data);

	//Read the data
	return receive_servo_reply();
}

uint16_reply read_uint16_from_servo(uint8_t id, uint8_t address)
{
	ServoReply reply = read_servo_data(id, address, 2);

	uint16_reply result;

	//uint16_t result = (reply.parameters[1] << 8) + reply.parameters[0];

	if(reply.error != 0)
	{
		result.error = reply.error;
		result.is_error = true;
		result.result = 0;
	}
	else
	{
		result.result = (reply.parameters[1] << 8) + reply.parameters[0];
	}

	free_servo_reply(reply);

	return result;
}

#ifdef IS_X86
void send_servo_action()
{
}
#else
void send_servo_action()
{
	send_servo_command(BROADCAST_ID, ACTION_INSTRUCTION, 0, 0);
	//TODO: Olavs fel
	//_delay_ms(1200);
	while(!servos_are_done_rotating())
		;
}
#endif

void write_servo_single_byte(uint8_t id, uint8_t address, uint8_t value)
{
	write_servo_data(id, address, &value, 1);
}


ServoReply receive_servo_reply()
{
	spi_set_interrupts(false);
	//Switch the direction of the tri-state gate
	set_bit(PORTD, PIN_RX_TOGGLE);
	usart_set_direction(RX);

	//send_servo_command(1, 0, 0, 0);
	
	ServoReply servo_reply;

	//Receive the 2 start bytes. These are ignored for now
	//TODO: Ensure that they are 0xff
	usart_receive();
	usart_receive();

	servo_reply.id = usart_receive();
	servo_reply.length = usart_receive();
	servo_reply.parameter_amount = servo_reply.length - 2; //See ax12 datasheet
	servo_reply.error = usart_receive();

	if(servo_reply.error != 0)
	{
		goto failiure;
	}

	servo_reply.parameters = (uint8_t*) malloc(sizeof(uint8_t) * servo_reply.parameter_amount);

	for(uint8_t i = 0; i < servo_reply.parameter_amount; ++i)
	{
		servo_reply.parameters[i] = usart_receive();
	}

	//TODO: Check the checksum
	servo_reply.checksum = usart_receive();

failiure:
	//Reset the tri-state gate
	clear_bit(PORTD, PIN_RX_TOGGLE);
	usart_set_direction(TX);


	_delay_ms(5);
	
	spi_set_interrupts(true);
	
	return servo_reply;
}
void free_servo_reply(ServoReply reply)
{
	free(reply.parameters);
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
void disable_servo_torque(uint8_t id)
{
	write_servo_single_byte(id, TORQUE_ENABLE_ADDRESS, TORQUE_OFF);
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

void set_servo_compliance_thresholds(uint8_t id)
{
	uint8_t data[4] = {0x40, 0x01, 0x01, 0x40};

	write_servo_data(id, CCW_COMPLIANCE_SLOPE_ADDRESS, data, 4);
}

void init_all_servos()
{
	//Tell servos to only reply to read_data instructions
	write_servo_single_byte(BROADCAST_ID, RETURN_LEVEL_ADDRESS, ONLY_REPLY_TO_READ);

	for(uint8_t i = 1; i < 19; ++i)
	{
		//enable_servo_torque(i);
		_delay_ms(1);
		reset_servo_bounds(i);
		_delay_ms(1);
		set_servo_rotation_speed(i, 0x006f);
		_delay_ms(1);
		set_servo_compliance_thresholds(i);
		_delay_ms(1);
	}
}



void set_leg_angles(enum LegIds leg_index, uint16_t* angles)
{
	const uint8_t* ids = SERVO_MAP[leg_index];

	for (uint8_t i = 0; i < 3; ++i) 
	{
		printf("Moving servo %i\n", ids[i]);
		set_servo_angle(ids[i], angles[i]);
		_delay_ms(5);
	}
}

/*
	Reads the target positions for all the servos. Buffer must be a preallocated 
	pointer that fits NUM_SERVOS uints 
*/
void read_servo_target_positions(uint16_t* buffer)
{
	for (uint8_t i = 0; i < NUM_SERVOS; ++i) 
	{
		buffer[i] = read_uint16_from_servo(i + 1, GOAL_POSITION_ADDRESS).result;
	}
}

bool is_servo_position_in_bounds(uint16_t target_position, uint16_t current_position)
{
	return abs(target_position - current_position) < SERVO_TARGET_COMPLIANCE_MARGIN;
}

bool check_servo_done_rotating(uint8_t id, uint16_t target_position)
{
	uint16_reply current_position = 
		read_uint16_from_servo(id + 1, PRESENT_POSITION_ADDRESS);

	bool result = 
		is_servo_position_in_bounds(target_position, current_position.result) |
		current_position.is_error;
#ifdef IS_X86
	//Run the regular code but don't return it. 
	//This is to be able to check for memory leaks
	//using valgrind
	return read_simulator_servo_state(id) == '0';
#else
	return result;
#endif
}
bool servos_are_done_rotating()
{
#ifdef IS_X86
	//We need to wait for the simulator to process the command before checking this
	//Sleep for 0.1 seconds
	usleep(100000);
#endif

	uint16_t servo_targets[NUM_SERVOS];
	uint16_t* targ_ptr = servo_targets;

	read_servo_target_positions(servo_targets);

	for(uint8_t i = 0; i < NUM_SERVOS; i++)
	{
		if(check_servo_done_rotating(i, servo_targets[i]) == false)
		{
			return false;
		}
	}
	return true;
}

