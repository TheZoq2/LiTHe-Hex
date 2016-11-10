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

#include "timer.h"
#include <avr/io.h>

void timer_init(Timer* timer, enum Resolution res) {
    timer->resolution = res;
    timer->num_overflows = 0;

    if (res == BIT8) {

        // set prescaler
        TCCR0B |= TIMER8_PRESCALER;

        // reset the timer
        TCNT0 = 0;
        
        // enable overflow interrupt
        TIMSK0 |= (1 << TOIE0);

    } else {

        // set prescaler
        TCCR1B |= TIMER16_PRESCALER;

        // reset timer
        TCNT1 = 0;

        TIMSK1 |= (1 << TOIE1);
    }
}

uint32_t timer_value_millis(Timer* timer) {

    if (timer->resolution == BIT8) {
        
        return ((TCNT0 + (timer->num_overflows * MAX_8BIT_VALUE)) 
                / (CLOCK_FREQUENCY / TIMER8_PRESCALER_VALUE)) * 1000 * TIMER_SCALER;

    } else {

        return ((TCNT1 + (timer->num_overflows * MAX_16BIT_VALUE)) 
                / (CLOCK_FREQUENCY / TIMER16_PRESCALER_VALUE)) * 1000 * TIMER_SCALER;

    }
}

uint32_t timer_value_micros(Timer* timer) {
    return timer_value_millis(timer) * 1000;
}

