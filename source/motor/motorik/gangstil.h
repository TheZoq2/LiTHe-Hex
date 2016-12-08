#ifndef GANGSTIL_H
#define GANGSTIL_H

// Copyright 2016 Noak Ringman, Emil Segerbäck, Robin Sliwa, Frans Skarman, Hannes Tuhkala, Malcolm Wigren, Olav Övrebö

// This file is part of LiTHe Hex.

// LiTHe Hex is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// LiTHe Hex is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with LiTHe Hex.  If not, see <http://www.gnu.org/licenses/>.

#include <math.h>
#include "ik.h"
#include <stdint.h>
#include "servo.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

extern const size_t LF ;
extern const size_t RF ;
extern const size_t LM ;
extern const size_t RM ;
extern const size_t LB ;
extern const size_t RB ;

extern const float FRONT_LEG_JOINT_X           ;
extern const float FRONT_LEG_JOINT_Y           ;
extern const float MID_LEG_JOINT_Y             ;
extern const float HIGH                        ;
extern const float GROUNDED                    ;
extern const float MIN_DIST                    ;
extern const float MAX_DIST                    ;
extern const float VERT_MID_LEG_BORDER_OFFSET  ;
extern const float VERT_HEAD_LEG_BORDER_OFFSET ;
extern const float HORIZ_BORDER_TILT           ;
extern const float DIAG_DIVISIVE_BORDER_TILT   ;
extern const float CLOSE_BORDER_OFFSET         ;
extern const float DIAG_DIVISIVE_BORDER_OFFSET ;
extern const float CLOSE_BORDER_TILT           ;
//extern const size_t NUM_LEGS                   ;

//Defined as a #define because you can not allocate an array of a const variable size
#define NUM_LEGS 6


/**
 * struct used to represent leg positions, leg movements and robot positions. x is
 * positive forward, seen from the robot. y is positive to the left.
 */
typedef struct{
    float x;
    float y;
}Point2D;

/**
 * @brief rotateSetAngle keeps rotating until the robot has fully altered its
 * direction by the requested angle.
 * @param angle provides the angle the robot should rotate.
 * @param current position the legs currently hold.
 */
void rotate_set_angle(float angle, Point2D * current);

/**
 * @brief workTowardsGoal takes the robot closer to a requested position and
 * rotation, stepping with the set of legs that best accomplishes this.
 *
 * Also, returns the scaledown applied to whichever set of legs is selected as
 * optimal for forward movement.
 * @param rot requested angle the robot should preferably rotate, positive
 * anticlockwise as seen from above, whilst moving to the requested goal
 * position.
 * @param goal requested position to which the robot should preferrably be able
 * to move.
 * @param current position the legs curerently hold.
 * @return scaledown applied to grounded set of legs.
 */
float work_towards_goal(float rot, Point2D goal, Point2D * current);



/**
 * @brief get_default_leg_position gives a default position for requested leg.
 * @param leg indicates what leg of the robot (LF, RF, LM, RM, LB, RB) should be returned.
 * @return a standardised leg position, relative to joint.
 */
Point2D get_default_leg_position(size_t leg);



/**
 * @brief assume_standardized_stance Positions the robot in a reliable, standardized
 * hard-coded stance.
 * @param current current position of the legs.
 */
void assume_standardized_stance(Point2D * current);

/**
	Raises the body into the default position as defined by get_default_leg_postion()
*/
void raise_to_default_position(Point2D* current_position);

Point2D get_leg_position_from_radius
			(size_t leg, float distance_from_body, float outer_leg_x_offset);

#ifdef IS_UNIT_TEST
Point2D rotate_point_by_angle(Point2D original, float angle);
Point2D robot_to_ik_coords(Point2D original, size_t leg);
float absf(float a);
#endif


#ifdef IS_X86
struct CurrentServoState
{
	Point2D points[NUM_LEGS];
	float heights[NUM_LEGS];
	struct Leg angles[NUM_LEGS];
};

struct CurrentServoState current_servo_state;

void write_current_state();
Point2D divide_point2D(Point2D vector, float divisor);
Point2D vector_between_points(Point2D current, Point2D target);
#endif


#endif
