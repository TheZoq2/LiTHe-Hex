#include <check.h>
#include <stdlib.h>
#include <stdio.h>

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


Suite * motorik_test(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Money");

    /* Core test case */
    tc_core = tcase_create("Core");

	//ADD TESTS HERE
    tcase_add_test(tc_core, ik_tests);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(int argc, char *argv[])
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = money_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
