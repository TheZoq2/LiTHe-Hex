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

///////////////////////////////////////////////////////////////////
// "Private"
///////////////////////////////////////////////////////////////////

/*
 * Initializes an IRElem as a dummy.
 */
void dummy_init(IRElem* dummy) {
    dummy->port = DUMMY_PORT;
    dummy->time_since_measured = 0;
}

/*
 * Checks whether an IRElem is a dummy.
 */
bool is_dummy(IRElem* elem) {
    return elem->port == DUMMY_PORT;
}

bool contains(IRQueue* queue, irport_t port) {
    for (uint8_t i = 0; i < queue->curr_size; ++i) {
        if (elements[i].port == port) return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////

void ir_queue_init(IRQueue* queue) {

    // set all elements to dummies
    for (uint8_t i = 0; i < NUM_SENSORS; ++i) {
        IRElem e;
        dummy_init(&e);
        queue->elements[i] = e;
    }

    queue->curr_size = 0;
}

void schedule(IRQueue* queue, irport_t port) {
    // TODO implement
}

bool has_new_value(IRQueue* queue) {
    // TODO implement
}

irport_t dequeue(IRQueue* queue) {
    // TODO implement
}

