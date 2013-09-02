#include <limits.h>
#include <check.h>
#include "check_ipecac.h"
#include "../include/function.h"
#include "../include/misc.h"

START_TEST(test_sqrt_normal_ops){
	ipint_t r,n;

	ipecac_init(&r,0);
	ipecac_init(&n,27);

	ipecac_sqrt(&r,&n);

	fail_unless(r.data[0]==5);

	ipecac_free(&r);
	ipecac_free(&n);
}END_TEST

Suite *function_suite(){
	Suite *s = suite_create ("Functions");

	TCase *tc_sqrt = tcase_create ("Sqrt");
	tcase_add_test (tc_sqrt, test_sqrt_normal_ops);
	suite_add_tcase (s, tc_sqrt);

	return s;
}
