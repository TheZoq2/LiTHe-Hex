#include "gangstil.h"
//#include <inttypes.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

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


const size_t LF = 0;
const size_t RF = 1;
const size_t LM = 2;
const size_t RM = 3;
const size_t LB = 4;
const size_t RB = 5;

const float FRONT_LEG_JOINT_X           = 0.12;
const float FRONT_LEG_JOINT_Y           = 0.06;
const float MID_LEG_JOINT_Y             = 0.1;
const float HIGH                        = 0.05;
const float GROUNDED                    = -0.1;
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
 * @brief get_angle_set produces an array of the leg angles as calculated by the IK.
 * @param target provides the coordinates relative to the joints for all the legs, as 
 * they should be after movement.
 * @return array ordered LF RF LM RM LB RB (left/right - front/mid/back) of calculated 
 * angles for the legs.
 */
struct Leg* get_angle_set(Point2D * target, float * height){
    struct Leg* res = (struct Leg *)calloc(NUM_LEGS, sizeof(struct Leg));
    float x;
    float y;
    float z;
    for (size_t leg = 0; leg < NUM_LEGS; ++leg){
        if ((leg & 1) == 0){ //left hand side of robot
            x = target[leg].y;
            z = target[leg].x;
        }
        else{ //right hand side of robot
            x = -target[leg].y;
            z = -target[leg].x;
        }
        y = height[leg];
        res[leg] = leg_ik(x,y,z);
    }

    res[LF].angle1 = res[LF].angle1 + (M_PI / 4);
    res[RF].angle1 = res[RF].angle1 - (M_PI / 4);
    res[LB].angle1 = res[LB].angle1 - (M_PI / 4);
    res[RB].angle1 = res[RB].angle1 + (M_PI / 4);


//for debug
//    printf("\n");
//    for (int var = 0; var < 6; ++var) {
//        printf("leg %d rad angle: %f, %f, %f\n", var, res[var].angle1, res[var].angle2, res[var].angle3);
//    }
//    printf("\n");

    return res;
}

//for debug without motor
/*
void set_leg_angles(uint8_t id, uint16_t * angle){
    printf("set leg %" PRIu8" to angles ", id);
    printf("%" PRIu16, angle[0]);
    printf(", %" PRIu16, angle[1]);
    printf(",%" PRIu16"\n", angle[2]);
}

void send_servo_action(){
    printf("execute angles\n\n");
}*/


/**
 * @brief takes a target set of leg positions and causes the servos to execute them.
 * @param target set of foot positions arranged LF RF LM RM LB RB, indicating 
 * intended final placement of feet relative to the mounts of their joints.
 */
void execute_position(Point2D * target, float * z){
    struct Leg* ik = get_angle_set(target, z);

    uint16_t* angle = (uint16_t*)calloc(3, sizeof(uint16_t));
    uint8_t legId;
    for (size_t leg = 0; leg < NUM_LEGS; ++leg){
        int ang;
        if ((leg & 1) == 0){
            angle[0] = (uint16_t)(0x1ff - (int)(ik[leg].angle1 * (0x1ff/150*180) / M_PI));
            angle[1] = (uint16_t)(0x1ff + (int)(ik[leg].angle2 * (0x1ff/150*180) / M_PI));
            angle[2] = (uint16_t)(0x1ff + (int)(ik[leg].angle3 * (0x1ff/150*180) / M_PI));
            legId = (uint8_t)(leg/2);
        }
        else{
            angle[0] = (uint16_t)(0x1ff - (int)(ik[leg].angle1 * (0x1ff/150*180) / M_PI));
            angle[1] = (uint16_t)(0x1ff - (int)(ik[leg].angle2 * (0x1ff/150*180) / M_PI));
            angle[2] = (uint16_t)(0x1ff - (int)(ik[leg].angle3 * (0x1ff/150*180) / M_PI));
            legId = (uint8_t)(leg/2 + 3);
        }

        set_leg_angles(legId, angle);

		printf("yoloswag\n");
    }
    send_servo_action();
    
    free(angle);
}


