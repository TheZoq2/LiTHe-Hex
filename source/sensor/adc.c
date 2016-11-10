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

#include "adc.h"
#include "error.h"

void adc_init() {
    
    // initialize the mux
    ADMUX = (1<<REFS0);

    // initialize the adc control and status register
    // with prescaler 128
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)<(1<<ADPS0);

}

void adc_read(uint8_t channel) {

    if (channel > 7 || channel < 0) error();

    // set the multiplexer for this channel
    ADMUX = (ADMUX & 0xF8) | channel;

    // start a conversion
    ADCSRA |= (1<<ADSC);

    // wait for conversion to complete
    while (ADCSRA & (1<<ADSC)); 

    return (ADC);
}

