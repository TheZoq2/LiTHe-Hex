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

#ifndef SPI_H
#define SPI_H

#ifndef IS_X86
#include <avr/io.h>
#include <avr/interrupt.h>
#endif
#include <stdbool.h>
#include <stdint.h>

void spi_init();
uint8_t spi_receive_byte();
uint8_t spi_transmit_byte(uint8_t data);
void spi_transmit_ack();
void spi_transmit_fail();
void spi_set_interrupts(bool value);

#endif /* ifndef SPI_H */
