#include "gangstil.h"

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


/**
 * @brief getAngles produces an array of the leg angles as calculated by the IK.
 * @param target provides the coordinates relative to the joints for all the legs, as 
 * they should be after movement.
 * @return array ordered LF RF LM RM LB RB (left/right - front/mid/back) of calculated 
 * angles for the legs.
 */
struct Leg* getAngles(Point2D * target){
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
        y = -target[leg].z;
        res[leg] = leg_ik(x,y,z);
    }
    res[LF].angle1 = res[LF].angle1 + (M_PI / 4);
    res[RF].angle1 = res[RF].angle1 - (M_PI / 4);
    res[LB].angle1 = res[LB].angle1 - (M_PI / 4);
    res[RB].angle1 = res[RB].angle1 + (M_PI / 4);
}

/**
 * @brief defaultLegPosition gives a default position for requested leg.
 * @param leg indicates what leg of the robot (LF, RF, LM, RM, LB, RB) should be returned.
 * @return a standardised leg position, relative to joint.
 */
Point2D * defaultLegPosition(size_t leg){
    Point2D * res = (Point2D *)malloc(sizeof(Point2D));
    if (leg < 2){   //front
        res.x = 0.1;
        res.y = 0.1;
    }
    else if (leg < 4){  //mid
        res.x = 0;
        res.y = 0.14;
    }
    else{
        res.x = -0.1;
        res.y = 0.1;
    }    //back

    if ((leg & 1) == 1)  //right
        res.y = -res.y ;
    return res;
}


/**
 * @brief jointPosition gives the horizontal position of requested leg-robot joint
 * @param leg indicates what leg joint of the robot (LF, RF, LM, RM, LB, RB) should be returned.
 * @return a standardised joint position, relative to robot center.
 */
