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

const float FRONT_LEG_JOINT_X = 0.12;
const float FRONT_LEG_JOINT_Y = 0.6;
const float MID_LEG_JOINT_Y = 0.1;
const float HIGH        = 0.1;//placeholder value; adjust
const float GROUNDED    = 0;
const float MIN_DIST    = 0.06;
const float MAX_DIST    = 0.18;
const float VERT_MID_LEG_BORDER_OFFSET = 0.06;
const float VERT_HEAD_LEG_BORDER_OFFSET = -0.03;
const float HORIZ_BORDER_TILT = 0;
const float DIAG_DIVISIVE_BORDER_TILT = 1.3333333;
const float CLOSE_BORDER_OFFSET = 0.085;
const float DIAG_DIVISIVE_BORDER_OFFSET = 0.045;
const float CLOSE_BORDER_TILT = -1;

const size_t NUM_LEGS   = 6;


typedef struct{
    float x;
    float y;
    float z;
}Point;

typedef struct{
    float x;
    float y;
}Point2D;

struct Instruction{
    float speed;
    float step_length;
    float turn;
    Point2D targ;
};

struct Instruction instruction;
Point legs_current[6];   //0 = lf, 1 = rf, 2 = lm, 3 = rm, 4 = lb, 5 = rb
Point legs_target[6];
Point legs_default[6];

bool lrl_feet = true;               //true = lrl, false = rlr next for use


Point defaultLegPosition(size_t leg){
    Point res;
    res.z = 0;
    if (leg < 2)   //front
        res.x = 0.1;
    else if (leg < 4)  //mid
        res.x = 0;
    else
        res.x = -0.1;    //back

    if ((leg & 1) == 0)  //left
        res.y = 0.1;
    else
        res.y = -0.1;    //right
    return res;
}

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

float absf(float a){
    if(a < 0)
        return -a;
    return a;
}


void atAttention(){ //hardcoded function to set robot in a default position.
    legs_target[0].z = 0.1;
    legs_target[3].z = 0.1;
    legs_target[4].z = 0.1;
    //todo: execute
    legs_target[0] = defaultLegPosition(0);
    legs_target[0].z = 0.1;
    legs_target[3] = defaultLegPosition(3);
    legs_target[3].z = 0.1;
    legs_target[4] = defaultLegPosition(4);
    legs_target[4].z = 0.1;
    //todo: execute
    legs_target[0].z = GROUNDED;
    legs_target[3].z = GROUNDED;
    legs_target[4].z = GROUNDED;
    //todo: execute

    legs_target[1].z = 0.1;
    legs_target[2].z = 0.1;
    legs_target[5].z = 0.1;
    //todo: execute
    legs_target[1] = defaultLegPosition(1);
    legs_target[1].z = 0.1;
    legs_target[2] = defaultLegPosition(2);
    legs_target[2].z = 0.1;
    legs_target[5] = defaultLegPosition(5);
    legs_target[5].z = 0.1;
    //todo: execute
    legs_target[1].z = GROUNDED;
    legs_target[2].z = GROUNDED;
    legs_target[5].z = GROUNDED;
    //todo: execute
}


float dist(Point * vect){
    return (float)(sqrt((vect->x * vect->x) + (vect->y * vect->y)));
}


float dist2(Point2D * vect){
    return (float)(sqrt((vect->x * vect->x) + (vect->y * vect->y)));
}


float scaleToRangeBounds(float targLength, float currLength, float diffLength, float limit){
    float alpha = acos((powf(diffLength, 2) + powf(currLength, 2) - powf(targLength, 2))//no n/0 since diffLength & currLength > 0 if statement is entered
                       / (2 * diffLength * currLength));    // a = acos ((B2 + C2 - A2)/2BC), cosine trig formula
    float beta = asin(currLength * sin(alpha) / limit);     // b = asin (B * sin(a)/A ) // sin(b)/B = sin(a)/A, sine trig formula
    float gamma = M_PI - alpha - beta;                      //sum internal angles = PI
    float optimalDiff  = absf(limit * sin(gamma) / sin(alpha));  //sin(alpha)never 0, if current leg pos legal and statement entered.
                                                                 //C = A * sin(c) / sin(a),
                                                                 //sine trig formula (see above)
                                                                 //absf(...)eliminates errors from asin simplification of angles
    return optimalDiff/diffLength;       //scale multiplier for optimal diff
}


float minf(float a, float b){
    if (a < b)
        return a;
    return b;
}


void executeScaling(Point * targ, Point * curr, Point2D * diff, float * scale){
    for (int var = 0; var < 6; ++var) {

    }

    float lrlScaledown = minf(minf(scale[0], scale[3]), scale[4]);
    float rlrScaledown = minf(minf(scale[1], scale[2]), scale[5]);
    printf("lrl scaledown: %f \n rlr scaledown: %f \n", lrlScaledown, rlrScaledown);
    for(size_t index = 0; index < NUM_LEGS; ++index){
        float scale = lrlScaledown;
        if (index == RF || index == LM || index == RB)
            scale = rlrScaledown;
        diff[index].x = diff[index].x * scale;              //update diff
        diff[index].y = diff[index].y * scale;
        targ[index].x = curr[index].x + diff[index].x;       //update target
        targ[index].y = curr[index].y + diff[index].y;
    }
}


