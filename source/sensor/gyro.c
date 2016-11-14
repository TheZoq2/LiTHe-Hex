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

#include "gyro.h"
#include "math.h"

void gyro_init(GYRO gyro, Timer* timer) {

        gyro.enabled = true;

        gyro.value = 0.0;

        gyro.last_time_measured = 0;

        gyro.timer = timer;
}

/* Move all element one step forward (remove first) and add a new data value last in raw_data_list */
void gyro_add_data(GYRO* gyro, uint16_t data) {

	for(uint8_t i = 0; i < NUM_GYRO_DATA-1; i++) {
		gyto->raw_data_list[i] = gyro->raw_data_list[i+1];
	}
	gyro->raw_data_list[NUM_SENSOR_DATA-1] = gyro_value_to_rad(data);

}

/* For now just take first value from raw_data_list put as value */
void gyro_reduce_noise(GYRO* gyro) {
	
	gyro->value = gyrp->raw_data_list[0];
}

double gyro_value_to_rad(uint16_t val) {

    return (double)data;
}

double latest_gyro_value(GYRO* gyro) {

    return gyro->raw_data_list[NUM_GYRO_DATA - 1];
}

bool gyro_has_new_value(GYRO* gyro) {

    return timer_value_millis(gyro->timer) - 
        last_time_measured >= GYRO_UPDATE_TIME;
}

void gyro_schedule(GYRO* gyro) {

    gyro->last_time_measured = timer_value_millis(gyro->timer);
}

