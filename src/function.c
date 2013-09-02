#include "../include/ipecac.h"
#include "../include/function.h"
#include "../include/bitwise.h"
#include "../include/arithmetic.h"
#include "../include/misc.h"
#include "../include/util.h"


/* Should this be moved/copied to sunbathe? */
int ipecac_sqrt(ipint_t *r, ipint_t *n){
	ipint_t tmp,rem;
	ipint_t x[2];
	int i=0;

	ipecac_init(&rem,0);
	ipecac_init(&tmp,0);
	ipecac_init(x,0);
	ipecac_init(x+1,0);

	ipecac_clone(x,n);

	do{
		ipecac_clone(x+1,x);
		ipecac_clone(&tmp,x+i);
		ipecac_div(x+i,&rem,n,x+i);
		ipecac_add(x+i,x+i,&tmp);
		ipecac_bit_rshift(x+i,x+i,1);
	}while(ipecac_cmp(x,x+1)!=0);

	ipecac_clone(r,x);

	ipecac_free(x);
	ipecac_free(x+1);
	ipecac_free(&tmp);
	ipecac_free(&rem);

	return IPECAC_SUCCESS;
}
