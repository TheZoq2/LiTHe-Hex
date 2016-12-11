#include "conversion_util.h"

#include <math.h>

int radian_to_servo(float radian_angle)
{
	int result = (int)(radian_angle * (0x1ff/150*180) / M_PI);
	return result;
}
