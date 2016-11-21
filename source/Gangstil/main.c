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
const float FRONT_LEG_JOINT_Y = 0.06;
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
}Point2D;

struct Instruction{
    float speed;
    float step_length;
    float turn;
    Point2D targ;
};

struct Instruction instruction;
Point2D legs_current[6];   //0 = lf, 1 = rf, 2 = lm, 3 = rm, 4 = lb, 5 = rb
Point2D legs_target[6];
Point2D legs_default[6];


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


float dist(Point2D* vect){
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


void executeScaling(Point2D * targ, Point2D * curr, Point2D * diff, float * scale){

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
}


/*
 * returns scaling needed to not overstep boundry
*/
float boundryIntersect(Point2D * curr, Point2D * targ, float k, float m, bool upperLimit){

    if((targ->y - m> k * targ->x) != upperLimit)
        return 1; //no scaling needed; targ is within boundry

    printf("limit crossed\n");
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


void scaleLegs(Point2D * targ, Point2D * curr, Point2D * diff, float * scale ){

    for(size_t index = 0; index < NUM_LEGS; ++index){
        diff[index].x = targ[index].x - curr[index].x;
        diff[index].y = targ[index].y - curr[index].y;

        float targLength = dist( & targ[index]);
        scale[index] = 1;       //if leg within bounds, no change is necessary => scaledown = 1

        if (targLength > MAX_DIST)
            scale[index] = scaleToRangeBounds(targLength, dist(& curr[index]), dist(& diff[index]), MAX_DIST);
    }

    executeScaling(targ, curr, diff, scale);

    for (size_t index = 0; index < NUM_LEGS; ++index)
        scale[index] = 1;

    scaleToStraightBounds(scale, targ, curr);

    executeScaling(targ, curr, diff, scale);

    scaleToStraightBounds(scale, targ, curr);
}


void directLegs(float rot, Point2D * targ, Point2D * req, bool lrlRaised){
    for(size_t index = 0; index < NUM_LEGS; ++index){
        Point2D attention;
        attention.x = defaultLegPosition(index).x + jointPosition(index).x;
        attention.y = defaultLegPosition(index).y + jointPosition(index).y;
        Point2D absTarg;

        if (lrlRaised == (index == 0 || index == 3 || index == 4)){ //move legs "away" from position (body towards)
            absTarg.x =  req->x + cos(rot) * attention.x - sin(rot) * attention.y;
            absTarg.y =  req->y + sin(rot) * attention.x + cos(rot) * attention.y;
        }
        else{   //move legs "towards" target position (step)
            absTarg.x =   - req->x  + cos(rot) * attention.x + sin(rot) * attention.y;
            absTarg.y =   - req->y  - sin(rot) * attention.x + cos(rot) * attention.y;
        }
        targ[index].x = absTarg.x - jointPosition(index).x;
        targ[index].y = absTarg.y - jointPosition(index).y;

    }
}

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

    directLegs(rotation,target,&command, lrlRaised);

    scaleLegs(target, current, diff, scale);


    for (int index = 0; index < NUM_LEGS; ++index) {
        current[index].x = target[index].x;
        current[index].y = target[index].y;
    }

    for (int var = 0; var < 6; ++var) {
        printf("diff x: %f y: %f, abs: %f \n", diff[var].x ,diff[var].y, dist(& diff[var]) );
    }

    printf("\nnew run starts here \n\n");

    lrlRaised = true;

    directLegs(rotation, target, & command, lrlRaised);
    scaleLegs(target, current, diff, scale);


    for (int var = 0; var < 6; ++var) {
        printf("diff x: %f y: %f, abs: %f \n", diff[var].x ,diff[var].y, dist(& diff[var]) );
    }

    return 0;
}