void execute_step(Point2D * current, Point2D * target, bool lrlRaised){
    float * z = (float *)calloc(NUM_LEGS, sizeof(float));

    if(lrlRaised){
        z[LF] = GROUNDED + HIGH;
        z[RM] = GROUNDED + HIGH;
        z[LB] = GROUNDED + HIGH;
        z[RF] = GROUNDED;
        z[LM] = GROUNDED;
        z[RB] = GROUNDED;
    }
    else{
        z[LF] = GROUNDED;
        z[RM] = GROUNDED;
        z[LB] = GROUNDED;
        z[RF] = GROUNDED + HIGH;
        z[LM] = GROUNDED + HIGH;
        z[RB] = GROUNDED + HIGH;
    }
    execute_position(current, z);
    execute_position(target, z);
    z[LF] = GROUNDED;
    z[RM] = GROUNDED;
    z[LB] = GROUNDED;
    z[RF] = GROUNDED;
    z[LM] = GROUNDED;
    z[RB] = GROUNDED;
    execute_position(target, z);
    
    free(z);

    for (int leg = 0; leg < NUM_LEGS; ++leg) {
        current[leg].x = target[leg].x;
        current[leg].y = target[leg].y;
    }
}

/**
 * @brief get_default_leg_position gives a default position for requested leg.
 * @param leg indicates what leg of the robot (LF, RF, LM, RM, LB, RB) should be returned.
 * @return a standardised leg position, relative to joint.
 */
Point2D * get_default_leg_position(size_t leg){
    Point2D * res = (Point2D *)malloc(sizeof(Point2D));
    if (leg < 2){   //front
        res->x = 0.1;
        res->y = 0.1;
    }
    else if (leg < 4){  //mid
        res->x = 0;
        res->y = 0.14;
    }
    else{
        res->x = -0.1;
        res->y = 0.1;
    }    //back

    if ((leg & 1) == 1)  //right
        res->y = -res->y ;
    return res;
}


/**
 * @brief joint_position gives the horizontal position of requested leg-robot joint
 * @param leg indicates what leg joint of the robot (LF, RF, LM, RM, LB, RB) should be returned.
 * @return a standardised joint position, relative to robot center.
 */
Point2D * joint_position(size_t leg){
    Point2D * res = (Point2D *)malloc(sizeof(Point2D));
    if (leg < 2){   //front
        res->x = FRONT_LEG_JOINT_X;
        res->y = FRONT_LEG_JOINT_Y;
    }
    else if (leg < 4){  //mid
        res->x = 0;
        res->y = MID_LEG_JOINT_Y;
    }
    else{
        res->x = -FRONT_LEG_JOINT_X;
        res->y = FRONT_LEG_JOINT_Y;
    }    //back

    if ((leg & 1) == 1)  //right
        res->y = -res->y ;
    return res;
}


/**
 * @brief absf returns the absolute value of a given float.
 * @param a float to be treated.
 * @return a if a > 0, else -a.
 */
float absf(float a){
    if(a < 0)
        return -a;
    return a;
}


/**
 * @brief dist returns the absolute distance of a point-represented vector.
 * @param vect point to which absolute distance from origo is to be returned.
 * @return absolute distance to entered point from origo.
 */
float dist(Point2D * vect){
    return (float)(sqrt((vect->x * vect->x) + (vect->y * vect->y)));
}


/**
 * @brief scale_to_range_bounds Based on requested leg movement and current foot position
 * calculates how much the movement needs to be downscaled for the foot to land within
 * the maximum range of the leg.
 *
 * Only returns 1 (that is, implies no downscaling is needed)if the intended position
 * is already within the maximum reach of the leg.
 *
 * Note that scale_to_range_bounds is not sufficient to limit the movement of the legs;
 * further limitation (covered by scale_to_straight_bounds) is needed to avoid going
 * under or into the robot with the legs, and to avoid legs trying to intersect with
 * each other.
 * @param targLength distance from joint to intended foot position (after movement).
 * @param currLength distance from joint to current foot position (before movement).
 * @param diffLength distance from current foot position (before movement) to intended
 * foot position (after movement).
 * @return float that, when intened leg movement is multiplied with it, reduces the
 * leg movement to where it comes within the maximum reach of the leg.
 */
