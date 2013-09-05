#include <stdlib.h>
#include <cobra.h>
#include "bench_ipecac.h"
#include "../include/arithmetic.h"
#include "../include/convert.h"
#include "../include/misc.h"

struct multarg{
	ipint_t r;
	ipint_t a;
	ipint_t b;
};

static const char *bigsa="624275119274482205563424275119274482205563424275119274482205563424275119274482205563";
static const char *bigsb="647392454947635264739245494763522309638129647392454947635264739245494763522309638129";

START_TEST(pre_mult_normal_ops,arg){
	struct multarg *ma=(struct multarg*)arg;

	ipecac_init(&ma->r,0);
	ipecac_init(&ma->a,0);
	ipecac_init(&ma->b,0);

	ipecac_set_str(&ma->a,bigsa,10);
	ipecac_set_str(&ma->b,bigsb,10);
}END_TEST

START_TEST(test_mult_normal_ops,arg){
	struct multarg *ma=(struct multarg*)arg;
	int i;
	for(i=0;i<100;i++)
		ipecac_mul(&ma->r,&ma->a,&ma->b);
}END_TEST

START_TEST(post_mult_normal_ops,arg){
	struct multarg *ma=(struct multarg*)arg;
	ipecac_free(&ma->r);
	ipecac_free(&ma->a);
	ipecac_free(&ma->b);
}END_TEST

void arithmetic_suite(BenchSet *bs){
	struct testgroup *tg=add_testgroup(bs,"Arithmetic");
	struct multarg *arg;
	arg=malloc(sizeof(*arg));

	add_test(tg,"mult",pre_mult_normal_ops,test_mult_normal_ops,post_mult_normal_ops,arg);
}
