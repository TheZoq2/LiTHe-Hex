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

//uint8_t left_distance();
//uint8_t right_distance();
float corridor_angle();

void table_init(MainTable* table, IR ir_list[NUM_SENSORS]){
	table->ir_list = ir_list;
	table->front_distance = 0;
	table->left_distance = 0;
	table->right_distance = 0;
	table->corridor_angle = 0;
}

void update(MainTable* table, Lidar* lidar) {
	table->front_distance = lidar->value;
	table->corridor_angle = corridor_angle(table->ir_list[1], table->ir_list[2]);
}

float corridor_angle(IR ir_front, IR ir_back) {
	double wall_len = squrt(pow(LEN_BETWEEN_SIDE_IR, 2) + pow((ir_back.value + ir_front.value), 2));
	double angle = M_PI - M_PI/2 - asin(wall_len/ir_front.value);
	return angle;
}