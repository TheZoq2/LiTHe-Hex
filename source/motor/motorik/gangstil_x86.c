#include "gangstil.h"

#include <stdio.h>

#define STATE_OUT_FILENAME "/tmp/hexsim/leg_input"

void write_current_state()
{
	FILE* out_file = fopen(STATE_OUT_FILENAME, "w");

	if(out_file == NULL)
	{
		printf("Failed to open %s\n", STATE_OUT_FILENAME);
		exit(-1);
	}

	fwrite("[\n", 1, 2, out_file);

	for(uint8_t i = 0; i < NUM_LEGS; ++i) 
	{
		struct Leg angles = current_servo_state.angles[i];
		Point2D position = current_servo_state.points[i];
		float height = current_servo_state.heights[i];

		char buffer[512];

		const char* string_end = ",\n";
		if(i == NUM_LEGS-1)
		{
			string_end = "\n";
		}

		int length = sprintf(
					buffer, 
					"{\"angles\": [%f, %f, %f], \"point\": {\"x\": %f, \"y\": %f, \"z\": %f}}%s",
					angles.angle1,
					angles.angle2,
					angles.angle3,
					position.x,
					position.y,
					height,
					string_end
				);

		fwrite(buffer, 1, length, out_file);
	}

	fwrite("]\n", 1, 2, out_file);

	fclose(out_file);
}

