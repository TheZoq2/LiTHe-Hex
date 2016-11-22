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

#include "table.h"
#include <math.h>
#include "ir.h"

#define LEN_BETWEEN_SIDE_IR 10
#define PERCENT_FAULT_TOLERANCE_ANGLE 0.5

//uint8_t left_distance();
//uint8_t right_distance();
float corridor_angle();

void table_init(MainTable* table, IR ir_list[NUM_SENSORS]){
	table->ir_list = ir_list;
	table->front_distance = 0;
	table->down_distance = 0;
	table->corridor_angle = 0;
}

void update(MainTable* table, Lidar* lidar) {
	table->front_distance = lidar->value;
	table->corridor_angle = corridor_angle(table);
	table->down_distance = table->ir_list[0].value;
}

float corridor_angle(MainTable* ir_back) {
	double angle_left_side = atan((table->ir_list[1].value - table->ir_list[2].value)/LEN_BETWEEN_SIDE_IR);
	double angle_right_side = atan((table->ir_list[3].value - table->ir_list[4].value)/LEN_BETWEEN_SIDE_IR);
	if(fabs(angle_left_side - angle_right_side) / angle_left_side > PERCENT_FAULT_TOLERANCE_ANGLE) {
		return 90.0 // Can not tell angle, to much diff at sides
	}
	return (fabs(a) + fabs(b)) / 2
}