Point2D * jointPosition(size_t leg){
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
 * @brief scaleToRangeBounds Based on requested leg movement and current foot position
 * calculates how much the movement needs to be downscaled for the foot to land within
 * the maximum range of the leg.
 *
 * Only returns 1 (that is, implies no downscaling is needed)if the intended position
 * is already within the maximum reach of the leg.
 *
 * Note that scaleToRangeBounds is not sufficient to limit the movement of the legs;
 * further limitation (covered by scaleToStraightBounds) is needed to avoid going
 * under or into the robot with the legs, and to avoid legs trying to intersect with
 * each other.
 * @param targLength distance from joint to intended foot position (after movement).
 * @param currLength distance from joint to current foot position (before movement).
 * @param diffLength distance from current foot position (before movement) to intended
 * foot position (after movement).
 * @return float that, when intened leg movement is multiplied with it, reduces the
 * leg movement to where it comes within the maximum reach of the leg.
 */
float scaleToRangeBounds(float targLength, float currLength, float diffLength){
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
 * @brief updateTargByScale takes a set of required scaledowns for the available legs,
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
float updateTargByScale(Point2D * targ, Point2D * curr, Point2D * diff, float * scale, bool lrlRaised){

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
        printf("scaled grounded by %f\n", rlrScaledown);
    else
        printf("scaled grounded by %f\n", lrlScaledown);

    if (lrlRaised)
        return rlrScaledown;
    else
        return lrlScaledown;
}


/**
 * @brief boundryIntersect gives a scale multiplier needed to avoid crossing boundry,
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
float boundryIntersect(Point2D * curr, Point2D * targ, float k, float m, bool upperLimit){

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
 * @brief scaleToStraightBounds calls boundryIntersect for each leg, for a set of hard-coded values, for each leg setting scale to the lowest (worst) scaledown needed for that leg.
 * @param scale array of floats for storing scaledown needed for each leg.
 * @param targ intended movement vectors for feet.
 * @param curr current position of feet.
 */
void scaleToStraightBounds(float * scale, Point2D * targ, Point2D * curr){
    scale[LF] = minf(boundryIntersect(& curr[LF], & targ[LF], HORIZ_BORDER_TILT, VERT_HEAD_LEG_BORDER_OFFSET, false),
                minf(boundryIntersect(& curr[LF], & targ[LF], DIAG_DIVISIVE_BORDER_TILT, DIAG_DIVISIVE_BORDER_OFFSET, true),
                     boundryIntersect(& curr[LF], & targ[LF], CLOSE_BORDER_TILT, CLOSE_BORDER_OFFSET, false)));

    scale[RF] = minf(boundryIntersect(& curr[RF], & targ[RF], HORIZ_BORDER_TILT, -VERT_HEAD_LEG_BORDER_OFFSET, true),
                minf(boundryIntersect(& curr[RF], & targ[RF], -DIAG_DIVISIVE_BORDER_TILT, -DIAG_DIVISIVE_BORDER_OFFSET, false),
                     boundryIntersect(& curr[RF], & targ[RF], -CLOSE_BORDER_TILT, -CLOSE_BORDER_OFFSET, true)));

    scale[LM] = minf(boundryIntersect(& curr[LM], & targ[LM], DIAG_DIVISIVE_BORDER_TILT, -DIAG_DIVISIVE_BORDER_OFFSET, false),
                minf(boundryIntersect(& curr[LM], & targ[LM], HORIZ_BORDER_TILT, VERT_MID_LEG_BORDER_OFFSET, false),
                     boundryIntersect(& curr[LM], & targ[LM], -DIAG_DIVISIVE_BORDER_TILT, -DIAG_DIVISIVE_BORDER_OFFSET, false)));

    scale[RM] = minf(boundryIntersect(& curr[RM], & targ[RM], -DIAG_DIVISIVE_BORDER_TILT, DIAG_DIVISIVE_BORDER_OFFSET, true),
                minf(boundryIntersect(& curr[RM], & targ[RM], HORIZ_BORDER_TILT, -VERT_MID_LEG_BORDER_OFFSET, true),
                     boundryIntersect(& curr[RM], & targ[RM], DIAG_DIVISIVE_BORDER_TILT, DIAG_DIVISIVE_BORDER_OFFSET, true)));

    scale[LB] = minf(boundryIntersect(& curr[LB], & targ[LB], HORIZ_BORDER_TILT, VERT_HEAD_LEG_BORDER_OFFSET, false),
                minf(boundryIntersect(& curr[LB], & targ[LB], -DIAG_DIVISIVE_BORDER_TILT, DIAG_DIVISIVE_BORDER_OFFSET, true),
                     boundryIntersect(& curr[LB], & targ[LB], -CLOSE_BORDER_TILT, CLOSE_BORDER_OFFSET, false)));

    scale[RB] = minf(boundryIntersect(& curr[RB], & targ[RB], HORIZ_BORDER_TILT, -VERT_HEAD_LEG_BORDER_OFFSET, true),
                minf(boundryIntersect(& curr[RB], & targ[RB], DIAG_DIVISIVE_BORDER_TILT, -DIAG_DIVISIVE_BORDER_OFFSET, false),
                     boundryIntersect(& curr[RB], & targ[RB], CLOSE_BORDER_TILT, -CLOSE_BORDER_OFFSET, true)));
}


/**
 * @brief scaleLegs adjusts target position for the legs so that all final foot
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
float scaleLegs(Point2D * targ, Point2D * curr, float * scale, bool lrlRaised){
    Point2D * diff = (Point2D *)calloc(NUM_LEGS, sizeof(Point2D));
    for(size_t index = 0; index < NUM_LEGS; ++index){
        diff[index].x = targ[index].x - curr[index].x;
        diff[index].y = targ[index].y - curr[index].y;

        float targLength = dist( & targ[index]);

        scale[index] = scaleToRangeBounds(targLength, dist(& curr[index]), dist(& diff[index]));
    }

    float res = updateTargByScale(targ, curr, diff, scale, lrlRaised);

    printf("res is %f\n", res);
    scaleToStraightBounds(scale, targ, curr);

    res = res * updateTargByScale(targ, curr, diff, scale, lrlRaised);
    printf("res is %f\n", res);

    printf("\ntotal scaledown of grounded legs is %f \n\n", res);
    free(diff);
    return res;
}


/**
 * @brief directLegs calculates basic movements, asuming perfect leg freedom, for
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
void directLegs(float rot, Point2D * targ, Point2D * current, Point2D * req, bool lrlRaised){
    Point2D * attention = (Point2D *)malloc(sizeof(Point2D));
    Point2D * absTarg   = (Point2D *)malloc(sizeof(Point2D));

    for(size_t leg = LF; leg < NUM_LEGS; ++leg){
        Point2D * joint = jointPosition(leg);
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
 * @brief assumeStandardizedStance Positions the robot in a reliable, standardized
 * hard-coded stance.
 * @param current current position of the legs.
 */
void assumeStandardizedStance(Point2D * current){
    bool lrlRaised = true;
    bool rlrRaised = false;

    //todo: execute

    Point2D * stdLeg = defaultLegPosition(LF);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);
    stdLeg = defaultLegPosition(RM);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);
    stdLeg = defaultLegPosition(LB);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);

    //todo: execute;
    lrlRaised = false;
    //todo: execute;
    rlrRaised = true;
    //todo: execute;

    stdLeg = defaultLegPosition(RF);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);
    stdLeg = defaultLegPosition(LM);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);
    stdLeg = defaultLegPosition(RB);
    current->x = stdLeg->x;
    current->y = stdLeg->y;
    free(stdLeg);

    //todo: execute
    rlrRaised = false;
    //todo; execute
}


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
float workTowardsGoal(float rot, Point2D * goal, Point2D * current){
    Point2D * targ0 = calloc(NUM_LEGS, sizeof(Point2D));
    Point2D * targ1 = calloc(NUM_LEGS, sizeof(Point2D));

    float * scale = (float *)calloc(NUM_LEGS, sizeof(float));
    directLegs(rot, targ0, current, goal, true);
    float scaledown0 = scaleLegs(targ0, current, scale, true);

    directLegs(rot, targ0, current, goal, false);
    float scaledown1 = scaleLegs(targ0, current, scale, false);

    if (scaledown0 > scaledown1){
        //todo: execute with targ0
    }
    else{
        //todo: execute with targ1
    }

    free(targ0);
    free(targ1);
    free(scsle);
    if (scaledown0 > scaledown1)
        return scale0;
    else
        return scale1;
}


