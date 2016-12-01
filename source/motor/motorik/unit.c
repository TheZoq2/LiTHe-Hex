#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "gangstil.h"


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
		
		Point2D rotated = rotate_point_by_angle(angle);
		ck_assert(rotated.x == 0);
		ck_assert(rotated.y == 1);
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
