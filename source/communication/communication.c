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

#include "spi.h"
#include <stdbool.h>
#ifdef MainTable
#endif

typedef struct Frame {
	char control_byte;
	char len;
	char msg[];
} Frame;

char replay_msg_id[5] = {0x02, 0x03, 0x04, 0x20, 0x05};

bool check_partiy();

void send_replay_sensor(char current_msg);

void send_replay_motor(char current_msg);

bool message_require_replay(char current_msg);

char get_current_id(Frame* frame);

void get_new_frame(Frame* frame);

void send_frame(Frame* frame);

void on_spi_recv() {
	Frame frame;
	get_new_frame(&frame);
	char current_id = get_current_id(&frame);
	bool success = check_partiy(&frame);
	if(success) {
		spi_transmit_ack();
		
		bool replay = message_require_replay(current_id);
		if(replay) {
			if(current_id = 0x02) {
			/*	spi_transmit_byte(0x20);
				spi_transmit_byte(0x01);
				spi_transmit_byte(0xCC);*/
			}
			#ifdef MainTable
				send_replay_sensor(current_id);
			#endif
			//#ifdef //TODO send replay for motor if some file is def
				//send_replay_motor();
		} else {
			#ifdef //TODO send replay for motor if some file is def
			//control_motor();
		}
	} else { // Something is wrong with message
		spi_transmit_byte(0x1F);
	}
}

bool check_partiy(Frame* frame) {
	return true;
}

void get_new_frame(Frame* frame) {
	frame->control_byte = spi_recieve_byte();
	if(frame->control_byte & 0x80) { // msg is one byte long
		
	} else { // msg is more than one byte long
		frame->len = spi_recieve_byte();
		frame->msg[frame->len];
		for(uint8_t i = 0; i < frame->len; i++) {
			frame->msg[i] = spi_recieve_byte();
		}
	}
}

char get_current_id(Frame* frame) {
	return frame->control_byte;
}

bool message_require_replay(char current_msg) {
	for(uint8_t i = 0; i < 7; i++) {
		if(current_msg == replay_msg_id[i]) {
			return true;
		}
	}
}

void send_replay_sensor(char current_id) {
	Frame frame;
	if(current_id == 0x02) {
		spi_transmit_byte(0x20);
		send_sensor_data();
		spi_transmit_byte(0x21);
		send_sensor_wall_data();
	}
}

void send_frame(Frame* frame) {
	
}

void control_motor(char current_msg) {
	switch(current_msg){
		case 0x03 :
			// Toggle hindergong
			break;
		case 0x04 :
			// Set speed
			break;
		case 0x20 :
			// Go command 
			break;
		case 0x05 :
			// Return to neutral
			break;
	}
}

void send_replay_motor(char current_msg) {
	spi_transmit_byte(0x20);
	// Send servo status
	spi_transmit_byte(0x21);
	// Send debug string
	spi_transmit_byte(0x03);
	// Hinder here?
}