/**
 * @brief rotateSetSmallAngle rotates the robot a small angle, assumed to be at
 * sufficiently small that the robot legs should not be crossing across the
 * robot's body. Does not terminate until the full rotation has been achieved.
 * @param angle rotation the robot shall achieve.
 * @param current position the legs currently hold.
 * @return float indicating portion of angle remaining uncompleted.
 */
float rotateSetSmallAngle(float angle, Point2D * current){
    float remaining = 1;
    Point2D * emptyGoal = (Point2D *)malloc(sizeof(Point2D));
    emptyGoal->x = 0;
    emptyGoal->y = 0;

    while (remaining > 0.2) {
        float remainingAngle = remaining * angle;
        remaining = remaining - workTowardsGoal(remainingAngle, emptyGoal, current);
    }
    free(emptyGoal);
    
    return remaining;
}


/**
 * @brief rotateSetAngle keeps rotating until the robot has fully altered its
 * direction by the requested angle.
 * @param angle provides the angle the robot should rotate.
 * @param current position the legs currently hold.
 */
void rotateSetAngle(float angle, Point2D * current){
    assumeStandardizedStance(current);

    while(angle > 1){
        float completion = rotateSetSmallAngle(1, current);
        angle = angle - (1 - completion);
    }
    float remaining = angle * ( 1 - rotateSetSmallAngle(angle, current));
    if (remaining > 0.1)
        rotateSetSmallAngle(remaining, current);
}


/**
 * @brief main currently used for testing.
 * @param argc unused
 * @param argv unused
 * @return 0
 *//*
int main(int argc, char *argv[]){
    //testing variables
    Point2D * current   = (Point2D *)calloc(NUM_LEGS, sizeof(Point2D));
    Point2D * target    = (Point2D *)calloc(NUM_LEGS, sizeof(Point2D));
    Point2D * command   = (Point2D *)malloc(sizeof(Point2D));
    Point2D * diff      = (Point2D *)calloc(NUM_LEGS, sizeof(Point2D));
    float scale[NUM_LEGS];
    float rotation = 1;
    for (int var = 0; var < 6; ++var) {
        current[var] = defaultLegPosition(var);
    }
    bool lrlRaised = false;
    command->x = 0;
    command->y = 0;

    directLegs(rotation, target, current, & command, lrlRaised);
    scaleLegs(target, current, scale, lrlRaised);


    for (int index = 0; index < NUM_LEGS; ++index) {
        current[index].x = target[index].x;
        current[index].y = target[index].y;
    }

    for (int var = 0; var < 6; ++var) {
        printf("diff x: %f y: %f, abs: %f \n", diff[var].x ,diff[var].y, dist(& diff[var]) );
    }

    printf("\nnew run starts here \n\n");

    lrlRaised = true;

    directLegs(rotation, target, current, & command, lrlRaised);
    scaleLegs(target, current, scale, lrlRaised);


    for (int var = 0; var < 6; ++var) {
        printf("diff x: %f y: %f, abs: %f \n", diff[var].x ,diff[var].y, dist(& diff[var]) );
    }

    free(current);
    free(target);
    free(command);
    free(diff);
    current = NULL;
    target  = NULL;
    command = NULL;
    diff    = NULL;
    return 0;
}*/