float scale_to_range_bounds(float targLength, float currLength, float diffLength){
    if (currLength <= MAX_DIST)
        return 1; //no scaling down needed

    float alpha = acos((powf(diffLength, 2) + powf(currLength, 2) - powf(targLength, 2))//no n/0 since diffLength & currLength > 0 if statement is entered
                       / (2 * diffLength * currLength));    // a = acos ((B2 + C2 - A2)/2BC), cosine trig formula
    float beta = asin(currLength * sin(alpha) / MAX_DIST);     // b = asin (B * sin(a)/A ) // sin(b)/B = sin(a)/A, sine trig formula
    float gamma = M_PI - alpha - beta;                      //sum internal angles = PI
    float optimalDiff  = absf(MAX_DIST * sin(gamma) / sin(alpha));  //sin(alpha)never 0, if current leg pos legal and statement entered.
                                                                 //C = A * sin(c) / sin(a),
                                                                 //sine trig formula (see above)
                                                                 //absf(...)eliminates errors from asin simplification of angles
    return optimalDiff/diffLength;       //scale multiplier for optimal diff
}


/**
 * @brief minf returns the smaller of two float values.
 * @param a first float value, see @brief.
 * @param b second float value, see @brief.
 * @return the lesser value of @param a and @param b.
 */
float minf(float a, float b){
    if (a < b)
        return a;
    return b;
}


/**
 * @brief update_targ_by_scale takes a set of required scaledowns for the available legs,
 * chooses for sets of lrl and rlr (front to back) the worst scaledown needed, and
 * applies that to the sets of legs by updating target and diff vectors.
 *
 * Furthermore, the function returns the scaledown applied to the grounded set of legs.
 * @param targ  intended future position of feet.
 * @param curr  current position of feet.
 * @param diff  intended movement vectors for feet.
 * @param scale set of required scaledowns for the feet
 * @param lrlRaised true if lrl-set of legs are raised, meaning rlr-set of legs is
 * grounded. Conversely, lrlRaised false means lrl set of legs is grounded. Determines
 * what scaledown should be returned.
 * @return lowest scale multiplier of grounded set of legs.
 */
float update_targ_by_scale(Point2D * targ, Point2D * curr, Point2D * diff, float * scale, bool lrlRaised){

    float lrlScaledown = minf(minf(scale[0], scale[3]), scale[4]);
    float rlrScaledown = minf(minf(scale[1], scale[2]), scale[5]);

    for(size_t index = 0; index < NUM_LEGS; ++index){
        float scale = lrlScaledown;
        if (index == RF || index == LM || index == RB)
            scale = rlrScaledown;
        diff[index].x = diff[index].x * scale;              //update diff
        diff[index].y = diff[index].y * scale;
        targ[index].x = curr[index].x + diff[index].x;       //update target
        targ[index].y = curr[index].y + diff[index].y;
    }

    if (lrlRaised)
        return rlrScaledown;
    else
        return lrlScaledown;
}


/**
 * @brief boundry_intersect gives a scale multiplier needed to avoid crossing boundry,
 * expressed in m- and k-value, all things relative to the joint of the leg.
 * @param curr current position of feet.
 * @param targ intended future position of feet.
 * @param k value used in describing the function of the boundry, expressed as
 * y = kx + m, relative to the joint. See @param upperLimit below for more
 * information.
 * @param m value used in describing the function of the boundry, expressed as
 * y = kx + m, relative to the joint. See @param upperLimit below for more
 * information.
 * @param upperLimit bool deciding wether or not the border should be considered an
 * upper or lower limit. If true, the border is considered an upper limit (for
 * y-value, y positive to the left of the robot, x positive forward).
 * @return float that, when intened leg movement is multiplied with it, reduces the
 * leg movement to where it does not cross the boundry provided.
 */
float boundry_intersect(Point2D * curr, Point2D * targ, float k, float m, bool upperLimit){

    if((targ->y - m> k * targ->x) != upperLimit)
        return 1; //no scaling needed; targ is within boundry

    if (targ->x == curr->x){    //vertical leg movement
        float diff = (curr->x * k) + m - curr->y;
        float scale =absf(diff/(targ->y - curr->y));
        return scale;//no div by zero; that would mean legs do not move
    }

    float k2 = (targ->y - curr->y)/(targ->x - curr->x); //no div by zero; see preceeding if statement
    float m2 = curr->y - (k2 * curr->x);

    if (k == k2)
        return 1;   //parallel lines; no scaling. Should not realistically happen.

    float newX = (m - m2)/(k2 - k); //no div by zero; see preceeding if statement
    float scale =(newX - curr->x) / (targ->x - curr->x);
    return scale;  //no div by zero; see prior if statement
}


