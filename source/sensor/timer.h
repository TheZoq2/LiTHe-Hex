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

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

enum Resolution {BIT8, BIT16};

#define TIMER8_SCALER_MILLIS		0.1216
#define TIMER16_SCALER_MILLIS		0.00011875
#define TIMER8_SCALER_MICROS		121.6
#define TIMER16_SCALER_MICROS		0.11875
#define MAX_8BIT_VALUE				255
#define MAX_16BIT_VALUE				65535
#define TIMER8_PRESCALER			0x05
#define TIMER16_PRESCALER			0x01

typedef struct Timer {

    enum Resolution resolution;

    uint32_t num_overflows;

} Timer;

void timer_init(Timer* timer, enum Resolution res);
void timer_reset(Timer* timer);

uint32_t timer_value_millis(Timer* timer);
uint32_t timer_value_micros(Timer* timer);

#endif
