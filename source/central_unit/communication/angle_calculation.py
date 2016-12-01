import math

DISTANCE_BETWEEN_SENSORS = 0.16

# All these functions return angles in degrees
def get_right_angle(sensor_data):
    return math.atan((sensor_data.ir_front_right - sensor_data.ir_back_right)/DISTANCE_BETWEEN_SENSORS) * (180/math.pi)

def get_left_angle(sensor_data):
    return math.atan((sensor_data.ir_front_left - sensor_data.ir_back_left)/DISTANCE_BETWEEN_SENSORS) * (180/math.pi)

def get_average_angle(sensor_data):
    return ((get_left_angle(sensor_data) + get_right_angle(sensor_data))/2)
