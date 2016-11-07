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

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

const uint8_t NUM_SENSORS = 5;

typedef irport_t uint8_t;

enum Range {LONG_RANGE, SHORT_RANGE};

typedef struct IR {

    enum Range range;

    uint8_t id;

    irport_t port;

    double value;

    bool enabled;

} IR;

typedef struct IRCONTROL {
    
    IR sensors[NUM_SENSORS];

} IRCONTROL;

void ir_init(IRCONTROL* control);


