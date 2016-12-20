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

#ifndef TABLE_H
#define TABLE_H 

#include "ir.h"
#include "lidar.h"
#include "communication.h"

/*
 * Data structure that stores all sensor
 * values that are ready to send to the 
 * central unit.
 */
typedef struct MainTable {

    IR* ir_list;

    uint16_t front_distance;

    uint8_t left_distance;

    uint8_t right_distance;
	
	uint8_t down_distance;

    uint8_t corridor_angle;

} MainTable;

/*
 * A global pointer to the main table
 */
MainTable* main_table;

/*
 * Initializes the MainTable
 */
void table_init(MainTable* table, IR ir_list[NUM_SENSORS]);

/*
 * Updates the values in the MainTable
 */
void update(MainTable* table, Lidar* lidar);

void get_sensor_data(Frame* frame);

void get_wall_data(Frame* frame);

#endif /* ifndef MAINTABLE_H */