/**
 * @brief scale_to_straight_bounds calls boundry_intersect for each leg, for a set of
 * hard-coded values, for each leg setting scale to the lowest (worst) scaledown
 * needed for that leg.
 * @param scale array of floats for storing scaledown needed for each leg.
 * @param targ intended movement vectors for feet.
 * @param curr current position of feet.
 */
void scale_to_straight_bounds(float * scale, Point2D * targ, Point2D * curr){
    scale[LF] = minf(boundry_intersect(& curr[LF], & targ[LF], HORIZ_BORDER_TILT, VERT_HEAD_LEG_BORDER_OFFSET, false),
                minf(boundry_intersect(& curr[LF], & targ[LF], DIAG_DIVISIVE_BORDER_TILT, DIAG_DIVISIVE_BORDER_OFFSET, true),
                     boundry_intersect(& curr[LF], & targ[LF], CLOSE_BORDER_TILT, CLOSE_BORDER_OFFSET, false)));

    scale[RF] = minf(boundry_intersect(& curr[RF], & targ[RF], HORIZ_BORDER_TILT, -VERT_HEAD_LEG_BORDER_OFFSET, true),
                minf(boundry_intersect(& curr[RF], & targ[RF], -DIAG_DIVISIVE_BORDER_TILT, -DIAG_DIVISIVE_BORDER_OFFSET, false),
                     boundry_intersect(& curr[RF], & targ[RF], -CLOSE_BORDER_TILT, -CLOSE_BORDER_OFFSET, true)));

    scale[LM] = minf(boundry_intersect(& curr[LM], & targ[LM], DIAG_DIVISIVE_BORDER_TILT, -DIAG_DIVISIVE_BORDER_OFFSET, false),
                minf(boundry_intersect(& curr[LM], & targ[LM], HORIZ_BORDER_TILT, VERT_MID_LEG_BORDER_OFFSET, false),
                     boundry_intersect(& curr[LM], & targ[LM], -DIAG_DIVISIVE_BORDER_TILT, -DIAG_DIVISIVE_BORDER_OFFSET, false)));

    scale[RM] = minf(boundry_intersect(& curr[RM], & targ[RM], -DIAG_DIVISIVE_BORDER_TILT, DIAG_DIVISIVE_BORDER_OFFSET, true),
                minf(boundry_intersect(& curr[RM], & targ[RM], HORIZ_BORDER_TILT, -VERT_MID_LEG_BORDER_OFFSET, true),
                     boundry_intersect(& curr[RM], & targ[RM], DIAG_DIVISIVE_BORDER_TILT, DIAG_DIVISIVE_BORDER_OFFSET, true)));

    scale[LB] = minf(boundry_intersect(& curr[LB], & targ[LB], HORIZ_BORDER_TILT, VERT_HEAD_LEG_BORDER_OFFSET, false),
                minf(boundry_intersect(& curr[LB], & targ[LB], -DIAG_DIVISIVE_BORDER_TILT, DIAG_DIVISIVE_BORDER_OFFSET, true),
                     boundry_intersect(& curr[LB], & targ[LB], -CLOSE_BORDER_TILT, CLOSE_BORDER_OFFSET, false)));

    scale[RB] = minf(boundry_intersect(& curr[RB], & targ[RB], HORIZ_BORDER_TILT, -VERT_HEAD_LEG_BORDER_OFFSET, true),
                minf(boundry_intersect(& curr[RB], & targ[RB], DIAG_DIVISIVE_BORDER_TILT, -DIAG_DIVISIVE_BORDER_OFFSET, false),
                     boundry_intersect(& curr[RB], & targ[RB], CLOSE_BORDER_TILT, -CLOSE_BORDER_OFFSET, true)));
}


/**
 * @brief scale_legs adjusts target position for the legs so that all final foot
 * positions fall within possible areas, without intersecting with each other.
 *
 * Also returns the scaledown applied to the grounded set of legs.
 * @param targ  intended future position of feet.
 * @param curr  current position of feet.
 * @param diff  intended movement vectors for feet.
 * @param scale set of required scaledowns for the feet
 * @param lrlRaised true if lrl-set of legs are raised, meaning rlr-set of legs is
 * grounded. Conversely, lrlRaised false means lrl set of legs is grounded.
 * @return scale multiplier applied to grounded set of legs.
 */
