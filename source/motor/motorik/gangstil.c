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
const float HIGH                        = 0.03;
const float GROUNDED                    = -0.14;
const float MAX_DIST                    = 0.14;
const float VERT_MID_LEG_BORDER_OFFSET  = 0.06;
const float VERT_HEAD_LEG_BORDER_OFFSET = -0.03;
const float HORIZ_BORDER_TILT           = 0;
const float DIAG_DIVISIVE_BORDER_TILT   = 1.3333333;
const float CLOSE_BORDER_OFFSET         = 0.085;
const float DIAG_DIVISIVE_BORDER_OFFSET = 0.045;
const float CLOSE_BORDER_TILT           = -1;
const int   SMOOTH_STEP_ITERATIONS      = 5;
const float DEFAULT_LEG_DISTANCE = 0.09;
const float RELIABLY_EXECUTABLE_ROTATION = 0.2;
const float STRICT_ROTATION_MARGIN_OF_ERROR = 0.1;
const float STANDUP_LEG_DISTANCE = 0.16;

const uint16_t STANDARD_THRESHOLD = 100;
const uint16_t HIGH_PRECISION_THRESHOLD = 20;

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
 * @brief closef bool assessing wether two float values are close to each
 * other (within +- 0.001 of eac other)
 * @param a first float for examination
 * @param b second float for examination
 * @return true if a = b +- 0.001, else false.
 */
