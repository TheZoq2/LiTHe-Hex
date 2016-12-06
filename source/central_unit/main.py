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

AUTO_BUTTON_PIN = 40

def main():

    test_mode = False

    if len(sys.argv) > 0 and sys.argv[0] == "--test":
        test_mode = True

    spi = avr_communication.communication_init()
    res = []

    # Setup auto/manual mode and button for it
    auto = False
    button_temp = 0
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(AUTO_BUTTON_PIN, GPIO.IN, GPIO.PUD_DOWN)

    decision_packet = decision_making.DecisionPacket()

    send_queue = queue.Queue()
    receive_queue = queue.Queue()
    thread = web.CommunicationThread(send_queue, receive_queue)

    thread.start()

    while True:
        #pdb.set_trace()
        # Button toggle auto/manual mode and send mode to server
        button_input = GPIO.input(AUTO_BUTTON_PIN)
        print(button_input)
        if (button_input == 1):
            if (button_temp != button_input):
                auto = not auto
                button_temp = 1
                send_queue.put(web.ServerSendPacket(auto_mode=auto))
        else:
            button_temp = 0

        if auto:
            # Auto mode
            os.system('clear')
            print("Auto mode!")
            auto = do_auto_mode_iteration(spi, send_queue, receive_queue, decision_packet);
            time.sleep(0.5)

        else:
            # Manual mode
            # os.system('clear')
            # print("Entering manual mode!")
            auto = do_manual_mode_iteration(spi, send_queue, receive_queue)
            time.sleep(0.1)


def do_auto_mode_iteration(spi, send_queue, receive_queue, decision_packet):
    sensor_data = avr_communication.get_sensor_data(spi)
    print("sensor_data:", sensor_data)

    print("Right_angle: ",sensor_data.right_angle)
    print("Left_angle: ",sensor_data.left_angle)
    print("Average angle: ", sensor_data.average_angle)

    decision_making.get_decision(sensor_data, decision_packet)

    print("Decision: ", decision_packet.decision)

    pid_controller.regulate(sensor_data, decision_packet)
    print("Pid controller command: ", decision_packet.regulate_base_movement,
          ", ", decision_packet.regulate_command_y, ", ", decision_packet.regulate_goal_angle)
    #send_decision_avr(spi, decision_packet)

    # Send decision to server
    send_queue.put(web.ServerSendPacket(debug_string=
        decision_making.int_to_string_command(decision_packet.decision),
                                        sensor_data_packet=sensor_data))

    auto = True

    if not receive_queue.empty():
        packet = receive_queue.get()
        if packet.auto is not None:
            auto = packet.auto
        # Regulate algorithm parameters
        if packet.angle_scaledown is not None:
            decision_packet.regulate_angle_scaledown = packet.angle_scaledown
        if packet.movement_scaledown is not None:
            decision_packet.regulate_set_movement_scaledown = packet.movement_scaledown
        if packet.angle_adjustment_border is not None:
            decision_packet.regulate_angle_adjustment_border = packet.angle_adjustment_border

    return auto


def do_manual_mode_iteration(spi, send_queue, receive_queue):
    sensor_data = avr_communication.get_sensor_data(spi)

    send_queue.put(web.ServerSendPacket(sensor_data))

    auto = False

    if not receive_queue.empty():
        packet = receive_queue.get()
        if packet.auto is not None:
            auto = packet.auto
        if packet.has_motion_command():
            servo_speed = (int)(packet.thrust * constants.MAX_16BIT_SIZE)
            avr_communication.set_servo_speed(spi, servo_speed)

            x_speed = convert_to_sendable_byte(packet.x)
            y_speed = convert_to_sendable_byte(packet.y)
            rotation = convert_to_sendable_byte(packet.rotation)

            avr_communication.walk(spi, x_speed, y_speed, rotation, False)

    return auto


def send_decision_avr(spi, decision_packet):

    x_speed = convert_to_sendable_byte(0)
    y_speed = convert_to_sendable_byte(0)
    rotation = convert_to_sendable_byte(0)

    avr_communication.set_servo_speed(spi, decision_packet.speed)

    # TODO set the x_speed, y_speed, rotaton for each decision
    if decision_packet.decision == GO_FORWARD:
        x_speed = convert_to_sendable_byte(1)
        y_speed = convert_to_sendable_byte(0)
        rotation = convert_to_sendable_byte(0)

    elif decision_packet.decision == TURN_LEFT:
        x_speed = convert_to_sendable_byte(0)
        y_speed = convert_to_sendable_byte(0)
        rotation = convert_to_sendable_byte(1)

    elif decision_packet.decision == TURN_RIGHT:
        x_speed = convert_to_sendable_byte(0)
        y_speed = convert_to_sendable_byte(0)
        rotation = convert_to_sendable_byte(-1)

    elif decision_packet.decision == STOP:
        x_speed = convert_to_sendable_byte(0)
        y_speed = convert_to_sendable_byte(0)
        rotation = convert_to_sendable_byte(0)

    avr_communication.walk(spi, x_speed, y_speed, rotation, auto_mode=True)

# Malcolm conversion for no no negative numbers, other name?
def convert_to_sendable_byte(byte):
    return (int)(((byte + 1) / 2) * constants.MAX_BYTE_SIZE)

if __name__ == '__main__':
    main()
