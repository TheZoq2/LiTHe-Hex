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
import communication.web as web
import tests.fake_spi as fake_spi
import time
import json
import main
import math
import pdb
import queue
import communication.angle_calculation as angle_calculation

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
    """
    Tests for the web module.
    """

    def test_send_packet_normal_json(self):
        """
        Tests a normal json conversion with all of the possible keys.
        """
        sensor_data_packet = avr_communication.SensorDataPacket(*SENSOR_ARGS)
        send_packet = web.ServerSendPacket(
            sensor_data_packet,
            AUTO_NORMAL,
            DEBUG_STRING_NORMAL)
        json_string = send_packet.get_json()
        self.assertDictEqual(EXPECTED_NORMAL, json.loads(json_string))

    def test_send_packet_partial(self):
        """
        Tests a json conversion with only a few keys.
        """
        send_packet = web.ServerSendPacket(debug_string=DEBUG_STRING_PARTIAL)
        json_string = send_packet.get_json()
        self.assertDictEqual(EXPECTED_PARTIAL, json.loads(json_string))

    def test_send_packet_empty(self):
        """
        Tests json conversion of no keys.
        """
        send_packet = web.ServerSendPacket()
        json_string = send_packet.get_json()
        self.assertDictEqual({}, json.loads(json_string))


class SpiTestCase(unittest.TestCase):
    """
    Tests for the avr_communication module.
    """

    def test_send_bytes(self):
        """
        Tests the _send_bytes function.
        """
        spi = fake_spi.SpiDev()
        seq = [0x00, 0x0F, 0xFF]
        spi.set_expected_write_sequence([avr_communication.GARBAGE] + seq)
        try:
            avr_communication._send_bytes(spi, *seq)
        except fake_spi.UnexpectedDataException as e:
            self.fail("Expected {}, got {}".format(e.expected, e.actual))

    def test_read_single_byte(self):
        """
        Tests the _recieve_bytes function for a single byte message.
        """
        spi = fake_spi.SpiDev()
        msg = 0xFE
        # type that indicates that it's a one byte message with an
        # uneven number of ones
        type_ = 0b00001110
        spi.set_fake_read_sequence([type_, msg])
        result = avr_communication._recieve_bytes(spi)
        self.assertListEqual(result, [msg])

    def test_read_multiple_bytes(self):
        """
        Tests the _recieve_bytes function for a multi-byte message.
        """
        spi = fake_spi.SpiDev()
        msg = [0x01, 0x02, 0x03, 0x01]
        # type that indicates that it's a multibyte message with an
        # uneven number of ones in the message and length byte
        type_ = 0b10001111
        spi.set_fake_read_sequence([type_, 4] + msg)
        result = avr_communication._recieve_bytes(spi)
        self.assertListEqual(result, msg)


def _set_sensor_data_sequence(spi):
    # it should send a data request for the sensor data.
    spi.set_expected_write_sequence([avr_communication.GARBAGE,
                                     (avr_communication.DATA_REQ << 2) | 0x01, 
                                     avr_communication.SENSOR_DATA])
    # we should receive an acknowledge along with the sensor data
    # type with the lower parity bit set to 1, since te rest of the message
    # has an uneven number of bits
    spi.set_fake_read_sequence([avr_communication.ACK, # ack
                                (avr_communication.SENSOR_DATA << 2) | 0x01, # type
                                7, # length
                                1, 1, 1, 1, 1, 1, 2]) # msg


def _add_servo_speed_and_walk(spi):
    spi.expected_seq += [
        avr_communication.GARBAGE,
        (avr_communication.SET_SERVO_SPEED << 2) | 0x01, # type
        2, # length
        0xFF, 0xFF, # servo speed

        avr_communication.GARBAGE,
        (avr_communication.WALK << 2) | 0x03, # type
        
        3, # length
        255, 0, 127 # x, y, r
    ]
    spi.data_sequence += [avr_communication.ACK, avr_communication.ACK]


class MainLoopTestCase(unittest.TestCase):
    """
    Test for the methods used by the main loop.
    """

    def test_manual_mode_no_input(self):
        """
        Tests an iteration of the manual mode in the main loop
        without data input from the server.
        Makes sure that if does not spontaneously turn on
        the auto mode, that is requests and sucessfully receives
        a sensor data packet, puts it in the queue, and that the 
        avr_communication reads the values correctly.
        """
        send_queue = queue.Queue()
        receive_queue = queue.Queue()
        spi = fake_spi.SpiDev()

        _set_sensor_data_sequence(spi)

        try:
            auto = main.do_manual_mode_iteration(spi, send_queue, receive_queue)
        except fake_spi.UnexpectedDataException as e:
            self.fail("Expected {}, got {}".format(e.expected, e.actual))

        self.assertFalse(auto)
        self.assertFalse(send_queue.empty())
        packet = send_queue.get()
        self.assertTrue(send_queue.empty())
        
        sensor_data = packet.sensor

        self.assertEqual(0.01, sensor_data.ir_down)
        self.assertEqual(0.01, sensor_data.ir_front_left)
        self.assertEqual(0.01, sensor_data.ir_back_left)
        self.assertEqual(0.01, sensor_data.ir_front_right)
        self.assertEqual(0.01, sensor_data.ir_back_right)
        self.assertEqual(2.58, sensor_data.lidar)

    def test_auto_change(self):
        """
        Tests whether auto mode is changed to true if such
        a command is sent from the server, when in manual mode.
        """
        send_queue = queue.Queue()
        receive_queue = queue.Queue()
        spi = fake_spi.SpiDev()
        _set_sensor_data_sequence(spi)

        packet = web.ServerReceivedPacket("{\"auto\": true}")

        receive_queue.put(packet)

        try:
            auto = main.do_manual_mode_iteration(spi, send_queue, receive_queue)
        except fake_spi.UnexpectedDataException as e:
            self.fail(
                "Something went wrong when reading sensor data: Expected {}, got {}"
                .format(e.expected, e.actual))

        self.assertTrue(auto)

    def test_auto_sent_but_not_changed(self):
        """
        Makes sure the auto mode does not change if a value for auto mode
        is sent but set to false when in manual mode.
        """
        send_queue = queue.Queue()
        receive_queue = queue.Queue()
        spi = fake_spi.SpiDev()
        _set_sensor_data_sequence(spi)

        packet = web.ServerReceivedPacket("{\"auto\": false}")

        receive_queue.put(packet)

        try:
            auto = main.do_manual_mode_iteration(spi, send_queue, receive_queue)
        except fake_spi.UnexpectedDataException as e:
            self.fail(
                "Something went wrong when reading sensor data: Expected {}, got {}"
                .format(e.expected, e.actual))

        self.assertFalse(auto)
        
    def test_manual_walk(self):

        send_queue = queue.Queue()
        receive_queue = queue.Queue()
        spi = fake_spi.SpiDev()
        _set_sensor_data_sequence(spi)
        _add_servo_speed_and_walk(spi)

        packet = web.ServerReceivedPacket(
            "{\"x\": 1.0, \"y\": -1.0, \"thrust\": 1.0, \"rotation\": 0.0}")

        receive_queue.put(packet)

        try:
            auto = main.do_manual_mode_iteration(spi, send_queue, receive_queue)
        except fake_spi.UnexpectedDataException as e:
            self.fail(
                "Something went wrong in the SPI-communication: Expected {}, got {}"
                .format(e.expected, e.actual))

        self.assertFalse(auto)
    
