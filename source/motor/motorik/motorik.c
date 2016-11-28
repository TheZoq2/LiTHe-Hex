//#define F_CPU 16000000UL

#include "avr_helper.h"

#ifndef IS_X86
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#endif

#include "macros.h"

#include "uart_lib.h"
#include "servo.h"

#include <stdint.h>
#include <stdlib.h>

#include "../../communication/communication.h"
#include "../../communication/communication.h"

void build_spi_reply_frame(Frame *frame_trans);
void handle_spi_frame(Frame *frame_recv);

// If SPI receive something
ISR(SPI_STC_vect) {

    Frame frame_recv;
	on_spi_recv(&frame_recv);

	uint8_t current_msg = get_id(&frame_recv);

	if(current_msg == DATA_REQUEST) {

		Frame frame_trans;
		build_spi_reply_frame(&frame_trans);
		send_frame(&frame_trans);
	} else {

		handle_spi_frame(&frame_recv);
	}
}

int main(void)
{
	// Enable global interrupts and init spi communication
	spi_init();
	sei();

	set_ddr(DDRD, 0xfE);
	//
	usart_init();
	
	_delay_ms(100);
	
	
	uint8_t sevo_id = 13;
	
	init_all_servos();

	send_servo_action();
	
	uint16_t angles[3] = {0x1ff, 0x1ff, 0x1ff};
	for(uint8_t i = 0; i < 6; ++i)
	{
		set_leg_angles(i, angles);
	}
	
	send_servo_action();

	_delay_ms(500);
		
	uint16_t angles_1[3] = {0x0ff, 0x1ff, 0x1ff};
	while(1)
	{
		
	}
}

void build_spi_reply_frame(Frame *frame_trans) {

	switch(frame_trans->msg[0]) {
		case SERVO_STATUS :
			// Send servo status data
			frame_trans->control_byte = SERVO_STATUS << 2;
			frame_trans->len = 0x05; // SET LEN OF MSG
			frame_trans->msg[0] = 0x00; // SET MSG
			break;
		case DEBUG_STRING :
			// Send servo status data
			frame_trans->control_byte = DEBUG_STRING << 2;
			frame_trans->len = 0x05; // SET LEN OF MSG
			frame_trans->msg[0] = 0x00; // SET MSG
			break;
		case OBSTACLE :
			// Send servo status data
			frame_trans->control_byte = OBSTACLE << 2;
			frame_trans->len = 0x00; 
			frame_trans->msg[0] = 0x00; // SET MSG
			break;
	}
}

void handle_spi_frame(Frame *frame_recv) {

	switch(get_id(frame_recv)){
		case TOGGLE_OBSTACLE :
			// Toggle obstacle
			//uint8_t on_off = frame_recv->msg[0];
			break;
		case SET_SERVO_SPEED :
			// Set speed
			//uint16_t servo_speed = ((uint16_t) frame_recv->msg[1] << 8) | (uint16_t) frame_recv->msg[0];
			break;
		case WALK_COMMAMD :
			// Go command 
			//uint8_t len = frame_recv->msg[0];
			//uint8_t x_speed = frame_recv->msg[1];
			//uint8_t y_speed = frame_recv->msg[2];
			//uint8_t turn_speed = frame_recv->msg[3];
			break;
		case RETURN_TO_NEUTRAL :
			// Return to neutral
			break;
	}
}
