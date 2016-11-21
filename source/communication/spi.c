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

#define ACK 0x7E

void spi_init() {
    // Set MISO output, all others input
    DDRB &= 0x0F;
    DDRB |= (1 << DDB6);
	// Enable SPI and interrupt enable bit
	SPCR = (1<<SPE) | (1<<SPIE);
}

uint8_t spi_recieve_byte() {

	// Wait for reception complete 
	while(!(SPSR) & (1<<SPIF));

	// Return Data Register
	return SPDR;
}

uint8_t spi_transmit_byte(uint8_t data) {
	SPDR = data;
	// Wait for reception complete
	while(!(SPSR) & (1<<SPIF)){}

	// Return Data Register
	return SPDR;
}

void transmit_ack() {
    spi_transmit_byte(ACK);
}

