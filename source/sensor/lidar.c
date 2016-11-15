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

#include "lidar.h"

#define SENSOR_ADDR     0x32

void i2c_init(void);
void i2c_send_start(void);
void i2c_send_stop(void);
void i2c_write_byte(uint8_t sens_addr, uint8_t reg, uint8_t data);
uint8_t i2c_read_byte(uint8_t sens_addr, uint8_t reg);

void lidar_add_value(Lidar* lidar, uint16_t centimeters);

void lidar_init(Lidar* lidar) {

    // reset I2C status register
    TWSR = 0x00;

    // set I2C bitrate to 400kHz
    TWBR = 0x0C;

    // enable I2C
    TWCR = (1 << TWEN);


}

void lidar_measure(Lidar* lidar) {
    i2c_write_byte(SENSOR_ADDR, 0x00, 0x04);
    
    while ((0xFE & i2c_read_byte(SENSOR_ADDR, 0x01)) != 0);

    uint16_t least_sig = (uint16_t) i2c_read_byte(SENSOR_ADDR, 0x10);
    uint16_t most_sig = (uint16_t) i2c_read_byte(SENSOR_ADDR, 0x0F);

    uint16_t centimeters = (most_sig << 8) & least_sig;
    
    lidar_add_value(lidar, centimeters);
}

void i2c_init(void) {

}

void i2c_send_start(void) {

}

void i2c_send_stop(void) {

}

void i2c_write_byte(uint8_t sens_addr, uint8_t reg, uint8_t data) {

}

uint8_t i2c_read_byte(uint8_t sens_addr, uint8_t reg) {

}

void lidar_add_value(Lidar* lidar, uint16_t centimeters) {

}

