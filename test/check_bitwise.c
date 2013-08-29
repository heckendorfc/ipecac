#include <check.h>
#include "check_ipecac.h"
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

Suite *bitwise_suite(){
	Suite *s = suite_create ("Bitwise Operators");

	TCase *tc_and = tcase_create ("AND");
	tcase_add_test (tc_and, test_bit_and_normal_ops);
	suite_add_tcase (s, tc_and);

	TCase *tc_or = tcase_create ("OR");
	tcase_add_test (tc_or, test_bit_or_normal_ops);
	suite_add_tcase (s, tc_or);

	return s;
}
