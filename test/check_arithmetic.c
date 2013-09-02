#include <limits.h>
#include <check.h>
#include "check_ipecac.h"
#include "../include/bitwise.h"
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

	ipecac_free(&a);
	ipecac_free(&b);
	ipecac_free(&r);
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

	ipecac_free(&a);
	ipecac_free(&b);
	ipecac_free(&r);
}END_TEST

START_TEST(test_mul_normal_ops){
	ipint_t a,b,r;
	
	ipecac_init(&a,INT_MAX);
	ipecac_init(&b,1);
	ipecac_add(&b,&a,&b); // 32
	ipecac_init(&r,0x0);

	ipecac_mul(&a,&b,&b);

	fail_unless(a.data[1]==0x40000000 && a.data[0]==0);

	ipecac_add(&a,&a,&b);
	ipecac_mul(&r,&a,&a);

	fail_unless(r.data[3]==0x10000000 && r.data[2]==0x40000000 && r.data[1]==0x40000000&& r.data[0]==0);

	ipecac_free(&a);
	ipecac_free(&b);
	ipecac_free(&r);
}END_TEST

START_TEST(test_div_normal_ops){
	ipint_t a,b,r,q;
	
	ipecac_init(&q,0);
	ipecac_init(&a,2);
	ipecac_init(&b,0);
	ipecac_init(&r,0x0);
	ipecac_bit_lshift(&b,&a,32);
	ipecac_bit_lshift(&r,&a,16);
	ipecac_bit_or(&r,&r,&b);
	ipecac_bit_or(&r,&r,&a);

	fail_unless(r.data[1]==0x2 && r.data[0]==0x00020002);

	//ipecac_bit_rshift(&a,&r,1);

	/* Single digit case */
	ipecac_div(&q,&b,&r,&a);

	fail_unless(q.data[1]==0x1 && q.data[0]==0x00010001);

	ipecac_bit_lshift(&b,&a,16);
	ipecac_bit_or(&b,&b,&a);

	/* Multi digit case */
	ipecac_div(&q,&a,&r,&b);

	fail_unless(q.data[0]==0x00010000);

	ipecac_free(&a);
	ipecac_free(&b);
	ipecac_free(&q);
	ipecac_free(&r);
}END_TEST

Suite *arithmetic_suite(){
	Suite *s = suite_create ("Arithmetic Operators");

	TCase *tc_add = tcase_create ("Add");
	tcase_add_test (tc_add, test_add_normal_ops);
	suite_add_tcase (s, tc_add);

	TCase *tc_sub = tcase_create ("Sub");
	tcase_add_test (tc_sub, test_sub_normal_ops);
	suite_add_tcase (s, tc_sub);

	TCase *tc_mul = tcase_create ("Mul");
	tcase_add_test (tc_mul, test_mul_normal_ops);
	suite_add_tcase (s, tc_mul);

	TCase *tc_div = tcase_create ("Div");
	tcase_add_test (tc_div, test_div_normal_ops);
	suite_add_tcase (s, tc_div);

	return s;
}
