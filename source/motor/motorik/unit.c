#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "gangstil.h"
#include <stdbool.h>


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
	}

	//Testing the side legs
	{
		Point2D original;
		original.x = 0.20;
		original.y = 0;

		Point2D result = robot_to_ik_coords(original, 2);
		
		ck_assert(float_is_almost(result.x, 0.20));
		ck_assert(float_is_almost(result.y, 0));

		original.x = 0.15;
		original.y = 0.05;

		result = robot_to_ik_coords(original, 3);
		

		printf("%f, %f\n", original.x, original.y);
		ck_assert(float_is_almost(result.x, 0.15));
		ck_assert(float_is_almost(result.y, 0.05));
	}
	//Testing the side legs
	{
		Point2D original;
		original.x = -0.20;
		original.y = 0;

		Point2D result = robot_to_ik_coords(original, 3);
		
		ck_assert(float_is_almost(result.x, 0.20));
		ck_assert(float_is_almost(result.y, 0));

		original.x = -0.15;
		original.y = 0.05;

		result = robot_to_ik_coords(original, 3);
		
		printf("%f, %f\n", original.x, original.y);
		ck_assert(float_is_almost(result.x, 0.15));
		ck_assert(float_is_almost(result.y, 0.05));
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

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
