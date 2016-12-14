import sys
import math

input_file = "/tmp/hexsim/sensors"
output_file = "/tmp/hexsim/command"

CORRIDOR_WIDTH = 0.8
SENSOR_OFFSET = 0.1
ANGLE_SCALEDOWN = 0.5
MOVEMENT_SCALEDOWN = 0.1
ANGULAR_ADJUSTMENT_BORDER = 0.1
BASE_MOVEMENT = "0.1" #placeholder forward movement.


def write_output_command(command):
    """
    Writes the command to the output file
    """
    with open(output_file, 'w') as txt:
        txt.write(command)


def _to_radians(deg):
    return (math.pi / 180) * deg


def regulate(sensor_data, decision_packet):

    # offset for the robot length from mid in a corridor
    offset = (
            CORRIDOR_WIDTH * (sensor_data.ir_front_left + SENSOR_OFFSET) /
            (
                sensor_data.ir_front_right +
                (2 * SENSOR_OFFSET)
                + sensor_data.ir_front_left
            )
            ) - (CORRIDOR_WIDTH/2)

    decision_packet.command_y = (sensor_data.ir_front_right - sensor_data.ir_front_left) * decision_packet.regulate_movement_scaledown
    # TODO: fix a check for if one angle is way off. If it happens do not use it
    # TODO: fix so that we do not use positive angle all the time

    if abs(offset) > decision_packet.regulate_angular_adjustment_border:
        decision_packet.regulate_goal_angle = -_to_radians(sensor_data.average_angle) - math.pi * offset * decision_packet.regulate_angle_scaledown
    else:
        decision_packet.regulate_goal_angle = -_to_radians(sensor_data.average_angle)
        decision_packet.regulate_base_movement;

    #Cap -1 < regulate_goal_angle < 1
    if decision_packet.regulate_goal_angle > 1:
        decision_packet.regulate_goal_angle = 1
    elif decision_packet.regulate_goal_angle < -1:
        decision_packet.regulate_goal_angle = -1

if __name__ == "__main__":
    while True:
        
        SensorDataPacket(ir_down, ir_front_left, ir_back_left,
                     ir_front_right, ir_back_right, lidar_msd, lidar_lsd)
