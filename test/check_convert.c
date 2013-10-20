#include <stdlib.h>
#include <string.h>
#include <check.h>
#include "check_ipecac.h"
#include "../include/arithmetic.h"
#include "../include/bitwise.h"
#include "../include/misc.h"
#include "../include/convert.h"

static const char *lstrtest = "2474759212146018060034179605221714198337456593024321708869587061479529987775603938855127754045566936181338939114075183455901964200052474240035667";

START_TEST(test_set_str_normal_ops){
	ipint_t r;

	ipecac_init(&r,0);

	ipecac_set_str(&r,"8590065666",10);

	fail_unless((r.data[1]==0x2 && r.data[0]==0x00020002) || r.data[0]==0x200020002);

	ipecac_set_str(&r,"300030003",16);

	fail_unless((r.data[1]==0x3 && r.data[0]==0x00030003) || r.data[0]==0x300030003);

	ipecac_free(&r);
}END_TEST

START_TEST(test_get_str_normal_ops){
	ipint_t r;
	char *s;
	char *st;

	ipecac_init(&r,0);

	ipecac_set_str(&r,"85900656668590065666",10);

	ipecac_get_str(&r,&s,&st,10);

	fail_unless(strcmp(st,"85900656668590065666")==0);

	free(s);

	ipecac_set_str(&r,lstrtest,10);
	ipecac_get_str(&r,&s,&st,10);

	fail_unless(strcmp(st,lstrtest)==0);

	free(s);
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
