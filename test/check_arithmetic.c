#include <limits.h>
#include <check.h>
#include "check_ipecac.h"
#include "../include/arithmetic.h"
#include "../include/misc.h"
#include "../include/convert.h"

START_TEST(test_add_normal_ops){
	ipint_t a,b,r;
	
	ipecac_init(&a,INT_MAX);
	ipecac_init(&b,1);
	ipecac_init(&r,0x0);

	ipecac_add(&r,&a,&b);
	ipecac_add(&r,&r,&r);
	ipecac_add(&r,&r,&r);

	fail_unless(r.data[1]==2 && r.data[0]==0);

}END_TEST

START_TEST(test_sub_normal_ops){
	ipint_t a,b,r;
	
	ipecac_init(&a,INT_MAX);
	ipecac_init(&b,1);
	ipecac_init(&r,0x0);

	ipecac_add(&a,&a,&b); // 32
	ipecac_add(&b,&a,&a); // 33
	ipecac_add(&r,&b,&b); // 34

	fail_unless(r.data[1]==2 && r.data[0]==0);

	ipecac_sub(&r,&r,&b); // 34-33=33
	ipecac_sub(&r,&r,&a); // 33-32=32

	fail_unless(r.data[1]==0 && r.data[0]==1<<31);

}END_TEST

START_TEST(test_mul_normal_ops){
	ipint_t a,b,r;
	
	ipecac_init(&a,INT_MAX);
	ipecac_init(&b,1);
	ipecac_add(&b,&a,&b); // 32
	ipecac_init(&r,0x0);

	ipecac_mul(&r,&b,&b);

	fail_unless(r.data[1]==0x40000000 && r.data[0]==0);

}END_TEST

Suite *arithmetic_suite(){
	Suite *s = suite_create ("Arithmetic Operators");

	TCase *tc_add = tcase_create ("Add");
	tcase_add_test (tc_add, test_add_normal_ops);
	suite_add_tcase (s, tc_add);

	TCase *tc_sub = tcase_create ("Sub");
	tcase_add_test (tc_add, test_sub_normal_ops);
	suite_add_tcase (s, tc_sub);

	TCase *tc_mul = tcase_create ("Mul");
	tcase_add_test (tc_add, test_mul_normal_ops);
	suite_add_tcase (s, tc_mul);

	return s;
}
