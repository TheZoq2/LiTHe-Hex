
#ifndef ERROR_H
#define ERROR_H

#include <avr/io.h>

/*
 * Signals an error by setting PB0 to 1
 * and loops forever.
 */
void error() {
    DDRB |= 0x01;
    PORTB |= 0x01;
    while (1);
}

#endif
