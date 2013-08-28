#include <check.h>
#include "check_ipecac.h"
#include "../include/bitwise.h"

START_TEST(test_bit_or_normal_ops){
}END_TEST

Suite *cont_bitwise_suite(){
	Suite *s = suite_create ("Bitwise Operators");

	TCase *tc_or = tcase_create ("OR");
	tcase_add_test (tc_or, test_bit_or_normal_ops);
	suite_add_tcase (s, tc_or);

	return s;
}
