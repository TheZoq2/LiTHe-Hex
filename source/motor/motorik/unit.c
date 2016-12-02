#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "gangstil.h"
#include <stdbool.h>
#include "status.h"


/*
	To add a unit test: 

	Add another 
		START_TEST ($NAME)
		{
		}
		END_TEST
	block

	Inside the block, you can add ck_assert(boolean_value) for tings you
	want to test.

	Add
    tcase_add_test(tc_core, $NAME);

	in the motorik_test function.
*/

bool float_is_almost(float tested, float target)
{
	return (absf(tested-target) < 0.0001);
}

bool test_robot_to_ik(size_t leg, float x, float y, float expected_x, float expected_y)
{
	Point2D original;
	original.x = x;
	original.y = y;

	Point2D result = robot_to_ik_coords(original, leg);

	return (float_is_almost(result.x, expected_x)) && (float_is_almost(result.y, expected_y));
}

/**
 * Tests functions that are used to test other functions
 */
START_TEST (meta_test)
{
	ck_assert(float_is_almost(0, 1) == false);
	ck_assert(float_is_almost(0, 0.00000001) == true);
	ck_assert(float_is_almost(5, 5.00000001) == true);
	ck_assert(float_is_almost(5.00000001, 5) == true);
	ck_assert(float_is_almost(4.99999, 5) == true);
	ck_assert(float_is_almost(-5, 5) == false);
}
END_TEST

START_TEST (status_tests) {

    CurrentStatus status;

    status_set_speed(&status, 255, 127);
    status_set_rotation(&status, 0);
    status_set_servo_speed(&status, 255, 255);

    ck_assert(status.x_speed == 1.0);
    ck_assert(status.y_speed == 0.0);
    ck_assert(status.rotation == -1.0);
    
} 
END_TEST

START_TEST (ik_tests)
{
}
END_TEST

START_TEST (coordinate_conversion_tests)
{
	{
		Point2D original;
		original.x = 1;
		original.y = 0;
		float angle = M_PI/2;
		
		Point2D rotated = rotate_point_by_angle(original, angle);

		ck_assert(float_is_almost(rotated.x, 0));
		ck_assert(float_is_almost(rotated.y, 1));

		angle = M_PI / 4;
		rotated = rotate_point_by_angle(original, angle);
		ck_assert(float_is_almost(rotated.x, 1/sqrt(2)));
		ck_assert(float_is_almost(rotated.y, 1/sqrt(2)));
		angle = -M_PI / 4;
		rotated = rotate_point_by_angle(original, angle);
		ck_assert(float_is_almost(rotated.x, 1/sqrt(2)));
		ck_assert(float_is_almost(rotated.y, -1/sqrt(2)));
		angle = M_PI;
		rotated = rotate_point_by_angle(original, angle);
		ck_assert(float_is_almost(rotated.x, -1));
		ck_assert(float_is_almost(rotated.y, 0));

		original.x = 1;
		original.y = 1;
		angle = -M_PI / 4;
		rotated = rotate_point_by_angle(original, angle);
		ck_assert(float_is_almost(rotated.x, sqrt(2)));
		ck_assert(float_is_almost(rotated.y, 0));
	}

	//Testing the side legs
	{
		//Testing legs extended 'forward' in the IK system
		ck_assert(test_robot_to_ik(LF, 5, 5, 5 * sqrt(2), 0));
		ck_assert(test_robot_to_ik(LM, 0, 20, 20, 0));
		ck_assert(test_robot_to_ik(LB, -5, 5, 5 * sqrt(2), 0));

		ck_assert(test_robot_to_ik(RF, 5, -5, 5 * sqrt(2), 0));
		ck_assert(test_robot_to_ik(RM, 0, -20, 20, 0));
		ck_assert(test_robot_to_ik(RB, -5, -5, 5 * sqrt(2), 0));


		//Testing legs turned 45 degrees clockwise
		float expected_x = 5/sqrt(2);
		float expected_y = -5/sqrt(2);
		ck_assert(test_robot_to_ik(LF, 5, 0, expected_x, expected_y));
		ck_assert(test_robot_to_ik(LM, 5/sqrt(2), 5/sqrt(2),  expected_x, expected_y));
		ck_assert(test_robot_to_ik(LB, 0, 5, expected_x, expected_y));

		ck_assert(test_robot_to_ik(RF, 0, -5, expected_x, expected_y));
		ck_assert(test_robot_to_ik(RM, -5/sqrt(2), -5/sqrt(2), expected_x, expected_y));
		ck_assert(test_robot_to_ik(RB, -5, 0, expected_x, expected_y));

		expected_x = 5/sqrt(2);
		expected_y = 5/sqrt(2);
		ck_assert(test_robot_to_ik(LF, 0, 5, expected_x, expected_y));
		ck_assert(test_robot_to_ik(LB, -5, 0, expected_x, expected_y));
		ck_assert(test_robot_to_ik(RF, 5, 0, expected_x, expected_y));
		ck_assert(test_robot_to_ik(RB, 0, -5, expected_x, expected_y));
	}
}
END_TEST


Suite * motor_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Money");

    /* Core test case */
    tc_core = tcase_create("Core");

	//ADD TESTS HERE
    tcase_add_test(tc_core, status_tests);
    tcase_add_test(tc_core, meta_test);
    tcase_add_test(tc_core, ik_tests);
    tcase_add_test(tc_core, coordinate_conversion_tests);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(int argc, char *argv[])
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = motor_suite();
    sr = srunner_create(s);
	//Turn off forking
	srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
