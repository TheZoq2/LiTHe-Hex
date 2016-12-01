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

def main():
    
    test_mode = False

    if len(sys.argv) > 0 and sys.argv[0] == "--test":
        test_mode = True

    spi = avr_communication.communication_init()
    res = []

    auto = False

    decision_packet = decision_making.DecisionPacket()

    send_queue = queue.Queue()
    receive_queue = queue.Queue()
    thread = web.CommunicationThread(send_queue, receive_queue)

    thread.start()

    while True:
        #pdb.set_trace()
        if auto:

            os.system('clear')
            sensor_data = avr_communication.get_sensor_data(spi)
            print(sensor_data)

            print("right_angle: ",sensor_data.right_angle)
            print("left_angle: ",sensor_data.left_angle)
            print("Average angle: ", sensor_data.average_angle)

            decision_making.get_decision(sensor_data, decision_packet)

            print("Decision: ", decision_packet.decision)

            pid_controller.regulate(sensor_data)

            if not receive_queue.empty():
                packet = receive_queue.get()
                if packet.auto is not None:
                    auto = packet.auto 

            time.sleep(1)

        else:
            # Guys, this is test for server stuffs
            sensor_data = avr_communication.get_sensor_data(spi)
            print("Putting data in queue")

            send_queue.put(web.ServerSendPacket(sensor_data, corridor))

            if not receive_queue.empty():
                packet = receive_queue.get()
                if packet.auto is not None:
                    auto = packet.auto
                if packet.has_motion_command:
                    pass

            time.sleep(0.1)


if __name__ == '__main__':
    main()
