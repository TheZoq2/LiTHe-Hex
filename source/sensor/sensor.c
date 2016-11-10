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

#include "ir.h"
#include "adc.h"
#include "ir_queue.h"
#include "avr/interrupt.h"
#include "timer.h"

Timer* timer8;
Timer* timer16;

// When TIMER0 overflow increase timer8 overflow counter;
ISR(TIMER0_OVF_vect) {
	timer8->num_overflows++;
}

// When TIMER1 overflow increase timer16 overflow counter;
ISR(TIMER1_OVF_vect) {
	timer16->num_overflows++;
}

int main(void) {
	
	DDRD = 0xFF;
	
	Timer timer8bit;
	timer8 = &timer8bit;
	timer_init(timer8, BIT8);
	
	Timer timer16bit;
	timer16 = &timer16bit;
	timer_init(timer16, BIT16);
	
	// Enable global interrupts
	sei();
	
	IR ir_list[NUM_SENSORS];
	
	ir_init(ir_list);

    adc_init();
	
	IRQueue ir_queue;
	
	ir_queue_init(&ir_queue);

	uint32_t count = 0;
	
	uint16_t res1;
	uint16_t res2;    

	uint8_t dir = 0;
	
	uint32_t time = timer_value_millis(timer16);
	PORTD = 0x00;
		
	while (timer_value_millis(timer16) < 5000) {}
	PORTD = 0x0F;
	
	while (timer_value_millis(timer16) < 65000) {}
	PORTD = 0xFF;
	    
	while(1) {
		/*
		// if first irport in queue has new value then start A/D conv. and save data 
		if(has_new_value(&ir_queue)) {
			irport_t port = dequeue(&ir_queue);
			ir_add_data(&ir_list[port], adc_read(port));
			res1 = ir_list[port].raw_data_list[NUM_SENSORS-1];
			schedule(&ir_queue, port);
		}
			
		
		PORTB = (uint8_t)((unsigned int)res1 >> 2);
		//PORTD = (uint8_t)((unsigned int)res2 >> 2);
		*/
		

	}
}
