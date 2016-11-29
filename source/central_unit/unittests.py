import unittest
import communication
import comm_gui.web as web
import time
import json
import math
import queue

SENSOR_ARGS = (1, 2, 3, 4, 5, 0x01, 0xFE)
CORRIDOR_ARGS = (0x01, 0xFE, 1, 2, 3, 180)
CORRIDOR_ARGS_PARTIAL = (0x00, 0xEE, 1, 2, 3, 90)
DEBUG_STRING_NORMAL = "hello"
DEBUG_STRING_PARTIAL = "this is the debug string for the partial test"
AUTO_NORMAL = False

EXPECTED_NORMAL = {
    "ir_down" : 0.01,
    "ir_fl" : 0.02,
    "ir_bl" : 0.03,
    "ir_fr" : 0.04,
    "ir_br" : 0.05,
    "lidar" : 5.1,
    "dist_f" : 5.1,
    "dist_d" : 0.01,
    "dist_l" : 0.02,
    "dist_r" : 0.03,
    "corr_angle" : math.pi,
    "auto" : AUTO_NORMAL,
    "debug" : DEBUG_STRING_NORMAL
}

EXPECTED_PARTIAL = {
    "dist_f" : 5.1,
    "dist_d" : 0.01,
    "dist_l" : 0.02,
    "dist_r" : 0.03,
    "corr_angle" : math.pi / 2,
    "debug" : DEBUG_STRING_PARTIAL
}

class WebTestCase(unittest.TestCase):

    def test_send_packet_normal_json(self):
        sensor_data_packet = communication.SensorDataPacket(*SENSOR_ARGS)
        corridor_packet = communication.CorridorDataPacket(*CORRIDOR_ARGS)
        send_packet = web.ServerSendPacket(
            sensor_data_packet,
            corridor_packet,
            AUTO_NORMAL,
            DEBUG_STRING_NORMAL)
        json_string = send_packet.get_json()
        self.assertDictEqual(EXPECTED_NORMAL, json.loads(json_string))

    def test_send_packet_partial(self):
        corridor_packet = communication.CorridorDataPacket(*CORRIDOR_ARGS_PARTIAL)
        send_packet = web.ServerSendPacket(
            corridot_data_packet=corridor_packet,
            debug=DEBUG_STRING_PARTIAL)
        json_string = send_packet.get_json()
        self.assertDictEqual(EXPECTED_NORMAL, json.loads(json_string))


    def test_send_packet_empty(self):
        send_packet = web.ServerSendPacket()
        json_string = send_packet.get_json()
        self.assertDictEqual({}, json.loads(json_string))


