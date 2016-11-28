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

#define PERCENT_FAULT_TOLERANCE_ANGLE 0.5
#define NUM_BYTE_SEND				  5

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
	table->down_distance = table->ir_list[DOWN].value;
}

float corridor_angle(MainTable* table) {
	double angle_left_side = atan((table->ir_list[FRONT_LEFT].value - table->ir_list[BACK_LEFT].value)/LEN_BETWEEN_SIDE_IR);
	double angle_right_side = atan((table->ir_list[FRONT_RIGHT].value - table->ir_list[BACK_RIGHT].value)/LEN_BETWEEN_SIDE_IR);
	if(fabs(angle_left_side - angle_right_side) / angle_left_side > PERCENT_FAULT_TOLERANCE_ANGLE) {
		return M_PI; // Can not tell angle, to much diff at sides
	}
	return (fabs(angle_left_side) + fabs(angle_right_side)) / 2;
}

void get_sensor_data(Frame* frame) {
	uint8_t i = 0;
	for(i = 0; i < NUM_SENSORS; i++) {
		frame->msg[i] = mainTable->ir_list[i].value;
	}
	frame->msg[NUM_SENSORS] = (uint8_t)(mainTable->front_distance >> 8);
	frame->msg[NUM_SENSORS + 1] = (uint8_t)mainTable->front_distance;
	frame->len = NUM_SENSORS + 2;
	//for() {
	//	frame[i++] = mainTable->corridor_angle;
	//}
}

void send_sensor_wall_data(Frame* frame) {
	
}