#include "../include/ipecac.h"
#include "../include/arithmetic.h"
#include "../include/misc.h"
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

	for(i=0;i<=j;i++){
		// Set at tmp in case r==large || r==small
		tmp=large->data[i]+small->data[i]+k;
		if(tmp<large->data[i] && tmp<small->data[i])
			k=1;
		else
			k=0;
		r->data[i]=tmp;
	}

	if(k){
		r->data[carrybit/DATA_WIDTH]|=k<<(carrybit%DATA_WIDTH);
	}

	return IPECAC_SUCCESS;
}

int ipecac_sub(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->bits_used;
	uint32_t ib = b->bits_used;
	uint32_t rused=r->bits_used;
	int i,j,k;
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

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,r->bits_used/DATA_WIDTH)==IPECAC_ERROR)
			return IPECAC_ERROR;

	k=0;

	for(i=0;i<=j;i++){
		// Set at tmp in case r==large
		tmp=large->data[i]-small->data[i]-k;
		if(tmp>large->data[i])
			k=1;
		else
			k=0;
		r->data[i]=tmp;
	}

	return IPECAC_SUCCESS;
}

static int basic_mul(ipint_t *r, ipint_t *a, ipint_t *b){
	int i,j;
	ipdata_t tmp;
	ipdata_t k;
	half_ipdata_t *sa=(half_ipdata_t*)a->data;
	half_ipdata_t *sb=(half_ipdata_t*)b->data;
	half_ipdata_t *sr=(half_ipdata_t*)r->data;

	r->bits_used=a->bits_used+b->bits_used;

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,r->bits_used/DATA_WIDTH)==IPECAC_ERROR)
			return IPECAC_ERROR;

	for(j=0;j<=(b->bits_used-1)/(DATA_WIDTH/2);j++){
		/* // Good for small DATA_WIDTH
		if(sb[j]==0){
			sr[j]=0;
			continue;
		}
		*/
		k=0;
		for(i=0;i<=(a->bits_used-1)/(DATA_WIDTH/2);i++){
			tmp=sa[i]*sb[j]+sr[i+j]+k;
			k=HIGH_HALF(tmp);
			sr[i+j]=tmp;
		}
		sr[i+j]=k;
	}

	return IPECAC_SUCCESS;
}

int ipecac_mul(ipint_t *r, ipint_t *a, ipint_t *b){
	return basic_mul(r,a,b,1);
}
