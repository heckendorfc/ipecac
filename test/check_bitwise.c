#include <check.h>
#include "check_ipecac.h"
#include "../include/arithmetic.h"
#include "../include/bitwise.h"
#include "../include/misc.h"
#include "../include/convert.h"

START_TEST(test_bit_and_normal_ops){
	ipint_t a,b,r;
	
	ipecac_init(&a,0xF0);
	ipecac_init(&b,0x10);
	ipecac_init(&r,0x0);

	ipecac_bit_and(&r,&a,&b);

	fail_unless(ipecac_get_long(&r)==0x10);

}END_TEST

START_TEST(test_bit_or_normal_ops){
	ipint_t a,b,r;
	
	ipecac_init(&a,0xF0);
	ipecac_init(&b,0x0F);
	ipecac_init(&r,0x0);

	ipecac_bit_or(&r,&a,&b);

	fail_unless(ipecac_get_long(&r)==0xFF);

}END_TEST

START_TEST(test_bit_ls_normal_ops){
	ipint_t b,r;
	
	ipecac_init(&r,0x1);
	ipecac_bit_lshift(&r,&r,28);
	ipecac_init(&b,0x1);
	ipecac_bit_lshift(&b,&b,4);
	ipecac_bit_or(&r,&r,&b);
	ipecac_bit_lshift(&r,&r,32);

	fail_unless(r.data[1]==0x10000010 && r.data[0]==0);
}END_TEST

START_TEST(test_bit_rs_normal_ops){
	ipint_t b,r;
	
	ipecac_init(&r,0x1);
	ipecac_bit_lshift(&r,&r,28);
	ipecac_init(&b,0x1);
	ipecac_bit_lshift(&b,&b,4);
	ipecac_bit_or(&r,&r,&b);
	ipecac_bit_lshift(&r,&r,32);
	ipecac_set(&b,0x1);
	ipecac_bit_lshift(&b,&b,17);
	ipecac_bit_or(&r,&r,&b);
	ipecac_bit_rshift(&r,&r,16);

	fail_unless(r.data[1]==0x1000 && r.data[0]==0x00100002);
}END_TEST

Suite *bitwise_suite(){
	Suite *s = suite_create ("Bitwise Operators");

	TCase *tc_and = tcase_create ("AND");
	tcase_add_test (tc_and, test_bit_and_normal_ops);
	suite_add_tcase (s, tc_and);

	TCase *tc_or = tcase_create ("OR");
	tcase_add_test (tc_or, test_bit_or_normal_ops);
	suite_add_tcase (s, tc_or);

	TCase *tc_ls = tcase_create ("LSHIFT");
	tcase_add_test (tc_ls, test_bit_ls_normal_ops);
	suite_add_tcase (s, tc_ls);

	TCase *tc_rs = tcase_create ("RSHIFT");
	tcase_add_test (tc_rs, test_bit_rs_normal_ops);
	suite_add_tcase (s, tc_rs);

	return s;
}
