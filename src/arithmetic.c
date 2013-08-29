#include "../include/ipecac.h"
#include "../include/arithmetic.h"
#include "internal.h"

int ipecac_add(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->bits_used;
	uint32_t ib = b->bits_used;
	uint32_t rused=r->bits_used;
	int i,j,k,carrybit;
	ipint_t *large,*small;
	ipdata_t tmp;
	
	if(ia>ib){
		j=(ia-1)/DATA_WIDTH;
		large=a;
		small=b;
		r->bits_used=ia;
	}
	else{
		j=(ib-1)/DATA_WIDTH;
		large=b;
		small=a;
		r->bits_used=ib;
	}

	if(rused>r->bits_used){
		for(i=r->bits_used+DATA_WIDTH;i<rused;i+=DATA_WIDTH)
			r->data[i/DATA_WIDTH]=0;
		r->data[r->bits_used/DATA_WIDTH]&=(~0)>>(r->bits_used%DATA_WIDTH);
	}

	carrybit=r->bits_used++;

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,r->bits_used/DATA_WIDTH)==IPECAC_ERROR)
			return IPECAC_ERROR;

	k=0;

	while(j>=0){
		// Set at tmp in case r==large || r==small
		tmp=large->data[j]+small->data[j]+k;
		if(tmp<large->data[j] && tmp<small->data[j])
			k=1;
		else
			k=0;
		r->data[j]=tmp;
		j--;
	}

	if(k){
		r->data[carrybit/DATA_WIDTH]|=k<<(carrybit%DATA_WIDTH);
	}

	return IPECAC_SUCCESS;
}
