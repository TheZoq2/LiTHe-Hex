#include "ik.h"

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

float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
    float x = x1 - x2;
    float y = y1 - y2;
    float z = z1 - z2;

    return sqrt(x*x + y*y + z*z);
}

void leg_ik(float x, float y, float z, float* a1, float* a2, float* a3) {
    float gamma = atan2(-z, x);
    *a1 = fmin(fmax(gamma, JOINT_1_MIN), JOINT_1_MAX);

    float knee_pos_x = JOINT_1_LENGTH * cos(*a1);
    float knee_pos_z = -JOINT_1_LENGTH * sin(*a1);

    float dist_to_desired = distance(knee_pos_x, 0, knee_pos_z, x, y, z);
    float knee_toe_distance = fmin(dist_to_desired,
                                   JOINT_2_LENGTH + JOINT_3_LENGTH);

    float alpha = acos(y / dist_to_desired) +
        acos((JOINT_3_LENGTH*JOINT_3_LENGTH -
             JOINT_2_LENGTH*JOINT_2_LENGTH -
             knee_toe_distance*knee_toe_distance) /
             (-2 * JOINT_2_LENGTH * knee_toe_distance)) -
        M_PI / 2;

    float beta = acos((knee_toe_distance*knee_toe_distance -
                       JOINT_3_LENGTH * JOINT_3_LENGTH -
                       JOINT_2_LENGTH*JOINT_2_LENGTH) /
                      (-2 * JOINT_3_LENGTH * JOINT_2_LENGTH));

    *a2 = fmin(fmax(alpha, JOINT_2_MIN), JOINT_2_MAX);
    *a3 = fmin(fmax(beta, JOINT_3_MIN), JOINT_3_MAX);
}
