#include "ir_queue.h"

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

}

