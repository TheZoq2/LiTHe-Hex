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
#include <avr/interrupt.h>
#include "timer.h"
#include "gyro.h"
#include "lidar.h"
#include "table.h"
#include "communication.h"

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

// If SPI receive something
ISR(SPI_STC_vect) {
    Frame frame_recv;
	on_spi_recv(&frame_recv);
	
	if(get_id(&frame_recv) == DATA_REQUEST) {
	
		Frame frame_trans;
		frame_trans.control_byte = SENSOR_DATA << 2;
	
		if(frame_recv.msg[0] == SENSOR_DATA) {
			get_sensor_data(&frame_trans);
			send_frame(&frame_trans);
		} else if (frame_recv.msg[0] == CORRIDOR_DATA) {
			get_wall_data(&frame_trans);
			send_frame(&frame_trans);
		}
	}
}

// Setup hardware ports on AVR
void port_init(){
	DDRD = 0x00;
	PORTD = 0x00;
	PORTD = 0x00;
	DDRD = (1 << DDD6);
}

int main(void) {
	
	port_init();
	
	Timer timer8bit;
	timer8 = &timer8bit;
	timer_init(timer8, BIT8);
	
	Timer timer16bit;
	timer16 = &timer16bit;
	timer_init(timer16, BIT16);

	spi_init();
	
	// Enable global interrupts
	sei();
	
	IR ir_list[NUM_SENSORS];
	ir_init(ir_list);

    adc_init();
	
	IRQueue ir_queue;
	ir_queue_init(&ir_queue, timer8);
	
	// Add all ir_sensor to ir_queue
	for(uint8_t i = 0; i < NUM_SENSORS; i++) {
		schedule(&ir_queue, ir_list[i].port);
	}

	Lidar lidar;
	lidar_init(&lidar, timer16);
	
	MainTable mainTableData;
	mainTable = &mainTableData;
	table_init(mainTable, ir_list);

	while(1) {
		
		// if first irport in queue has new value then start A/D conv. and save data 
		if(has_new_value(&ir_queue)) {
			irport_t port = dequeue(&ir_queue);
			ir_add_data(&ir_list[port], adc_read(port));
			ir_reduce_noise(&ir_list[port]);
			schedule(&ir_queue, port);
		}
	
       // Disable global interrupts for lidar
	   // to not interrupt reading of 16 bits
       //cli();
       lidar_measure(&lidar);
       //sei();

		update(mainTable, &lidar);
	}
}
