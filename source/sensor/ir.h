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

#ifndef IR_H
#define IR_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_SENSORS			5
#define NUM_SENSOR_DATA		5
#define DUMMY_PORT			255

typedef uint8_t irport_t;

enum Range {LONG_RANGE, SHORT_RANGE};

const static enum Range RANGES[NUM_SENSORS] = {
    SHORT_RANGE, LONG_RANGE, LONG_RANGE, LONG_RANGE, LONG_RANGE
};

typedef struct IR {

    enum Range range;

    irport_t port;

    double value;
	
	uint16_t raw_data_list[NUM_SENSOR_DATA];

    bool enabled;

} IR;

void ir_init(IR ir_list[NUM_SENSORS]);

void ir_add_data(IR* ir, uint16_t data);

void ir_reduce_noise(IR* ir);

#endif