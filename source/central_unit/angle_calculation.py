# Copyright 2016 Noak Ringman, Emil Segerbäck, Robin Sliwa, Frans Skarman, Hannes Tuhkala, Malcolm Wigren, Olav Övrebö
#

# This file is part of LiTHe Hex.

# LiTHe Hex is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# LiTHe Hex is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with LiTHe Hex.  If not, see <http://www.gnu.org/licenses/>.

import math
import constants

# All these functions return angles in degrees
def get_right_angle(sensor_data):
    return math.atan((sensor_data.ir_front_right - sensor_data.ir_back_right) / 
                     constants.DISTANCE_BETWEEN_SENSORS) * (180/math.pi)


def get_left_angle(sensor_data):
    return math.atan((sensor_data.ir_front_left - sensor_data.ir_back_left) / 
                     constants.DISTANCE_BETWEEN_SENSORS) * (180/math.pi)


def get_average_angle(sensor_data):
    return ((get_left_angle(sensor_data) + get_right_angle(sensor_data))/2)
