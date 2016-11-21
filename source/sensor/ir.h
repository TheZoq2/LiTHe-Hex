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
#define LEN_BETWEEN_SIDE_IR 10

typedef uint8_t irport_t;

enum Range {LONG_RANGE, SHORT_RANGE};
enum Placement {FRONT_LEFT, BACK_LEFT, FRONT_RIGHT, BACK_RIGHT, DOWN};

const static double LONG_BASE = 17391.0;
const static double LONG_EXP = -1.071;

const static double SHORT_BASE = 2680.8;
const static double SHORT_EXP = -1.018;

const static enum Range RANGES[NUM_SENSORS] = {
    SHORT_RANGE, LONG_RANGE, LONG_RANGE, LONG_RANGE, LONG_RANGE
};

const static enum Placement PLACEMENTS[NUM_SENSORS] = {
    DOWN, FRONT_LEFT, BACK_LEFT, FRONT_RIGHT, BACK_RIGHT
};

typedef struct IR {

    enum Range range;

    enum Placement placement;

    irport_t port;

    uint8_t value;
	
	double raw_data_list[NUM_SENSOR_DATA];

} IR;

void ir_init(IR ir_list[NUM_SENSORS]);

void ir_add_data(IR* ir, uint16_t data);

void ir_reduce_noise(IR* ir);

double latest_ir_value(IR* ir);

double ir_value_to_centimeters(uint16_t val, enum Range range);

#endif
