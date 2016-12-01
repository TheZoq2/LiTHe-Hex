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

import unittest
import communication.avr_communication as avr_communication
import comm_gui.web as web
import tests.fake_spi as fake_spi
import time
import json
import math
import queue
import angle_calculation

SENSOR_ARGS = (1, 2, 3, 4, 5, 0x01, 0xFE)
DEBUG_STRING_NORMAL = "hello"
DEBUG_STRING_PARTIAL = "this is the debug string for the partial test"
AUTO_NORMAL = False
SENSOR_DATA_PACKET = avr_communication.SensorDataPacket(*SENSOR_ARGS)

EXPECTED_NORMAL = {
    "ir_down" : 0.01,
    "ir_fl" : 0.02,
    "ir_bl" : 0.03,
    "ir_fr" : 0.04,
    "ir_br" : 0.05,
    "lidar" : 5.1,
    "angle_l": angle_calculation.get_left_angle(SENSOR_DATA_PACKET),
    "angle_r": angle_calculation.get_right_angle(SENSOR_DATA_PACKET),
    "angle_avg" : angle_calculation.get_average_angle(SENSOR_DATA_PACKET),
    "auto" : AUTO_NORMAL,
    "debug" : DEBUG_STRING_NORMAL
}

EXPECTED_PARTIAL = {
    "debug" : DEBUG_STRING_PARTIAL
}


class WebTestCase(unittest.TestCase):

    def test_send_packet_normal_json(self):
        sensor_data_packet = avr_communication.SensorDataPacket(*SENSOR_ARGS)
        send_packet = web.ServerSendPacket(
            sensor_data_packet,
            AUTO_NORMAL,
            DEBUG_STRING_NORMAL)
        json_string = send_packet.get_json()
        self.assertDictEqual(EXPECTED_NORMAL, json.loads(json_string))

    def test_send_packet_partial(self):
        send_packet = web.ServerSendPacket(debug_string=DEBUG_STRING_PARTIAL)
        json_string = send_packet.get_json()
        self.assertDictEqual(EXPECTED_PARTIAL, json.loads(json_string))

    def test_send_packet_empty(self):
        send_packet = web.ServerSendPacket()
        json_string = send_packet.get_json()
        self.assertDictEqual({}, json.loads(json_string))


class SpiTestCase(unittest.TestCase):

    def test_send_bytes(self):
        spi = fake_spi.SpiDev()
        seq = [0x00, 0x0F, 0xFF]
        spi.set_expected_write_sequence([avr_communication.GARBAGE] + seq)
        try:
            avr_communication.writebytes(spi, seq)
        except fake_spi.UnexpectedDataException as e:
            self.fail("Expected {}, got {}".format(e.expected, e.actual))


