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

import communication
import comm_gui.web as web
import queue
import time
import decision_making
import pid_controller
import pdb
import math
import os


def main():
    spi = communication.communication_init()
    res = []
    
   # send_queue = queue.Queue()
   # receive_queue = queue.Queue()
   # thread = web.CommunicationThread(send_queue, receive_queue)

   # thread.start()

    while True:
        #pdb.set_trace()
        os.system('clear')
        sensor_data = communication.get_sensor_data(spi)
        print(sensor_data)

        angle_right = math.atan(math.fabs(sensor_data.ir_front_right - sensor_data.ir_back_right)/0.16)
        angle_left = math.atan(math.fabs(sensor_data.ir_front_left - sensor_data.ir_back_left)/0.16)

        angle = (angle_left + angle_right) * (90/math.pi)
        print("right_angle= ",angle_right*(180/math.pi))
        print("left_angle= ",angle_left*(180/math.pi))

        print(angle)
        decision = decision_making.get_decision(sensor_data)
        print(decision)
        pid_controller.reglate(sensor_data)
        time.sleep(1)

        #print(communication.walk(spi, 10, 2, 1))
        #time.sleep(1)

        # Guys, this is test for server stuffs
        # sensor_data = communication.SensorDataPacket(1, 1, 1, 1, 1, 1, 1)
        # time.sleep(0.1)
        # corridor = communication.CorridorDataPacket(2.0, 2.0, 3.0, 2.0, 0.0)
        # time.sleep(0.1)

        # print("Putting data in queue")
        # send_queue.put(web.ServerSendPacket(sensor_data, corridor))

        # if not receive_queue.empty():
        #     print("Getting: ")
        #     print(receive_queue.get().get_raw())
        # time.sleep(1)


if __name__ == '__main__':
    main()

