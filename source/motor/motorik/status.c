
#include "status.h"

void status_set_speed(CurrentStatus* status, uint8_t x_speed, uint8_t y_speed) {
    status->x_speed = (float)(x_speed - BIT_OFFSET) / BIT_OFFSET;
    status->y_speed = (float)(y_speed - BIT_OFFSET) / BIT_OFFSET;
}

void status_set_rotation(CurrentStatus* status, uint8_t rotation) {
    status->rotation = (float)(rotation - BIT_OFFSET) / BIT_OFFSET;
}

void status_set_servo_speed(CurrentStatus* status, uint8_t speed_lsb, uint8_t speed_msb) {
    status->servo_speed = (float)(speed_lsb | (speed_msb << 8)) / MAX_16_BIT;
}

