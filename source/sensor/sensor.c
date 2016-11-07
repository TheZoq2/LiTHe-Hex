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



int main(void)
{
	DDRB = 0xFF;
    IRCONTROL control;
    
    ir_init(&control);

    adc_init();

	uint32_t count = 0;
	
	uint16_t res;    
    
	while(1) {
		count++;
		
		if (count % 10000 == 0) {
			adc_start_conversion(5);
			while (ADCSRA & (1<<ADSC)) {}
			res = adc_read_result();
			
			PORTB = (uint8_t)((unsigned int)res >> 2);
		}
		
	}
}
