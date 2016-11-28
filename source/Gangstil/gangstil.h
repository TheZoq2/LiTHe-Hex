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
#include "../IK/ik.h"
#include <stdint.h>
#include "../../motor/motorik/servo.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

const size_t LF = 0;
const size_t RF = 1;
const size_t LM = 2;
const size_t RM = 3;
const size_t LB = 4;
const size_t RB = 5;

const float FRONT_LEG_JOINT_X           = 0.12;
const float FRONT_LEG_JOINT_Y           = 0.06;
const float MID_LEG_JOINT_Y             = 0.1;
const float HIGH                        = 0.1;
const float GROUNDED                    = 0;
const float MIN_DIST                    = 0.06;
const float MAX_DIST                    = 0.18;
const float VERT_MID_LEG_BORDER_OFFSET  = 0.06;
const float VERT_HEAD_LEG_BORDER_OFFSET = -0.03;
const float HORIZ_BORDER_TILT           = 0;
const float DIAG_DIVISIVE_BORDER_TILT   = 1.3333333;
const float CLOSE_BORDER_OFFSET         = 0.085;
const float DIAG_DIVISIVE_BORDER_OFFSET = 0.045;
const float CLOSE_BORDER_TILT           = -1;

const size_t NUM_LEGS = 6;


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
float work_towards_goal(float rot, Point2D * goal, Point2D * current);

#endif
