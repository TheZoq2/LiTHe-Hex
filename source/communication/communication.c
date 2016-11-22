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

#include <stdbool.h>
#include "spi.h"
#include "communication.h"

uint8_t replay_msg_id[5] = {0x02, 0x03, 0x04, 0x20, 0x05};

bool check_partiy();

void send_replay_sensor(uint8_t current_msg);

void send_replay_motor(uint8_t current_msg);

bool message_require_replay(uint8_t current_msg);

uint8_t get_current_id(Frame* frame_recv);

void get_new_frame(Frame* frame_recv);

void send_frame(Frame* frame_send);

void on_spi_recv() {
	Frame frame_recv;
	get_new_frame(&frame_recv);
	uint8_t current_id = get_current_id(&frame_recv);
	
	bool success = check_partiy(&frame_recv); 
	if(success) { // continue if message ok
		spi_transmit_ack();
		
		bool replay = message_require_replay(current_id);
		if(replay) { // send a replay to central-unit
			if(current_id == 0x02) {
				Frame frame_send;
				frame_send.control_byte = 0x20;
				frame_send.len = 0x01;
				frame_send.msg = 0xCC;
				send_frame(&frame_send);
			}
			#ifdef MainTable
				send_replay_sensor(current_id);
			#endif
			//#ifdef //TODO send replay for motor if some file is def
				//send_replay_motor();
		} else { // 
			//#ifdef //TODO send replay for motor if some file is def
			//control_motor();
		}
	} else { // Something was wrong with message
		spi_transmit_byte(0x1F);
	}
}

bool check_partiy(Frame* frame) {
	// check parity for control_byte
	uint8_t byte = frame->control_byte
	bool parity_con = false;
	while(byte) {
		parity_con = !parity_con;
		byte &= (byte - 1); 
	}

	// check parity for length and message bytes
	bool parity_msg = false;
	for(uint8_t i = 0; i < frame->len; i++) {
		uint8_t msg = frame->msg[i];
		while(msg) {
			parity_con = !parity_con;
			msg &= (msg - 1);
		}
	}
	
	return parity_con && parity_msg;
}

void get_new_frame(Frame* frame_recv) {
	frame_recv->control_byte = spi_recieve_byte();
	if(frame_recv->control_byte & 0x80) { // msg is one byte long
		
	} else { // msg is more than one byte long
		frame_recv->len = spi_recieve_byte();
		frame_recv->msg[frame->len];
		for(uint8_t i = 0; i < frame_recv->len; i++) {
			frame_recv->msg[i] = spi_recieve_byte();
		}
	}
}

char get_current_id(Frame* fram_recve) {
	return frame_recv->control_byte;
}

bool message_require_replay(uint8_t current_msg) {
	for(uint8_t i = 0; i < 7; i++) {
		if(current_msg == replay_msg_id[i]) {
			return true;
		}
	}
}

void send_replay_sensor(uint8_t current_id) {
	Frame frame_send_1;
	Frame frame_send_2;
	if(current_id == 0x02) {
		frame_send_1.control_byte = 0x20;
		send_sensor_data(&frame_send_1);
		frame_send_2.control_byte = 0x21;
		send_sensor_wall_data(&frame_send_2);
	}
	frame_send(&frame_send_1);
	frame_send(&frame_send_2);
}

void send_frame(Frame* frame) {
	spi_transmit_byte(frame->control_byte);
	spi_transmit_byte(frame->len);
	for(uint8_t i = 0; i < frame->len; i++) {}
		spi_transmit_byte(frame->msg[i]);
	}
}

void control_motor(uint8_t current_msg) {
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

void send_replay_motor(uint8_t current_msg) {
	Frame frame_send_status;
	Frame frame_send_string;
	Frame frame_send_hinder;
	frame_send_status.control_byte = 0x20;
	// Send servo status
	frame_send_status.control_byte = 0x21;
	// Send debug string
	frame_send_status.control_byte = 0x03;
	// Hinder here?
	send_frame(frame_send_status);
	send_frame(frame_send_string);
	send_frame(frame_send_hinder);
}