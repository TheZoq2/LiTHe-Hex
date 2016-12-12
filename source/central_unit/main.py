#!/usr/bin/env python3

# Copyright 2016 Noak Ringman, Emil Segerbäck, Robin Sliwa, Frans Skarman, Hannes Tuhkala, Malcolm Wigren, Olav Övrebö

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

import communication.avr_communication as avr_communication
import communication.web as web
import sys
import queue
import time
import decisions.decision_making as decision_making
import decisions.pid_controller as pid_controller
import pdb
import math
import os
import constants
try:
    import RPi.GPIO as GPIO
except ImportError:
    pass


AUTO_BUTTON_PIN = 37 

def main():

    test_mode = True

    if len(sys.argv) > 0 and sys.argv[0] == "--test":
        test_mode = True

    motor_spi = avr_communication.motor_communication_init()
    sensor_spi = avr_communication.sensor_communication_init()
    res = []

    # Setup auto/manual mode and button for it
    auto = False
    # button_temp = 0
    # GPIO.setmode(GPIO.BOARD)
    # GPIO.setup(AUTO_BUTTON_PIN, GPIO.IN, GPIO.PUD_DOWN)

    decision_packet = decision_making.DecisionPacket()

    send_queue = queue.Queue()
    receive_queue = queue.Queue()
    thread = web.CommunicationThread(send_queue, receive_queue)

    prev_x = prev_y = prev_rot = prev_speed = 0

    thread.start()

    while True:
        #pdb.set_trace()
        # Button toggle auto/manual mode and send mode to server
        # button_input = GPIO.input(AUTO_BUTTON_PIN)
        # if (button_input == 1):
        #     if (button_temp != button_input):
        #         auto = not auto
        #         button_temp = 1
        #         send_queue.put(web.ServerSendPacket(auto_mode=auto))
        # else:
        #     button_temp = 0

        if auto:
            # Auto mode
            os.system('clear')
            print("Auto mode!")
            auto, prev_speed, prev_x, prev_y, prev_rot = do_auto_mode_iteration(
                sensor_spi, motor_spi, send_queue, 
                receive_queue, decision_packet,
                prev_speed, prev_x, prev_y, prev_rot);
            time.sleep(0.5)

        else:
            # Manual mode
            # os.system('clear')
            # print("Entering manual mode!")
            auto, prev_speed, prev_x, prev_y, prev_rot = do_manual_mode_iteration(
                sensor_spi, motor_spi, send_queue, receive_queue, 
                prev_speed, prev_x, prev_y, prev_rot)
            time.sleep(0.1)


def receive_server_packet(receive_queue):
    packet = None 
    while not receive_queue.empty():
        packet = receive_queue.get()
    return packet


def do_auto_mode_iteration(sensor_spi, motor_spi, send_queue,
                           receive_queue, decision_packet,
                           prev_speed, prev_x, prev_y, prev_rot):
    try:
        sensor_data = avr_communication.get_sensor_data(sensor_spi)
        print(" YAY: {} ".format(sensor_data))
        decision_making.get_decision(sensor_data, decision_packet)

        print("Decision: ", decision_packet.decision)

        pid_controller.regulate(sensor_data, decision_packet)
       # print("Pid controller command: ", decision_packet.regulate_base_movement,
       #       ", ", decision_packet.regulate_command_y, ", ", decision_packet.regulate_goal_angle)
        send_decision_avr(motor_spi, decision_packet)

        # Send decision to server
        print("Sending sensor data to server: " + str(sensor_data))
        send_queue.put(web.ServerSendPacket(sensor_data_packet=sensor_data))
        send_queue.put(web.ServerSendPacket(debug_string=
            decision_making.int_to_string_command(decision_packet.decision)))

        auto = True

        packet = receive_server_packet(receive_queue)

        if packet is not None:
            if packet.auto is not None:
                auto = packet.auto
            # Regulate algorithm parameters
            if packet.angle_scaledown is not None:
                decision_packet.regulate_angle_scaledown = packet.angle_scaledown
            if packet.movement_scaledown is not None:
                decision_packet.regulate_set_movement_scaledown = packet.movement_scaledown
            if packet.angle_adjustment_border is not None:
                decision_packet.regulate_angle_adjustment_border = packet.angle_adjustment_border
        return auto, prev_speed, prev_x, prev_y, prev_rot

    except Exception as e:
        print(e)
        print("Could not read sensor data. Skipping...")
        return True, prev_speed, prev_x, prev_y, prev_rot
   # print("sensor_data:", sensor_data)

   # print("Right_angle: ",sensor_data.right_angle)
   # print("Left_angle: ",sensor_data.left_angle)
   # print("Average angle: ", sensor_data.average_angle)




