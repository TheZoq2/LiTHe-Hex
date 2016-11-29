import unittest
import communication
import comm_gui.web as web
import time
import json
import math
import queue
import angle_calculation

SENSOR_ARGS = (1, 2, 3, 4, 5, 0x01, 0xFE)
DEBUG_STRING_NORMAL = "hello"
DEBUG_STRING_PARTIAL = "this is the debug string for the partial test"
AUTO_NORMAL = False
SENSOR_DATA_PACKET = communication.SensorDataPacket(*SENSOR_ARGS)

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
        sensor_data_packet = communication.SensorDataPacket(*SENSOR_ARGS)
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


