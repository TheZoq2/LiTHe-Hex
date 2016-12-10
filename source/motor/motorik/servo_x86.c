#include "servo.h"

#include <stdio.h>

#define STATE_IN_FILENAME "/tmp/hexsim/servo_states"

char read_simulator_servo_state(uint8_t id)
{
	//Try to open the servo state file
	FILE* in_file = fopen(STATE_IN_FILENAME, "r");

	//The file couldn't be opened for some reason. Notify the user and keep running
	if(in_file == NULL)
	{
		printf("Failed to open servo state file, assuming done rotating");
		return '0';
	}

	//Read the first 18 bytes of the file
	char buffer[18];

	//Servos are 1 indexed
	uint8_t target_index = id - 1;

	size_t read_amount = fread(buffer, 1, 18, in_file);

	if(target_index > read_amount - 1)
	{
		printf("Servo state file does not contain enough information, assuming done rotating");
		return '0';
	}

	fclose(in_file);

	return buffer[target_index];
}

#define BUFFER_SIZE 512
float read_leg_float_from_file(uint8_t id, char* filename)
{
	float result = 0;

	FILE* in_file = fopen(filename, "r");

	if(in_file == NULL)
	{
		printf("Failed to open servo state file, assuming done rotating");
		return result;
	}

	char buffer[BUFFER_SIZE];

	uint8_t target_index = id - 1;
	size_t read_amount = fread(buffer, 1, BUFFER_SIZE, in_file);

	size_t current_index = 0;
	size_t angles_read = 0;
	bool done = false;
	char string_buffer[BUFFER_SIZE];
	size_t current_string_index = 0;

	while(done == false && current_index <= BUFFER_SIZE)
	{
		//If this is the last byte of a string
		if(buffer[current_index] == '\n')
		{
			if(angles_read == target_index)
			{
				result = atof(string_buffer);
				done = true;
				break;
			}
			else
			{
				//clear the buffer
				for(size_t i = 0; i < BUFFER_SIZE; i++)
				{
					string_buffer[i] = 0;
				}
				current_string_index = 0;
				angles_read++;
			}
		}
		else
		{
			string_buffer[current_string_index] = buffer[current_index];
			current_string_index++;
		}

		current_index++;
	}

	return result;
}

float read_servo_angle(uint8_t id)
{
	return read_leg_float_from_file(id, "/tmp/hexsim/servo_angles");
}
float read_servo_target_angle(uint8_t id)
{
	return read_leg_float_from_file(id, "/tmp/hexsim/servo_targets");
}