/*
 * returns scaling needed to not overstep boundry
*/
float boundryIntersect(Point * curr, Point * targ, float k, float m, bool upperLimit){

    if((targ->y - m> k * targ->x) != upperLimit)
        return 1; //no scaling needed; targ is within boundry

    printf("limit crossed\n");
    if (targ->x == curr->x){    //vertical leg movement
        float diff = (curr->x * k) + m - curr->y;
        float scale =absf(diff/(targ->y - curr->y));
        printf("vertical intersection; scale set to %f \n");
        return scale;//no div by zero; that would mean legs do not move
    }

    float k2 = (targ->y - curr->y)/(targ->x - curr->x); //no div by zero; see preceeding if statement
    float m2 = curr->y - (k2 * curr->x);

    if (k == k2)
        printf("parallell\n");
    if (k == k2)
        return 1;   //parallel lines; no scaling. Should not realistically happen.

    float newX = (m - m2)/(k2 - k); //no div by zero; see preceeding if statement
    float scale =(newX - curr->x) / (targ->x - curr->x);
    printf("diagonal intersection; scale set to %f , k: %f, m: %f, upper limit: %s\n", scale, k, m, upperLimit ? "true" : "false");
    return scale;  //no div by zero; see prior if statement
}


void scaleToStraightBounds(float * scale, Point * targ, Point * curr){
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


void scaleLegs(Point * targ, Point * curr, Point2D * diff, float * scale ){

    for(size_t index = 0; index < NUM_LEGS; ++index){
        diff[index].x = targ[index].x - curr[index].x;
        diff[index].y = targ[index].y - curr[index].y;

        float targLength = dist( & targ[index]);
        scale[index] = 1;       //if leg within bounds, no change is necessary => scaledown = 1

        if (targLength > MAX_DIST)
            scale[index] = scaleToRangeBounds(targLength, dist(& curr[index]), dist2(& diff[index]), MAX_DIST);
    }

    executeScaling(targ, curr, diff, scale);

    for (size_t index = 0; index < NUM_LEGS; ++index)
        scale[index] = 1;

    scaleToStraightBounds(scale, targ, curr);

    executeScaling(targ, curr, diff, scale);

    scaleToStraightBounds(scale, targ, curr);
    for (int var = 0; var < 6; ++var) {
        if(scale[var] != 1)
            printf("imperfect scaling in leg %d with value %f \n", var, scale[var]);
    }
}


void directLegs(float rot, Point * targ, Point2D * req){
    for(size_t index = 0; index < NUM_LEGS; ++index){
        Point attention;
        attention.x = defaultLegPosition(index).x + jointPosition(index).x;
        attention.y = defaultLegPosition(index).y + jointPosition(index).y;
        Point absTarg;
        if (targ[index].z == 0){ //move legs "away" from position (body towards)
            absTarg.x =  - req->x + cos(rot) * attention.x + sin(rot) * attention.y;
            absTarg.y =  - req->y - sin(rot) * attention.y + cos(rot) * attention.y;
        }
        else{   //move legs "towards" target position (step)
            absTarg.x =    req->x  + cos(rot) * attention.x - sin(rot) * attention.y;
            absTarg.y =    req->y  + sin(rot) * attention.x + cos(rot) * attention.y;
        }
        targ[index].x = absTarg.x - jointPosition(index).x;
        targ[index].y = absTarg.y - jointPosition(index).y;

    }
}


void instruction_decode(struct Instruction command){
    Point2D legs_diff[NUM_LEGS];
    float scaledown[NUM_LEGS];
    float rot = command.turn * command.step_length;

    for(size_t index = 0; index < NUM_LEGS; ++index){

        if (lrl_feet == (index == LF || index == RM || index == LB))
            legs_target[index].z = HIGH;    //move legs this sequence
        else
            legs_target[index].z = GROUNDED;       //keep legs grounded.

        //set desired target position

        directLegs(rot, legs_target, & command.targ);
    }

    scaleLegs(legs_target, legs_current, legs_diff, scaledown);
    //leg movement has been scaled propperly

}


int main(int argc, char *argv[]){
    //testing variables
    Point current[NUM_LEGS];
    Point target[NUM_LEGS];
    Point2D command;
    Point2D diff[NUM_LEGS];
    float scale[NUM_LEGS];
    float rotation = 1;
    for (int var = 0; var < 6; ++var) {
        current[var] = defaultLegPosition(var);
        target[var].z = 0;
    }
    target[1].z = 0.1;
    target[2].z = 0.1;
    target[5].z = 0.1;
    command.x = 0;
    command.y = 0;


    directLegs(rotation,target,&command);

    scaleLegs(target, current, diff, scale);
    for (int var = 0; var < 6; ++var) {
        //printf("target x: %f y: %f \n", target[var].x ,target[var].y );
        printf("diff x: %f y: %f \n", diff[var].x ,diff[var].y );
    }

    printf("%f", DIAG_DIVISIVE_BORDER_TILT);

    return 0;
}
