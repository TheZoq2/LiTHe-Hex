
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


input_file = "/tmp/hexsim/sensors"
output_file = "/tmp/hexsim/command"

CORRIDOR_WIDTH = 0.8
SENSOR_OFFSET = 0.1
ANGLE_SCALEDOWN = 0.5
MOVEMENT_SCALEDOWN = 0.1
ANGULAR_ADJUSTMENT_BORDER = 0.1
BASE_MOVEMENT = "0.1" #placeholder forward movement.


# def write_output_command(command):
#     """
#     Writes the command to the output file
#     """
#     with open(output_file, 'w') as txt:
#         txt.write(command)


def _to_radians(deg):
    return (math.pi / 180) * deg


def _avg(*args):
    return sum(args) / len(args)


def regulate(sensor_data, decision_packet):

    # offset for the robot length from mid in a corridor
    # offset = (
    #         CORRIDOR_WIDTH * (sensor_data.ir_front_left + SENSOR_OFFSET) /
    #         (
    #             sensor_data.ir_front_right +
    #             (2 * SENSOR_OFFSET)
    #             + sensor_data.ir_front_left
    #         )
    #         ) - (CORRIDOR_WIDTH/2)

    avg_left = _avg(sensor_data.ir_front_left, sensor_data.ir_back_left)
    avg_right = _avg(sensor_data.ir_front_right, sensor_data.ir_back_right)
    angle = _to_radians(sensor_data.average_angle)
    dist_to_left_wall = math.cos(angle) * (constants.SENSOR_Y_DIST / 2 + avg_left)
    dist_to_right_wall = math.cos(angle) * (constants.SENSOR_Y_DIST / 2 + avg_right)

    offset = dist_to_left_wall - dist_to_right_wall # negative to left, positive to right

    decision_packet.regulate_command_y = (dist_to_left_wall - dist_to_right_wall) * \
            decision_packet.regulate_movement_scaledown / CORRIDOR_WIDTH
    decision_packet.regulate_command_y = min(max(decision_packet.regulate_command_y, -1), 1)



    # if abs(offset) > decision_packet.regulate_angular_adjustment_border:
    #     decision_packet.regulate_goal_angle = -_to_radians(sensor_data.average_angle) - \
    #             math.pi * offset * decision_packet.regulate_angle_scaledown
    # else:
    #     decision_packet.regulate_goal_angle = -_to_radians(sensor_data.average_angle)
    #     decision_packet.regulate_base_movement;

    decision_packet.regulate_goal_angle = math.pow(-angle,3)
    #Cap -1 < regulate_goal_angle < 1
    decision_packet.regulate_goal_angle = min(max(decision_packet.regulate_goal_angle, -1), 1)
