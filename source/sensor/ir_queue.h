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
