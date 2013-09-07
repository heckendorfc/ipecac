#include <stdlib.h>
#include <limits.h>
#include <check.h>
#include "check_ipecac.h"
#include "../include/bitwise.h"
#include "../include/arithmetic.h"
#include "../include/misc.h"
#include "../include/convert.h"

static const char *bigstra="424275119274482205563";
static const char *bigstrb="6473924549476352";

START_TEST(test_add_normal_ops){
	ipint_t a,b,r;
	char *rstr;

	ipecac_init(&a,0);
	ipecac_init(&b,0);
	ipecac_init(&r,0);

	ipecac_set_str(&a,bigstra,10);
	ipecac_set_str(&b,bigstrb,10);

	ipecac_add(&r,&a,&b);

	ipecac_get_str(&r,&rstr,10);

	fail_unless(strcmp("424281593199031681915",rstr)==0);

	free(rstr);
	ipecac_free(&a);
	ipecac_free(&b);
	ipecac_free(&r);
}END_TEST

START_TEST(test_sub_normal_ops){
	ipint_t a,b,r;
	char *rstr;

	ipecac_init(&a,0);
	ipecac_init(&b,0);
	ipecac_init(&r,0);

	ipecac_set_str(&a,bigstra,10);
	ipecac_set_str(&b,bigstrb,10);

	ipecac_sub(&r,&a,&b);

	ipecac_get_str(&r,&rstr,10);

	fail_unless(strcmp("424268645349932729211",rstr)==0);

	free(rstr);
	ipecac_free(&a);
	ipecac_free(&b);
	ipecac_free(&r);
}END_TEST

START_TEST(test_mul_normal_ops){
	ipint_t a,b,r;
	char *rstr;

	ipecac_init(&a,0);
	ipecac_init(&b,0);
	ipecac_init(&r,0);

	ipecac_set_str(&a,bigstra,10);
	ipecac_set_str(&b,bigstrb,10);

	ipecac_mul(&r,&a,&b);

	ipecac_get_str(&r,&rstr,10);

	fail_unless(strcmp("2746725110403077721474608213671346176",rstr)==0);

	free(rstr);
	ipecac_free(&a);
	ipecac_free(&b);
	ipecac_free(&r);
}END_TEST

START_TEST(test_div_normal_ops){
	ipint_t a,b,q,r;
	char *rstr;

	ipecac_init(&a,0);
	ipecac_init(&b,0);
	ipecac_init(&r,0);
	ipecac_init(&q,0);

	ipecac_set_str(&a,bigstra,10);
	ipecac_set_str(&b,bigstrb,10);

	ipecac_div(&q,&r,&a,&b);

	ipecac_get_str(&q,&rstr,10);

	fail_unless(strcmp("65536",rstr)==0);

	free(rstr);

	ipecac_set_str(&b,"9605120",10);

	ipecac_div(&q,&r,&a,&b);

	ipecac_get_str(&q,&rstr,10);

	fail_unless(strcmp("44171766648879",rstr)==0);

	free(rstr);
	ipecac_free(&a);
	ipecac_free(&b);
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
