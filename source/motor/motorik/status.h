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

#define BIT_OFFSET  127
#define MAX_16_BIT  65535

typedef struct {

    float x_speed;

    float y_speed;

    float rotation;

    float servo_speed;

} CurrentStatus;

void status_set_speed(CurrentStatus* status, uint8_t x_speed, uint8_t y_speed);
void status_set_rotation(CurrentStatus* status, uint8_t rotation);
void status_set_servo_speed(CurrentStatus* status, uint8_t speed_lsb, uint8_t speed_msb);

#endif /* ifndef STATUS_H */
