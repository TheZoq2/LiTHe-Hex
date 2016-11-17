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
#include <stdint.h>
#include <avr/io.h>

#define SENSOR_ADDR_READ     0xC5
#define SENSOR_ADDR_WRITE    0xC4
#define LIDAR_DATA_REG       0x8F

void i2c_init(void);
void i2c_send_start(void);
void i2c_send_stop(void);
void i2c_write_byte(uint8_t reg, uint8_t data);
void i2c_wait(void);
uint16_t i2c_read_data(void);
double lidar_value_to_meters(uint16_t centimeters);

void lidar_add_value(Lidar* lidar, uint16_t centimeters);

void lidar_init(Lidar* lidar) {
    
    i2c_init();

    lidar->value = 0.0;

}

void lidar_measure(Lidar* lidar) {
	
	//i2c_send_start();
	
    i2c_write_byte(0x00, 0x04);
    
    i2c_wait();
    
    lidar_add_value(lidar, i2c_read_data());
}

void i2c_init(void) {
	
	// set pull-up res.
	//DDRC &= 0xFC;
	PORTC = (1 << PC0)|(1 << PC1);

    // reset I2C status register
    TWSR = 0x00;

    // set I2C bitrate to 400kHz
    TWBR = 0x0C;

    // enable I2C
    TWCR = (1 << TWEN);

}

void i2c_send_start(void) {
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

void i2c_send_stop(void) {
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void i2c_write_byte(uint8_t reg, uint8_t data) {
    i2c_send_start();
    
    // send sensor address
    TWDR = SENSOR_ADDR_WRITE;
    TWCR = (1 << TWINT)|(1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);

    // send register address
    TWDR = reg;
    TWCR = (1 << TWINT)|(1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);

    // send data 
    TWDR = data;
    TWCR = (1 << TWINT)|(1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);

    i2c_send_stop();
}

uint16_t i2c_read_data(void) {
    i2c_send_start();

    // send sensor address
    TWDR = SENSOR_ADDR_WRITE;
    TWCR = (1 << TWINT)|(1 << TWEN);
    while ((TWCR & (1 << TWINT)) == 0);
	
	// send register address
	TWDR = LIDAR_DATA_REG;
	TWCR = (1 << TWINT)|(1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);

	i2c_send_stop();

	i2c_send_start();

	// send sensor address, read
	TWDR = SENSOR_ADDR_READ;
	TWCR = (1 << TWINT)|(1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
	
	TWCR = (1 << TWINT)|(1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
	uint8_t least_sig = TWDR;
	
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWEA);
	while ((TWCR & (1 << TWINT)) == 0);
	uint8_t most_sig = TWDR;

	i2c_send_stop();
	
	return (most_sig << 8) & least_sig;
}

void i2c_wait() {
    uint8_t status = 1;

    while (status != 0) {
        i2c_send_start();

        // send sensor address
        TWDR = SENSOR_ADDR_WRITE;
        TWCR = (1 << TWINT)|(1 << TWEN);
        while ((TWCR & (1 << TWINT)) == 0);

        // send status register address
        TWDR = 0;
        TWCR = (1 << TWINT)|(1 << TWEN);
        while ((TWCR & (1 << TWINT)) == 0);

        i2c_send_stop();

        i2c_send_start();

        // send sensor address, read
        TWDR = SENSOR_ADDR_READ;
        TWCR = (1 << TWINT)|(1 << TWEN);
        while ((TWCR & (1 << TWINT)) == 0);
        
        // read status register
        TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWEA);
        while ((TWCR & (1 << TWINT)) == 0);
        
        status = (0x01 & TWDR);

        i2c_send_stop();
        
    }

}

void lidar_add_value(Lidar* lidar, uint16_t centimeters) {
	
	for(uint8_t i = 0; i < NUM_LIDAR_DATA-1; i++) {
		lidar->raw_data_list[i] = lidar->raw_data_list[i+1];
	}
	lidar->raw_data_list[NUM_LIDAR_DATA-1] = lidar_value_to_meters(centimeters);
}

double lidar_value_to_meters(uint16_t centimeters) {
	
	return (centimeters / 100);
}