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

const float JOINT_1_MAX = M_PI / 2;
const float JOINT_1_MIN = -M_PI / 2;
const float JOINT_2_MAX = M_PI / 2;
const float JOINT_2_MIN = -M_PI / 2;
const float JOINT_3_MAX = M_PI / 2;
const float JOINT_3_MIN = -M_PI / 2;

const float JOINT_1_LENGTH = 0.05;
const float JOINT_2_LENGTH = 0.065;
const float JOINT_3_LENGTH = 0.13;

void leg_ik(float x, float y, float z, float* a1, float* a2, float* a3);

#endif
