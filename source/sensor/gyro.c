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
#include "adc.h"

#define GYRO_ZERO   732
/*double gyro_value_to_rad(uint16_t val);
void gyro_add_data(Gyro* gyro, uint16_t data);
double latest_gyro_value(Gyro* gyro);
bool gyro_has_new_value(Gyro* gyro);
*/
void gyro_init(Gyro* gyro, Timer* timer) {

        gyro->value = 0.0;

        gyro->last_time_measured = 0;

        gyro->timer = timer;
}

void gyro_reset(Gyro* gyro) {

    gyro->value = 0.0;

    gyro->last_time_measured = 0;

}

/* Move all element one step forward (remove first) and add a new data value last in data_list */
/*void gyro_add_data(Gyro* gyro, uint16_t data) {

	for(uint8_t i = 0; i < NUM_GYRO_DATA-1; i++) {
		gyro->data_list[i] = gyro->data_list[i+1];
	}
	gyro->data_list[NUM_GYRO_DATA-1] = gyro_value_to_rad(data);

}*/

/*double gyro_value_to_rad(uint16_t val) {
    // TODO implement
    return (double)val;
}

double latest_gyro_value(Gyro* gyro) {

    return gyro->data_list[NUM_GYRO_DATA - 1];
}

bool gyro_has_new_value(Gyro* gyro) {

    return timer_value_millis(gyro->timer) - 
        gyro->last_time_measured >= GYRO_UPDATE_TIME;
}
*/
void gyro_measure(Gyro* gyro) {

    double raw = ((double)adc_read(GYRO_PORT) - (double)GYRO_ZERO) / 10000000;
	double time = (double)timer_value_micros(gyro->timer);
    gyro->value += (double) (raw * (time - (double)gyro->last_time_measured));
	gyro->last_time_measured = timer_value_micros(gyro->timer);
	PORTD = raw;
}