float scale_legs(Point2D * targ, Point2D * curr, float * scale, bool lrlRaised){
    Point2D * diff = (Point2D *)calloc(NUM_LEGS, sizeof(Point2D));
    for(size_t index = 0; index < NUM_LEGS; ++index){
        diff[index].x = targ[index].x - curr[index].x;
        diff[index].y = targ[index].y - curr[index].y;

        float targLength = dist( & targ[index]);

        scale[index] = scale_to_range_bounds(targLength, dist(& curr[index]), dist(& diff[index]));
    }

    float res = update_targ_by_scale(targ, curr, diff, scale, lrlRaised);


    scale_to_straight_bounds(scale, targ, curr);

    res = res * update_targ_by_scale(targ, curr, diff, scale, lrlRaised);

    free(diff);
    return res;
}


/**
 * @brief direct_legs calculates basic movements, asuming perfect leg freedom, for
 * ideal fullfillment of intended movement.
 * @param rot intended rotation to be performed across intended length of movement.
 * @param targ  intended future position of feet.
 * @param current current position of feet.
 * @param req preferred movement of robot, relative to current position.
 * @param lrlRaised true if lrl-set of legs are raised, meaning rlr-set of legs is
 * grounded. Conversely, lrlRaised false means lrl set of legs is grounded. Used to
 * determine what direction each leg should be moved, relative to the body (away
 * from target, with negative rotation, if feet are grounded).
 */
void direct_legs(float rot, Point2D * targ, Point2D * current, Point2D * req, bool lrlRaised){
    Point2D * attention = (Point2D *)malloc(sizeof(Point2D));
    Point2D * absTarg   = (Point2D *)malloc(sizeof(Point2D));

    for(size_t leg = LF; leg < NUM_LEGS; ++leg){
        Point2D * joint = joint_position(leg);
        attention->x = current[leg].x + joint->x;
        attention->y = current[leg].y + joint->y;

        if (lrlRaised == (leg == 0 || leg == 3 || leg == 4)){ //move legs "away" from position (body towards)
            absTarg->x =  req->x + cos(rot) * attention->x - sin(rot) * attention->y;
            absTarg->y =  req->y + sin(rot) * attention->x + cos(rot) * attention->y;
        }
        else{   //move legs "towards" target position (step)
            absTarg->x =   - req->x  + cos(rot) * attention->x + sin(rot) * attention->y;
            absTarg->y =   - req->y  - sin(rot) * attention->x + cos(rot) * attention->y;
        }
        targ[leg].x = absTarg->x - joint->x;
        targ[leg].y = absTarg->y - joint->y;
        free(joint);
    }
    free(attention);
    free(absTarg);
}


/**
 * @brief assume_standardized_stance Positions the robot in a reliable, standardized
 * hard-coded stance.
 * @param current current position of the legs.
 */
void assume_standardized_stance(Point2D * current){
    
    float * z = (float *)calloc(NUM_LEGS, sizeof(float));
    z[LF] = GROUNDED + HIGH;
    z[RM] = GROUNDED + HIGH;
    z[LB] = GROUNDED + HIGH;
    z[RF] = GROUNDED;
    z[LM] = GROUNDED;
    z[RB] = GROUNDED;
    execute_position(current, z);

    Point2D * stdLeg = get_default_leg_position(LF);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);
    stdLeg = get_default_leg_position(RM);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);
    stdLeg = get_default_leg_position(LB);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);

    
    execute_position(current, z);
    
    z[LF] = GROUNDED;
    z[RM] = GROUNDED;
    z[LB] = GROUNDED;
    
    execute_position(current, z);
    z[RF] = GROUNDED + HIGH;
    z[LM] = GROUNDED + HIGH;
    z[RB] = GROUNDED + HIGH;
    
    execute_position(current, z);

    stdLeg = get_default_leg_position(RF);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);
    stdLeg = get_default_leg_position(LM);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);
    stdLeg = get_default_leg_position(RB);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);

    
    execute_position(current, z);
    z[RF] = GROUNDED;
    z[LM] = GROUNDED;
    z[RB] = GROUNDED;
    execute_position(current, z);
    free(z);
}


