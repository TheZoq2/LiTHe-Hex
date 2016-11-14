#include <stdio.h>
#include "ik.h"

int main() {
    struct Leg leg = leg_ik(30, 0, 30);
    printf("angle1 = %f\n", leg.angle1);
    printf("angle2 = %f\n", leg.angle2);
    printf("angle3 = %f\n", leg.angle3);
}
