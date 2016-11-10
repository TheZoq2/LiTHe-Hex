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

#include "ir_queue.h"
#include "error.h"

///////////////////////////////////////////////////////////////////
// "Private"
///////////////////////////////////////////////////////////////////

/*
 * Initializes an IRElem as a dummy.
 */
void dummy_init(IRElem* dummy) {
    dummy->port = DUMMY_PORT;
    dummy->last_time_measured = 0;
}

/*
 * Checks whether an IRElem is a dummy.
 */
bool is_dummy(IRElem* elem) {
    return elem->port == DUMMY_PORT;
}

bool contains(IRQueue* queue, irport_t port) {
    for (uint8_t i = 0; i < queue->curr_size; ++i) {
        if (queue->elements[i].port == port) return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////

void ir_queue_init(IRQueue* queue, Timer* timer) {

    // set all elements to dummies
    for (uint8_t i = 0; i < NUM_SENSORS; ++i) {
        IRElem e;
        dummy_init(&e);
        queue->elements[i] = e;
    }

    queue->curr_size = 0;
    queue->timer = timer;
}

void schedule(IRQueue* queue, irport_t port) {

    if (queue->curr_size == NUM_SENSORS) error();


    IRElem e;
    e.port = port;
    e.last_time_measured = timer_value_millis(queue->timer);

    queue->elements[queue->curr_size] = e;
    queue->curr_size++;

}

bool has_new_value(IRQueue* queue) {
    if (queue->curr_size == 0) return false;
    return timer_value_millis(queue->timer) - 
        queue->elements[0].last_time_measured >= IR_UPDATE_TIME;
}

irport_t dequeue(IRQueue* queue) {
    irport_t port = queue->elements[0].port;
    
    // shift all values down
    for (int i = 0; i < queue->curr_size - 1; ++i) {
        queue->elements[i] = queue->elements[i + 1]; 
    }

    queue->curr_size--;

    return port;
}