bool closef(float a, float b){
    return (a + 0.001 > b && b + 0.001 > a);
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
 * @brief maxf returns the larger of two float values.
 * @param a first float value, see @brief.
 * @param b second float value, see @brief.
 * @return the greater value of @param a and @param b.
 */
float maxf(float a, float b){
    if (a > b)
        return a;
    return b;
}


/**
 * @brief rotate_point_by_angle rotates a Point2D in the 2D plane by a passed angle.
 * @param original Point2D to be rotated.
 * @param angle float value representing angle of rotation.
 * @return point after rotation, expressed as a Point2D.
 */
Point2D rotate_point_by_angle(Point2D original, float angle)
{
	Point2D result;
	result.x = cos(angle)*original.x - sin(angle)*original.y;
	result.y = sin(angle)*original.x + cos(angle)*original.y;

	return result;
}


//Frans, please document your code /Olav
Point2D robot_to_ik_coords(Point2D original, size_t leg)
{
	Point2D result = original;

	if(leg == RF)
	{
		return rotate_point_by_angle(result, M_PI / 4);
	}
	else if(leg == RM)
	{
		return rotate_point_by_angle(result, M_PI / 2);
	}
	else if(leg == RB)
	{
		return rotate_point_by_angle(result, 3 * M_PI / 4);
	}
	else if(leg == LF)
	{
		return rotate_point_by_angle(result, -M_PI / 4);
	}
	else if(leg == LM)
	{
		return rotate_point_by_angle(result, -M_PI / 2);
	}
	else if(leg == LB)
	{
		return rotate_point_by_angle(result, -3 * M_PI / 4);
	}

	return original;
}


/**
 * @brief get_angle_set produces an array of the leg angles as calculated by the IK.
 * @param target provides the coordinates relative to the joints for all the legs, as 
 * they should be after movement.
 * @return array ordered LF RF LM RM LB RB (left/right - front/mid/back) of calculated 
 * angles for the legs.
 */
struct Leg* get_angle_set(Point2D * target, float * height){
    struct Leg* res = (struct Leg *)malloc(NUM_LEGS * sizeof(struct Leg));
    for (size_t leg = 0; leg < NUM_LEGS; ++leg){
		Point2D target_robot_coords = robot_to_ik_coords(target[leg], leg);

        res[leg] = leg_ik(target_robot_coords.x, height[leg], target_robot_coords.y);
    }

	return res;
}


/**
 * @brief takes a target set of leg positions and causes the servos to execute them.
 * @param target set of foot positions arranged LF RF LM RM LB RB, indicating 
 * intended final placement of feet relative to the mounts of their joints.
 */
void execute_position(Point2D * target, float * z, uint16_t threshold){
    struct Leg* ik = get_angle_set(target, z);

    uint16_t angles[3];
    uint8_t legId;
    for (size_t leg = 0; leg < NUM_LEGS; ++leg){
		if ((leg & 1) == 0){
            angles[0] = (uint16_t)(0x1ff - radian_to_servo(ik[leg].angle1));
            angles[1] = (uint16_t)(0x1ff - radian_to_servo(ik[leg].angle2));
            angles[2] = (uint16_t)(0x1ff - radian_to_servo(ik[leg].angle3));
            legId = (uint8_t)(leg/2);
        }
        else{
            angles[0] = (uint16_t)(0x1ff - radian_to_servo(ik[leg].angle1));
            angles[1] = (uint16_t)(0x1ff + radian_to_servo(ik[leg].angle2));
            angles[2] = (uint16_t)(0x1ff + radian_to_servo(ik[leg].angle3));
            legId = (uint8_t)(leg/2 + 3);
		}

#ifdef IS_X86
		current_servo_state.points[legId] = target[legId];
		current_servo_state.heights[legId] = z[legId];
		current_servo_state.angles[legId] = ik[legId];
#endif
        set_leg_angles(legId, angles);
    }

    send_servo_action(threshold);

#ifdef IS_X86
	write_current_state();
#endif

	free(ik);
}


/**
	Returns a vector of the distance between 2 point2d objects
 */
Point2D vector_between_points(Point2D current, Point2D target)
{
	Point2D result;

	result.x = target.x - current.x;
	result.y = target.y - current.y;

	return result;
}

Point2D divide_point2D(Point2D vector, float divisor)
{
	Point2D result;
	result.x = vector.x/divisor;
	result.y = vector.y/divisor;

	return result;
}

Point2D add_point2D(Point2D point1, Point2D point2)
{
	Point2D result;
	result.x = point1.x + point2.x;
	result.y = point1.y + point2.y;
	return result;
}


/**
 * @brief execute_step adjusts legs as appropriate to transition from one position to
 * another, as executable in a single step.
 * @param current position legs are in at start of function call. Is set to passed
 * target position upon completion of the step.
 * @param target position robot should have assumed upon completion of the step.
 * @param lrlRaised decides which set of legs should be used for stepping forward, and
 * which should remain grounded.
 */
void execute_step(Point2D * current, Point2D * target, bool lrlRaised){
    if (current->x == NAN || current->y == NAN || target->x == NAN || target->y == NAN)
    {
		free(current);
		current = raise_to_default_position();
    }
	
	float z[NUM_LEGS];

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

	const uint8_t STEP_AMOUNT = 2;

	Point2D intermediate_positions[NUM_LEGS];
	Point2D step_lengths[NUM_LEGS];
    for (size_t leg = 0; leg < NUM_LEGS; ++leg) 
	{
		intermediate_positions[leg] = current[leg];

		Point2D difference = vector_between_points(current[leg], target[leg]);
		step_lengths[leg] = divide_point2D(difference, STEP_AMOUNT);
	}
	
	bool is_first_step = true;

	for(uint8_t step = 0; step < STEP_AMOUNT; step++)
	{
		for (size_t leg = 0; leg < NUM_LEGS; ++leg) 
		{
			intermediate_positions[leg] = 
				add_point2D(intermediate_positions[leg], step_lengths[leg]);
		}

		uint8_t threshold = STANDARD_THRESHOLD;
		if(is_first_step == true)
		{
			threshold = HIGH_PRECISION_THRESHOLD;
		}
		is_first_step = false;
		execute_position(intermediate_positions, z, threshold);
	}



    z[LF] = GROUNDED;
    z[RM] = GROUNDED;
    z[LB] = GROUNDED;
    z[RF] = GROUNDED;
    z[LM] = GROUNDED;
    z[RB] = GROUNDED;
    execute_position(target, z, STANDARD_THRESHOLD);

    for (size_t leg = 0; leg < NUM_LEGS; ++leg) {
        current[leg].x = target[leg].x;
        current[leg].y = target[leg].y;
    }
}


/**
 * @brief get_default_leg_position gives a default position for requested leg.
 * @param leg indicates what leg of the robot (LF, RF, LM, RM, LB, RB) should be returned.
 * @return a standardised leg position, relative to joint.
 */
Point2D get_default_leg_position(size_t leg){
	float distance_from_body = DEFAULT_LEG_DISTANCE;
	
	return get_leg_position_from_radius(leg, distance_from_body, 0.02);
}


Point2D get_leg_position_from_radius
			(size_t leg, float distance_from_body, float outer_leg_x_offset){
    Point2D res;
    if (leg < 2){   //front
        res.x = distance_from_body / sqrt(2) + outer_leg_x_offset;
        res.y = distance_from_body / sqrt(2);
    }
    else if (leg < 4){  //mid
        res.x = 0;
        res.y = distance_from_body;
    }
    else{//back
        res.x = -distance_from_body / sqrt(2) - outer_leg_x_offset;
        res.y = distance_from_body / sqrt(2);
    }    

    if (leg % 2 == 1)  //right
        res.y = -res.y ;
    return res;
}


/**
 * @brief joint_position gives the horizontal position of requested leg-robot joint
 * @param leg indicates what leg joint of the robot (LF, RF, LM, RM, LB, RB) should be returned.
 * @return a standardised joint position, relative to robot center.
 */
Point2D joint_position(size_t leg){
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
	{
        res.y = -res.y;
	}
    return res;
}


/**
 * @brief dist returns the absolute distance of a point-represented vector.
 * @param vect point to which absolute distance from origo is to be returned.
 * @return absolute distance to entered point from origo.
 */
float dist(Point2D * vect){
    return sqrtf((vect->x * vect->x) + (vect->y * vect->y));
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
    if (targLength <= MAX_DIST || absf(diffLength) < 0.0001)
        return 1; //no scaling down needed

    float alpha = acos((powf(diffLength, 2) + powf(currLength, 2) - powf(targLength, 2))//no n/0 since diffLength & currLength > 0 if statement is entered
                       / (2 * diffLength * currLength));    // a = acos ((B2 + C2 - A2)/2BC), cosine trig formula
	float max_dist = MAX_DIST;
    float beta = asin(currLength * sin(alpha) / max_dist);     // b = asin (B * sin(a)/A ) // sin(b)/B = sin(a)/A, sine trig formula
    float gamma = M_PI - alpha - beta;                      //sum internal angles = PI
    float optimalDiff  = absf(MAX_DIST * sin(gamma) / sin(alpha));  //sin(alpha)never 0, if current leg pos legal and statement entered.
                                                                 //C = A * sin(c) / sin(a),
                                                                 //sine trig formula (see above)
                                                                 //absf(...)eliminates errors from asin simplification of angles
    return optimalDiff/diffLength;       //scale multiplier for optimal diff
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

    if((targ->y - m> k * targ->x) != upperLimit){
        return 1; //no scaling needed; targ is within boundry
    }

    if (closef(targ->x, curr->x)){//vertical leg movement
        float diff = (curr->x * k) + m - curr->y;
        float scale =absf(diff/(targ->y - curr->y));
        return scale;//no div by zero; that would mean legs do not move
    }

    float k2 = (targ->y - curr->y)/(targ->x - curr->x); //no div by zero; see preceeding if statement
    float m2 = curr->y - (k2 * curr->x);

    if (closef(k, k2))
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
    Point2D diff[NUM_LEGS];
    for(size_t index = 0; index < NUM_LEGS; ++index){
        diff[index].x = targ[index].x - curr[index].x;
        diff[index].y = targ[index].y - curr[index].y;

        float targLength = dist( & targ[index]);

        scale[index] = scale_to_range_bounds(targLength, dist(& curr[index]), dist(& diff[index]));
    }

    float res = update_targ_by_scale(targ, curr, diff, scale, lrlRaised);

    scale_to_straight_bounds(scale, targ, curr);

    res = res * update_targ_by_scale(targ, curr, diff, scale, lrlRaised);

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
void direct_legs(float rot, Point2D * targ, Point2D * current, Point2D req, bool lrlRaised){
    Point2D legRelativeRobotMid;

    for(size_t leg = LF; leg < NUM_LEGS; ++leg){
        Point2D joint = joint_position(leg);
        legRelativeRobotMid.x = current[leg].x + joint.x;
        legRelativeRobotMid.y = current[leg].y + joint.y;


        if (lrlRaised == (leg == 0 || leg == 3 || leg == 4)){ //move legs "away" from position (body towards)
            targ[leg].x =  req.x + cos(rot) * legRelativeRobotMid.x - sin(rot) * legRelativeRobotMid.y;
            targ[leg].y =  req.y + sin(rot) * legRelativeRobotMid.x + cos(rot) * legRelativeRobotMid.y;
        }
        else{   //move legs "towards" target position (step)
            targ[leg].x =   - req.x  + cos(rot) * legRelativeRobotMid.x + sin(rot) * legRelativeRobotMid.y;
            targ[leg].y =   - req.y  - sin(rot) * legRelativeRobotMid.x + cos(rot) * legRelativeRobotMid.y;
        }
        targ[leg].x = targ[leg].x - joint.x;
        targ[leg].y = targ[leg].y - joint.y;
    }

}
/**
 * @brief assume_standardized_stance Positions the robot in the default position by moving
 * 3 legs at a time.
 * @param current current position of the legs.
 */
void assume_standardized_stance(Point2D * current){
    float z[NUM_LEGS];
    z[LF] = GROUNDED + HIGH;
    z[RM] = GROUNDED + HIGH;
    z[LB] = GROUNDED + HIGH;
    z[RF] = GROUNDED;
    z[LM] = GROUNDED;
    z[RB] = GROUNDED;
    //execute_position(current, z);

    Point2D stdLeg = get_default_leg_position(LF);
    current[LF].x = stdLeg.x;
    current[LF].y = stdLeg.y;
    stdLeg = get_default_leg_position(RM);
    current[RM].x = stdLeg.x;
    current[RM].y = stdLeg.y;
    stdLeg = get_default_leg_position(LB);
    current[LB].x = stdLeg.x;
    current[LB].y = stdLeg.y;

    
    execute_position(current, z, STANDARD_THRESHOLD);
    
    z[LF] = GROUNDED;
    z[RM] = GROUNDED;
    z[LB] = GROUNDED;
    
    execute_position(current, z, STANDARD_THRESHOLD);
    z[RF] = GROUNDED + HIGH;
    z[LM] = GROUNDED + HIGH;
    z[RB] = GROUNDED + HIGH;
    
    execute_position(current, z, STANDARD_THRESHOLD);

    stdLeg = get_default_leg_position(RF);
    current[RF].x = stdLeg.x;
    current[RF].y = stdLeg.y;

    stdLeg = get_default_leg_position(LM);
    current[LM].x = stdLeg.x;
    current[LM].y = stdLeg.y;

    stdLeg = get_default_leg_position(RB);
    current[RB].x = stdLeg.x;
    current[RB].y = stdLeg.y;

    
    execute_position(current, z, STANDARD_THRESHOLD);
    z[RF] = GROUNDED;
    z[LM] = GROUNDED;
    z[RB] = GROUNDED;
    execute_position(current, z, STANDARD_THRESHOLD);
}


//Again, Frans, please document your code. 
//no u
Point2D* raise_to_default_position()
{
	//The position of the foot above the body when spreading the legs
	const float HEIGHT_ABOVE_BODY = 0.03;
	
	//Allocate memory for current positions
	Point2D* current_leg_positions = malloc(NUM_LEGS * sizeof(Point2D));

	//The current height above the body for all the legs
	float height[NUM_LEGS];
	//Get the default position and raise the legs above it
	for(size_t i = 0; i < NUM_LEGS; i++)
	{
		current_leg_positions[i] = get_leg_position_from_radius
			(i, STANDUP_LEG_DISTANCE, 0);
	}

	float  heights[4] = {HEIGHT_ABOVE_BODY, 0, -0.07, GROUNDED};

	for(size_t i = 0; i < 4; ++i)
	{
		for(size_t leg_id = 0; leg_id < NUM_LEGS; ++leg_id)
		{
			height[leg_id] = heights[i];
		}
		execute_position(current_leg_positions, height, STANDARD_THRESHOLD);
	}

	assume_standardized_stance(current_leg_positions);

	return current_leg_positions;
}


/**
 * @brief scale_goal scales goal coordinates to create a 1-distance vector,
 * and returns the lesser of original goal vector length and 1.
 * @param goal Point2D representing intended final position after a movement.
 * @return lesser of 1 and original length of goal vector.
 */
float scale_goal(Point2D * goal){
    float distance = dist(goal);
    if (goal->x != 0)
        goal->x = goal->x / distance;
    if (goal->y != 0)
        goal->y = goal->y / distance;
    return minf(1, distance);
}


/**
 * @brief work_towards_goal takes the robot closer to a requested position and
 * rotation, stepping with the set of legs that best accomplishes this.
 *
 * Movement of the robot (though not rotation) can be scaled down from best
 * possible by sending a goal with distance lesser than 1
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
float work_towards_goal(float rot, Point2D goal, Point2D * current){
    float requestedDownscale = scale_goal(& goal); //allows for detailed steering with small joystick tilts.

    if (goal.x == 0 && goal.y == 0 && rot == 0){
        return 1;   //no movement
	}
	
    Point2D targ[NUM_LEGS];
    float scale[NUM_LEGS];

    //printf("Working towards goal \n");

    direct_legs(rot, targ, current, goal, true);
    float scaledown0 = scale_legs(targ, current, scale, true);

    direct_legs(rot, targ, current, goal, false);
    float scaledown1 = scale_legs(targ, current, scale, false);

    float bestscale = maxf(scaledown0, scaledown1) * 0.9;
    if (bestscale < 0.001){
    	return bestscale; //too little movement to be relevant executing
    }

    bool lrlRaised = scaledown0 > scaledown1;

    rot = rot * bestscale;      //note: requested downscale dependant on joystick tilt; unrelated to rotation
    goal.x = goal.x * bestscale * requestedDownscale;
    goal.y = goal.y * bestscale * requestedDownscale;

    direct_legs(rot, targ, current, goal, lrlRaised);


	////spi_set_interrupts(false);
    execute_step(current, targ, lrlRaised);
	////spi_set_interrupts(true);
    return maxf(scaledown0, scaledown1);
}


/**
 * @brief rotate_set_angle keeps rotating until the robot has fully altered its
 * direction by the requested angle. standardizes legs before and after rotation.
 * @param angle provides the angle the robot should rotate.
 * @param current position the legs currently hold.
 */
void rotate_set_angle(float angle, Point2D * current){

    assume_standardized_stance(current);

    Point2D emptyGoal;
    emptyGoal.x = 0;
    emptyGoal.y = 0;

    while(angle > RELIABLY_EXECUTABLE_ROTATION){
        angle = angle - (RELIABLY_EXECUTABLE_ROTATION * work_towards_goal(RELIABLY_EXECUTABLE_ROTATION,emptyGoal, current));
    }

    while(angle < -RELIABLY_EXECUTABLE_ROTATION){
        angle = angle + (RELIABLY_EXECUTABLE_ROTATION * work_towards_goal(-RELIABLY_EXECUTABLE_ROTATION,emptyGoal, current));
    }

    while(angle > STRICT_ROTATION_MARGIN_OF_ERROR){
        angle = angle - (angle * work_towards_goal(angle, emptyGoal, current));
    }


    while(angle < -STRICT_ROTATION_MARGIN_OF_ERROR){
        angle = angle - (angle * work_towards_goal(angle, emptyGoal, current));
    }

    assume_standardized_stance(current);
}

