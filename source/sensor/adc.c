#include "adc.h"

void adc_init() {
    
    // initialize the mux
    ADMUX = (1<<REFS0);

    // initialize the adc control and status register
    // with prescaler 128
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)<(1<<ADPS0);

}

void adc_read(uint8_t channel) {

    // make sure channel is 0-7
    channel &= 7;

    // set the multiplexer for this channel
    ADMUX = (ADMUX & 0xF8) | channel;

    // start a conversion
    ADCSRA |= (1<<ADSC);

    // wait for conversion to complete
    while (ADCSRA & (1<<ADSC)); 

    return (ADC);
}

