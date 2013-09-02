#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "check_ipecac.h"

static Suite *master_suite(){
	Suite *s = suite_create ("Master");
	/* Core test case */
	//TCase *tc_core = tcase_create ("Core");
	return s;
}

int main(){
	int number_failed;
	SRunner *sr = srunner_create (master_suite());
	srunner_add_suite(sr,bitwise_suite());
	srunner_add_suite(sr,arithmetic_suite());
	srunner_add_suite(sr,convert_suite());
	srunner_add_suite(sr,function_suite());
	srunner_set_log(sr,"run.log");
	srunner_set_fork_status (sr,CK_NOFORK);
	srunner_run_all (sr, CK_NORMAL);
	number_failed = srunner_ntests_failed (sr);
	srunner_free (sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

