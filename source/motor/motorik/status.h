// Copyright 2016 Noak Ringman, Emil Segerbäck, Robin Sliwa, Frans Skarman, Hannes Tuhkala, Malcolm Wigren, Olav Övrebö

// This file is part of LiTHe Hex.

// LiTHe Hex is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// LiTHe Hex is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with LiTHe Hex.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STATUS_H
#define STATUS_H 

#include <stdint.h>
#include <stdbool.h>

typedef struct {

    volatile float x_speed;

    volatile float y_speed;

    volatile float rotation;
	
	volatile bool is_rotating;

    volatile uint16_t servo_speed;

    volatile bool return_to_neutral;

    volatile bool auto_mode;
	
	volatile bool speed_changed;

} CurrentStatus;

void status_init(volatile CurrentStatus* status);
void status_set_speed(volatile CurrentStatus* status, uint8_t x_speed, uint8_t y_speed);
void status_set_rotation(volatile CurrentStatus* status, uint8_t rotation);
void status_set_servo_speed(volatile CurrentStatus* status, uint8_t speed_lsb, uint8_t speed_msb);

#ifdef IS_UNIT_TEST
float uint8_to_float(uint8_t original);
#endif


#endif /* ifndef STATUS_H */
