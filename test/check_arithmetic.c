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

Suite *arithmetic_suite(){
	Suite *s = suite_create ("Arithmetic Operators");

	TCase *tc_add = tcase_create ("add");
	tcase_add_test (tc_add, test_add_normal_ops);
	suite_add_tcase (s, tc_add);

	return s;
}
