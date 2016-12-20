
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


import sys
import math
import angle_calculation
import constants
import copy



input_file = "/tmp/hexsim/sensors"
output_file = "/tmp/hexsim/command"

CORRIDOR_WIDTH = 0.8
SENSOR_OFFSET = 0.1
COMMAND_Y_THRESHOLD = 0.04
ANGLE_DIFF_THRESHOLD = 30
BASE_MOVEMENT = "0.1" #placeholder forward movement.


def _to_radians(deg):
    return (math.pi / 180) * deg


def _avg(*args):
    return sum(args) / len(args)


def _change_sensor_values(temp_sensor_data):
    if (temp_sensor_data.ir_front_left >= 0.6 or 
        temp_sensor_data.ir_back_left >= 0.6):
        temp_sensor_data.ir_front_left = 0
        temp_sensor_data.ir_back_left = 0
        
    if (temp_sensor_data.ir_front_right >= 0.6 or
        temp_sensor_data.ir_back_right >= 0.6):
        temp_sensor_data.ir_front_right = 0
        temp_sensor_data.ir_back_right = 0


def regulate(sensor_data, decision_packet):

    offset = 0

    temp_sensor_data = copy.deepcopy(sensor_data)
    _change_sensor_values(temp_sensor_data)
    
    temp_sensor_data.left_angle = angle_calculation.get_left_angle(temp_sensor_data)
    temp_sensor_data.right_angle = angle_calculation.get_right_angle(temp_sensor_data)
    temp_sensor_data.average_angle = angle_calculation.get_average_angle(temp_sensor_data)

    avg_left = _avg(temp_sensor_data.ir_front_left, temp_sensor_data.ir_back_left)
    avg_right = _avg(temp_sensor_data.ir_front_right, temp_sensor_data.ir_back_right)
    angle = temp_sensor_data.average_angle

    print("average left: ", avg_left)
    print("average right: ", avg_right)
    
    
    if (avg_right == 0 and avg_left == 0):
        offset = 0

    elif (avg_left == 0):
        offset = avg_right + constants.SENSOR_X_DIST/2 - CORRIDOR_WIDTH/2
    
    elif (avg_right == 0):
        offset = -(avg_left + constants.SENSOR_X_DIST/2 - CORRIDOR_WIDTH/2)
    
    elif (abs(temp_sensor_data.left_angle + temp_sensor_data.right_angle) > ANGLE_DIFF_THRESHOLD) :
        angle = 0
        #offset = 0
    
    else:
        offset = ((CORRIDOR_WIDTH/2 - avg_left) + (avg_right - CORRIDOR_WIDTH/2))/2

    print("offset before: ", offset)
    
    if (offset <= COMMAND_Y_THRESHOLD and offset >= -COMMAND_Y_THRESHOLD):
        offset = 0
    
    decision_packet.regulate_command_y = -offset * decision_packet.regulate_movement_scaledown
    print("Regulate movement scaledown: ", decision_packet.regulate_movement_scaledown)
            
    decision_packet.regulate_command_y = min(max(decision_packet.regulate_command_y, -1), 1)

    print("Regulate adjustment border: ", decision_packet.regulate_angular_adjustment_border)

    if (angle <= decision_packet.regulate_angular_adjustment_border and angle >= -decision_packet.regulate_angular_adjustment_border):
        angle = 0
    
    print("angle: ", angle)
    print("offset: ", offset)

    print("command_y: ", decision_packet.regulate_command_y)
    decision_packet.regulate_goal_angle = _to_radians(angle) * decision_packet.regulate_angle_scaledown
    #Cap -1 < regulate_goal_angle < 1
    decision_packet.regulate_goal_angle = min(max(decision_packet.regulate_goal_angle, -1), 1)
    print("goal_angle: ", decision_packet.regulate_goal_angle)


