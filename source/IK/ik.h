#ifndef IK_H
#define IK_H

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

/*
 * The maximum and minimum angles that the different joints can turn.
 * TODO: tweak
 */
const static float JOINT_1_MAX = M_PI / 2;
const static float JOINT_1_MIN = -M_PI / 2;
const static float JOINT_2_MAX = M_PI / 2;
const static float JOINT_2_MIN = -M_PI / 2;
const static float JOINT_3_MAX = M_PI / 2;
const static float JOINT_3_MIN = -M_PI / 2;

const static float JOINT_1_LENGTH = 0.05;
const static float JOINT_2_LENGTH = 0.065;
const static float JOINT_3_LENGTH = 0.13;

/* Used because some joints are angled and need a different angle than 0 to
 * point straight. TODO: tweak
 */
const static float JOINT_2_OFFSET = 0;
const static float JOINT_3_OFFSET = 0;

struct Leg {
    float angle1;
    float angle2;
    float angle3;
};

/**
 * Calculates the angle of all joints of a leg, given the desired position of
 * the foot. Coordinates are given relative to the joint closest to the body.
 * The y axis points up, the x axis points straight out from the leg mount, and
 * the z axis points in the cross product of x and y.
 *
 * @return The angles of all the joints of the leg
 */
struct Leg leg_ik(float x, float y, float z);

#endif