def do_manual_mode_iteration(sensor_spi, motor_spi, send_queue, receive_queue,
                             prev_speed, prev_x, prev_y, prev_rot):
    try:
        sensor_data = avr_communication.get_sensor_data(sensor_spi)
        send_queue.put(web.ServerSendPacket(sensor_data))
    except avr_communication.CommunicationError as e:
        print("Could not read sensor data: " + str(e))

    auto = False

    packet = receive_server_packet(receive_queue)

    if packet is not None:
        
        if packet.auto is not None:
            auto = packet.auto
        if packet.has_motion_command():
            print(packet.raw)
            servo_speed = (int)(packet.thrust * constants.MAX_16BIT_SIZE)
            
            return_to_neutral = packet.return_to_neutral

            if return_to_neutral is not None and return_to_neutral:
                avr_communication.back_to_neutral(motor_spi)

            if prev_speed != servo_speed:
                avr_communication.set_servo_speed(motor_spi, servo_speed, 100)

            x_speed = convert_to_sendable_byte(packet.x)
            y_speed = convert_to_sendable_byte(packet.y)
            rotation = convert_to_sendable_byte(packet.rotation)

            if x_speed != prev_x or y_speed != prev_y or rotation != prev_rot:
                avr_communication.walk(motor_spi, x_speed, y_speed, rotation, False, 100)

            prev_x = x_speed
            prev_y = y_speed
            prev_rot = rotation
            prev_speed = servo_speed

    return auto, prev_speed, prev_x, prev_y, prev_rot


def send_decision_avr(spi, decision_packet):

    x_speed = convert_to_sendable_byte(0)
    y_speed = convert_to_sendable_byte(0)
    rotation = convert_to_sendable_byte(0)

    if decision_packet.decision == decision_making.GO_FORWARD:
        x_speed = convert_to_sendable_byte(1)
        y_speed = convert_to_sendable_byte(0)
        rotation = convert_to_sendable_byte(0)

    elif decision_packet.decision == decision_making.TURN_LEFT:
        x_speed = convert_to_sendable_byte(0)
        y_speed = convert_to_sendable_byte(0)
        rotation = convert_to_sendable_byte(1)

    elif decision_packet.decision == decision_making.TURN_RIGHT:
        x_speed = convert_to_sendable_byte(0)
        y_speed = convert_to_sendable_byte(0)
        rotation = convert_to_sendable_byte(-1)

    elif decision_packet.decision == decision_making.STOP:
        x_speed = convert_to_sendable_byte(0)
        y_speed = convert_to_sendable_byte(0)
        rotation = convert_to_sendable_byte(0)

    count = 0
    avr_communication.set_servo_speed(spi, decision_packet.speed, timeout=100)
    avr_communication.walk(spi, x_speed, y_speed, rotation, auto_mode=True, timeout=100)

# Malcolm conversion for no no negative numbers, other name?
def convert_to_sendable_byte(byte):
    return (int)(((byte + 1) / 2) * constants.MAX_BYTE_SIZE)

if __name__ == '__main__':
    main()
