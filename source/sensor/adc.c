#include "adc.h"

void adc_init(ADCONVERTER* adc) {
    
    // initialize the mux
    ADMUX = (1<<REFS0);

    // initialize the adc control and status register
    // with prescaler 128
    ADCSRA = (1<<ADEN)|(1<<ASPS2)|(1<<ASPS1)<(1<<ASPS0);

}

void adc_start_conversion(ADCONVERTER* adc) {
    
}

