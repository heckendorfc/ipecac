#include "../include/ipecac.h"
#include "../include/bitwise.h"
#include "../include/misc.h"
#include "internal.h"

int ipecac_bit_and(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->used;
	uint32_t ib = b->used;
	uint32_t i;

	if(ia>ib)
		ia=ib;

	if(r->allocated<ia)
		if(resize_ipint(r,ia)==IPECAC_ERROR)
			return IPECAC_ERROR;

	for(i=0;i<ia;i++)
		r->data[i]=a->data[i]&b->data[i];

	r->used=ia;

	return IPECAC_SUCCESS;
}

int ipecac_bit_or(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->used;
	uint32_t ib = b->used;
	uint32_t i;
	ipint_t *tmp;

	if(ia<ib){
		ia=ib;
		ib=ia;
		tmp=a;
		a=b;
		b=tmp;
	}

	if(r->allocated<ia)
		if(resize_ipint(r,ia)==IPECAC_ERROR)
			return IPECAC_ERROR;

	for(i=0;i<ib;i++)
		r->data[i]=a->data[i]|b->data[i];
	for(;i<ia;i++)
		r->data[i]=a->data[i];

	r->used=ia;

	return IPECAC_SUCCESS;
}

int ipecac_bit_lshift(ipint_t *r, ipint_t *a, unsigned int c){
	const uint32_t sind=c/DATA_WIDTH;
	const uint32_t soff=c-sind*DATA_WIDTH;
	uint32_t used=a->used;
	ipdata_t carry=0;
	int i,j;

	if(c==0)
		return IPECAC_SUCCESS;

	if(a->used==1 && a->data[0]==0){
		ipecac_set(r,0);
		return IPECAC_SUCCESS;
	}

	r->used=a->used+sind;

	if(r->used>r->allocated)
		if(resize_ipint(r,r->used)==IPECAC_ERROR)
			return IPECAC_ERROR;

	for(i=used-1;i>=0;i--){
		r->data[i+sind+1]=carry|(a->data[i]>>(DATA_WIDTH-soff));
		carry=a->data[i]<<soff;
	}
	r->data[sind]=carry;
	for(i=sind-1;i>=0;i--)
		r->data[i]=0;

	return IPECAC_SUCCESS;
}

int ipecac_bit_rshift(ipint_t *r, ipint_t *a, unsigned int c){
	const uint32_t sind=c/DATA_WIDTH;
	const uint32_t soff=c-sind*DATA_WIDTH;
	uint32_t used=a->used;
	const uint32_t end=a->used-1;
	int i;

	if(c==0)
		return IPECAC_SUCCESS;

	if(c>=used*DATA_WIDTH){
		ipecac_set(r,0);
		return IPECAC_SUCCESS;
	}

	r->used-=sind;

	for(i=0;i<end;i++){
		r->data[i]=(a->data[i+sind+1]<<(DATA_WIDTH-soff))|(a->data[i+sind]>>soff);
	}
	r->data[end]=a->data[end+sind]>>soff;

	if(r->used>0 && r->data[used-1]==0)
		r->used--;

	return IPECAC_SUCCESS;
}