/**
 * @brief work_towards_goal takes the robot closer to a requested position and
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
float work_towards_goal(float rot, Point2D * goal, Point2D * current){
    Point2D * targ0 = calloc(NUM_LEGS, sizeof(Point2D));
    Point2D * targ1 = calloc(NUM_LEGS, sizeof(Point2D));

    float * scale = (float *)calloc(NUM_LEGS, sizeof(float));
    direct_legs(rot, targ0, current, goal, true);
    float scaledown0 = scale_legs(targ0, current, scale, true);

    direct_legs(rot, targ1, current, goal, false);
    float scaledown1 = scale_legs(targ1, current, scale, false);


    if (scaledown0 > scaledown1){
        execute_step(current, targ0, true);
    }
    else{
        execute_step(current, targ1, false);
    }

    free(targ0);
    free(targ1);
    free(scale);
    if (scaledown0 > scaledown1)
        return scaledown0;
    else
        return scaledown1;
}


/**
 * @brief rotate_set_small_angle rotates the robot a small angle, assumed to be at
 * sufficiently small that the robot legs should not be crossing across the
 * robot's body. Does not terminate until the full rotation has been achieved.
 * @param angle rotation the robot shall achieve.
 * @param current position the legs currently hold.
 * @return float indicating portion of angle remaining uncompleted.
 */
float rotate_set_small_angle(float angle, Point2D * current){
    float remaining = 1;
    Point2D * emptyGoal = (Point2D *)malloc(sizeof(Point2D));
    emptyGoal->x = 0;
    emptyGoal->y = 0;

    while (remaining > 0.2) {
        float remainingAngle = remaining * angle;
        remaining = remaining - work_towards_goal(remainingAngle, emptyGoal, current);
    }
    free(emptyGoal);
    
    return remaining;
}


/**
 * @brief rotate_set_angle keeps rotating until the robot has fully altered its
 * direction by the requested angle.
 * @param angle provides the angle the robot should rotate.
 * @param current position the legs currently hold.
 */
void rotate_set_angle(float angle, Point2D * current){
    assume_standardized_stance(current);

    while(angle > 1){
        float completion = rotate_set_small_angle(1, current);
        angle = angle - (1 - completion);
    }
    float remaining = angle * ( 1 - rotate_set_small_angle(angle, current));
    if (remaining > 0.1)
        rotate_set_small_angle(remaining, current);
}


/**
 * @brief main currently used for testing.
 * @param argc unused
 * @param argv unused
 * @return 0
 */
//int main(int argc, char *argv[]){
//    //testing variables
//    Point2D * current   = (Point2D *)calloc(NUM_LEGS, sizeof(Point2D));
//    Point2D * target    = (Point2D *)calloc(NUM_LEGS, sizeof(Point2D));
//    Point2D * command   = (Point2D *)malloc(sizeof(Point2D));
//    Point2D * diff      = (Point2D *)calloc(NUM_LEGS, sizeof(Point2D));
//    float scale[NUM_LEGS];
//    float rotation = 0;
//    for (int var = 0; var < 6; ++var) {
//        Point2D * temp = get_default_leg_position(var);
//        current[var].x = temp->x;
//        current[var].y = temp->y;
//    }
//    bool lrlRaised = false;
//    command->x = 1;
//    command->y = 0;
//
//
//    work_towards_goal(rotation, command, current);
//
//    /*
//    directLegs(rotation, target, current, & command, lrlRaised);
//    scaleLegs(target, current, scale, lrlRaised);
//
//
//    for (int index = 0; index < NUM_LEGS; ++index) {
//        current[index].x = target[index].x;
//        current[index].y = target[index].y;
//    }
//
//    for (int var = 0; var < 6; ++var) {
//        printf("diff x: %f y: %f, abs: %f \n", diff[var].x ,diff[var].y, dist(& diff[var]) );
//    }
//
//    printf("\nnew run starts here \n\n");
//
//    lrlRaised = true;
//
//    directLegs(rotation, target, current, & command, lrlRaised);
//    scaleLegs(target, current, scale, lrlRaised);
//
//
//    for (int var = 0; var < 6; ++var) {
//        printf("diff x: %f y: %f, abs: %f \n", diff[var].x ,diff[var].y, dist(& diff[var]) );
//    }*/
//
//    free(current);
//    free(target);
//    free(command);
//    free(diff);
//    current = NULL;
//    target  = NULL;
//    command = NULL;
//    diff    = NULL;
//    return 0;
//}
