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

#include "lidar.h"
#include "math.h"

double lidar_value_to_meters(uint32_t pulse_time);

void lidar_init(Lidar* lidar, Timer* timer) {

    lidar->value = 0.0;
    lidar->timer = timer;
}

void lidar_measure(Lidar* lidar) {
    
    // if the pin is already high, we should wait it out 
    while ((MONITOR_MASK & PIND) != 0);
    uint32_t start;
    uint32_t end;
    // wait until we detect a rising edge
    while ((MONITOR_MASK & PIND) == 0);
    start = timer_value_micros(lidar->timer);
    
    // wait until the falling edge
    while ((MONITOR_MASK & PIND) != 0);
    end = timer_value_micros(lidar->timer);

    uint32_t pulse_time = end - start;

    lidar->value = lidar_value_to_centimeters(pulse_time);
}

uint16_t lidar_value_to_centimeters(uint32_t pulse_time) {
	return floor(pulse_time / 10);
}
