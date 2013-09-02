#include <string.h>
#include <check.h>
#include "check_ipecac.h"
#include "../include/arithmetic.h"
#include "../include/bitwise.h"
#include "../include/misc.h"
#include "../include/convert.h"


START_TEST(test_set_str_normal_ops){
	ipint_t r;

	ipecac_init(&r,0);

	ipecac_set_str(&r,"8590065666",10);

	fail_unless(r.data[1]==0x2 && r.data[0]==0x00020002);

	ipecac_set_str(&r,"300030003",16);

	fail_unless(r.data[1]==0x3 && r.data[0]==0x00030003);

	ipecac_free(&r);
}END_TEST

START_TEST(test_get_str_normal_ops){
	ipint_t r;
	char *s;

	ipecac_init(&r,0);

	ipecac_set_str(&r,"8590065666",10);

	fail_unless(r.data[1]==0x2 && r.data[0]==0x00020002);

	ipecac_get_str(&r,&s,10);

	fail_unless(strcmp(s,"8590065666")==0);

	ipecac_free(&r);
}END_TEST

Suite *convert_suite(){
	Suite *s = suite_create ("Convert");

	TCase *tc_ss = tcase_create ("Set String");
	tcase_add_test (tc_ss, test_set_str_normal_ops);
	suite_add_tcase (s, tc_ss);

	TCase *tc_gs = tcase_create ("Get String");
	tcase_add_test (tc_gs, test_get_str_normal_ops);
	suite_add_tcase (s, tc_gs);

	return s;
}
