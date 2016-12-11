#ifndef H_COVNERSION_UTIL
#define H_COVNERSION_UTIL

/**
 * @brief radian_to_servo converts between radians and the unit of angular
 * measurement used in the ax12-servos.
 * @param radian_angle angle to be converted.
 * @return angle compatible with the servos' value interpretation of angles.
 */
int radian_to_servo(float radian_angle);

#endif
