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


Point2D legs_target[6];


/**
 * @brief defaultLegPosition gives a default position for requested leg.
 * @param leg indicates what leg of the robot (LF, RF, LM, RM, LB, RB) should be returned.
 * @return a standardised leg position, relative to joint.
 */
Point2D defaultLegPosition(size_t leg){
    Point2D res;
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
Point2D jointPosition(size_t leg){
    Point2D res;
    if (leg < 2){   //front
        res.x = FRONT_LEG_JOINT_X;
        res.y = FRONT_LEG_JOINT_Y;
    }
    else if (leg < 4){  //mid
        res.x = 0;
        res.y = MID_LEG_JOINT_Y;
    }
    else{
        res.x = -FRONT_LEG_JOINT_X;
        res.y = FRONT_LEG_JOINT_Y;
    }    //back

    if ((leg & 1) == 1)  //right
        res.y = -res.y ;
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
 * @brief atAttention deprecated, puts robot into a reliable, hard-coded stance.
 */
void atAttention(){
    float legHeight[NUM_LEGS];

    legHeight[LF] = HIGH;
    legHeight[RM] = HIGH;
    legHeight[LB] = HIGH;
    //todo: execute
    legs_target[LF] = defaultLegPosition(LF);
    legs_target[RM] = defaultLegPosition(RM);
    legs_target[LB] = defaultLegPosition(LB);
    //todo: execute
    legHeight[LF] = GROUNDED;
    legHeight[RM] = GROUNDED;
    legHeight[LB] = GROUNDED;
    //todo: execute

    legHeight[RF] = HIGH;
    legHeight[LM] = HIGH;
    legHeight[5] = HIGH;
    //todo: execute
    legs_target[RF] = defaultLegPosition(RF);
    legs_target[LM] = defaultLegPosition(LM);
    legs_target[5] = defaultLegPosition(5);
    //todo: execute
    legHeight[RF] = GROUNDED;
    legHeight[LM] = GROUNDED;
    legHeight[5] = GROUNDED;
    //todo: execute
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
float scaleLegs(Point2D * targ, Point2D * curr, Point2D * diff, float * scale, bool lrlRaised){

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
    for(size_t leg = LF; leg < NUM_LEGS; ++leg){
        Point2D attention;
        attention.x = /*defaultLegPosition(index).x*/current[leg].x + jointPosition(leg).x;
        attention.y = /*defaultLegPosition(index).y*/current[leg].y + jointPosition(leg).y;
        Point2D absTarg;

        if (lrlRaised == (leg == 0 || leg == 3 || leg == 4)){ //move legs "away" from position (body towards)
            absTarg.x =  req->x + cos(rot) * attention.x - sin(rot) * attention.y;
            absTarg.y =  req->y + sin(rot) * attention.x + cos(rot) * attention.y;
        }
        else{   //move legs "towards" target position (step)
            absTarg.x =   - req->x  + cos(rot) * attention.x + sin(rot) * attention.y;
            absTarg.y =   - req->y  - sin(rot) * attention.x + cos(rot) * attention.y;
        }
        targ[leg].x = absTarg.x - jointPosition(leg).x;
        targ[leg].y = absTarg.y - jointPosition(leg).y;
    }
}


/**
 * @brief main currently used for testing.
 * @param argc unused
 * @param argv unused
 * @return 0
 */
int main(int argc, char *argv[]){
    //testing variables
    Point2D current[NUM_LEGS];
    Point2D target[NUM_LEGS];
    Point2D command;
    Point2D diff[NUM_LEGS];
    float scale[NUM_LEGS];
    float rotation = 1;
    for (int var = 0; var < 6; ++var) {
        current[var] = defaultLegPosition(var);
    }
    bool lrlRaised = false;
    command.x = 0;
    command.y = 0;

    directLegs(rotation, target, current, & command, lrlRaised);
    scaleLegs(target, current, diff, scale, lrlRaised);


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
    scaleLegs(target, current, diff, scale, lrlRaised);


    for (int var = 0; var < 6; ++var) {
        printf("diff x: %f y: %f, abs: %f \n", diff[var].x ,diff[var].y, dist(& diff[var]) );
    }

    return 0;
}
