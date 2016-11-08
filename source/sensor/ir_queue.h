#ifndef IR_QUEUE_H
#define IR_QUEUE_H

#include <avr/io.h>

#include "ir.h"

typedef struct IRElem {

    irport_t port;

    uint16_t time_since_measured;

} IRElem;

typedef struct IRQueue {



} IRQueue;

void schedule(IRQueue* queue, irport_t port);

bool has_new_value(IRQueue* queue);

irport_t dequeue(IRQueue* queue);

#endif
