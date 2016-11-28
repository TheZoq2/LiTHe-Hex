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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#define GARBAGE     0x00

#define MAX_MESSAGE_LENGTH 20
#define MSG_PARITY_MASK 0x02
#define TYPE_PARITY_MASK 0x01

#include "spi.h"
#include <stdint.h>

enum ID {
    SEND_FAIL = 0x1F, ACKNOWLEDGE = 0x0F, DATA_REQUEST = 0x02, 
    TOGGLE_OBSTACLE = 0x03, SET_SERVO_SPEED = 0x20, WALK_COMMAMD = 0x21, 
    RETURN_TO_NEUTRAL = 0x05, SERVO_STATUS = 0x22, DEBUG_STRING = 0x23, 
    OBSTACLE = 0x03, SENSOR_DATA = 0x24, CORRIDOR_DATA = 0x25
};

typedef struct Frame {
	uint8_t control_byte;
	uint8_t len;
	uint8_t msg[MAX_MESSAGE_LENGTH];
} Frame;

void on_spi_recv();

#endif /* ifndef COMMUNICATION_H */
