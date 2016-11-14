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

#ifndef GYRO_H
#define GYRO_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"

#define NUM_GYRO_DATA		5
#define GYRO_PORT			5

const static uint32_t GYRO_UPDATE_TIME = 50;

typedef struct Gyro {

    double value;
	
    uint32_t last_time_measured;

    Timer* timer;

} Gyro;

void gyro_init(Gyro* gyro, Timer* timer);

void gyro_reset(Gyro* gyro);

void gyro_measure(Gyro* gyro);

#endif
