#include "servo.h"

#include <stdio.h>

#ifndef SHOULD_PRINT
	#define printf
#endif

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
	return '0';
}
