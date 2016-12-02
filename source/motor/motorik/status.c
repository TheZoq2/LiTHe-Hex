
#include "status.h"

#define BIT_OFFSET  127
#define MAX_16_BIT  65535

float uint8_to_float(uint8_t original)
{
	//return (float)((int8_t)original - BIT_OFFSET) / BIT_OFFSET;
	return (((float)original)/255.0) * 2.0 - 1.0;
}

void status_init(CurrentStatus* status) {
    status->x_speed = 0.0;
    status->y_speed = 0.0;
    status->rotation = 0.0;
    status->servo_speed = 0.0;
    status->return_to_neutral = false;
    status->auto_mode = false;
}

void status_set_speed(CurrentStatus* status, uint8_t x_speed, uint8_t y_speed) {
    status->x_speed = uint8_to_float(x_speed);
    status->y_speed = uint8_to_float(y_speed);
}

void status_set_rotation(CurrentStatus* status, uint8_t rotation) {
    status->rotation = uint8_to_float(rotation);
}

void status_set_servo_speed(CurrentStatus* status, uint8_t speed_lsb, uint8_t speed_msb) {
    status->servo_speed = (float)(speed_lsb | (speed_msb << 8)) / MAX_16_BIT;
}

