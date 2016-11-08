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

#ifndef IR_QUEUE_H
#define IR_QUEUE_H

#include <avr/io.h>

#include "ir.h"
#include "error.h"
#include <stdbool.h>

typedef struct IRElem {

    irport_t port;

    uint16_t time_since_measured;

} IRElem;

typedef struct IRQueue {

    IRElem elements[NUM_SENSORS]; 

    uint8_t curr_size;

} IRQueue;

/*
 * Initializes an IRQueue.
 */
void ir_queue_init(IRQueue* queue);

/*
 * Adds an IR-sensor with port "port" to the queue.
 * This should be done as soon as an ADC conversion for 
 * this sensor is done. Assumes that the port hasn't
 * already been scheduled.
 */
void schedule(IRQueue* queue, irport_t port);

/*
 * Checks whether the longest waiting element of the 
 * queue has waited long enough for there to be a new 
 * value from the sensor.
 */
bool has_new_value(IRQueue* queue);

/*
 * Returns and removed the port of the IR-sensor that
 * has waited the longest.
 */
irport_t dequeue(IRQueue* queue);

#endif
