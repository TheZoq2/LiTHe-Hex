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

#include "ir.h"
#include "math.h"

#define NUM_CORRECT_DATA_POINTS	4
#define PERCENT_FAULT_TOLERANCE 0.20
#define CORRECTION_FACTOR		20

void ir_init(IR ir_list[NUM_SENSORS]) {

    for (uint8_t i = 0; i < NUM_SENSORS; ++i) {

        IR ir;

        ir.range = RANGES[i];

        ir.placement = PLACEMENTS[i];

        ir.port = i;
        
        ir.value = 0.0;

        ir_list[i] = ir;
    }
}

/* Move all element one step forward (remove first) and add a new data value last in raw_data_list */
void ir_add_data(IR* ir, uint16_t data) {

	for(uint8_t i = 0; i < NUM_SENSOR_DATA-1; i++) {
		ir->raw_data_list[i] = ir->raw_data_list[i+1];
	}
	ir->raw_data_list[NUM_SENSOR_DATA-1] = ir_value_to_centimeters(data, ir->range);

}

/* Remove data if out of fault_tolerance, compensate for more recent value before calculate new ir->value */
void ir_reduce_noise(IR* ir) {
	
	// Reverse raw_data_list
	double data_list[NUM_SENSOR_DATA];
	uint8_t j = NUM_SENSOR_DATA-1;
	for(uint8_t i = 0; i < NUM_SENSOR_DATA; i++) {
		data_list[j] = ir->raw_data_list[i];
		j--;
	}

	uint8_t num_data_points = 0;
	double res = 0; 

	for(uint8_t i = 0; i < NUM_SENSOR_DATA; i++) {

		// If difference more then PRECENT_FAULT_TOLERANCE, remove data
		uint8_t remove_data = 1;
		for(uint8_t j = 0; j < NUM_SENSOR_DATA; j++) {
			if(fabs(data_list[j] - data_list[i]) / data_list[j] > PERCENT_FAULT_TOLERANCE) {
				remove_data++;
			}
		} 

		// remove_data = number of points outside FAULT_TOLERANCE, not add data to res
		if(remove_data > NUM_CORRECT_DATA_POINTS) continue;

		// If current value less/more then current average add/remove correction factor before add to res
		if(res / num_data_points > data_list[i]) {

			res += data_list[i]*(1.0+CORRECTION_FACTOR*i);
			
		} else if(res / num_data_points < data_list[i]) {

			res += data_list[i]*(1.0-CORRECTION_FACTOR*i);
			
		} else {
			res += data_list[i];
		}

		num_data_points++;

	}
	// average of res
	double val = res / num_data_points;
	if(val < 160) {
		ir->value = (uint8_t)val;
	} else {
		ir->value = 255;
	}
	ir->value = ir->raw_data_list[NUM_SENSOR_DATA-1];
}

double ir_value_to_centimeters(uint16_t val, enum Range range) {

    if (range == LONG_RANGE) {
        
        return (LONG_BASE * pow(val, LONG_EXP));

    } else {

        return (SHORT_BASE * pow(val, SHORT_EXP));
    
    }
}

double latest_ir_value(IR* ir) {
    return ir->raw_data_list[NUM_SENSOR_DATA - 1];
}

