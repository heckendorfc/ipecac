#include "../include/ipecac.h"
#include "../include/bitwise.h"
#include "../include/misc.h"
#include "internal.h"

int ipecac_bit_and(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = (a->bits_used-1)/DATA_WIDTH;
	uint32_t ib = (b->bits_used-1)/DATA_WIDTH;
	uint32_t i;

	if(ia>ib)
		ia=ib;

	if(r->bits_allocated/DATA_WIDTH<ia)
		if(resize_ipint(r,ia)==IPECAC_ERROR)
			return IPECAC_ERROR;

	for(i=0;i<=ia;i++)
		r->data[i]=a->data[i]&b->data[i];

	r->bits_used=(ia)*DATA_WIDTH+get_num_bits(r,ia);

	return IPECAC_SUCCESS;
}

int ipecac_bit_or(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = (a->bits_used-1)/DATA_WIDTH;
	uint32_t ib = (b->bits_used-1)/DATA_WIDTH;
	uint32_t i;

	if(ia<ib)
		ia=ib;

	if(r->bits_allocated/DATA_WIDTH<ia)
		if(resize_ipint(r,ia)==IPECAC_ERROR)
			return IPECAC_ERROR;

	for(i=0;i<=ia;i++)
		r->data[i]=a->data[i]|b->data[i];

	r->bits_used=(ia)*DATA_WIDTH+get_num_bits(r,ia);

	return IPECAC_SUCCESS;
}

int ipecac_bit_lshift(ipint_t *r, ipint_t *a, unsigned int c){
	const uint32_t sind=c/DATA_WIDTH;
	const uint32_t soff=c-sind*DATA_WIDTH;
	uint32_t used=a->bits_used;
	ipdata_t carry=0;
	int i;

	if(a->bits_used==1 && a->data[0]==0){
		ipecac_set(r,0);
		return IPECAC_SUCCESS;
	}

	r->bits_used=a->bits_used+c;

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,r->bits_used)==IPECAC_ERROR)
			return IPECAC_ERROR;

	for(i=((used-1))/DATA_WIDTH;i>=0;i--){
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
	uint32_t used=a->bits_used;
	const uint32_t end=(a->bits_used-1)/DATA_WIDTH;
	int i;

	if(c>=used){
		ipecac_set(r,0);
		return IPECAC_SUCCESS;
	}

	r->bits_used-=c;

	for(i=0;i<end;i++){
		r->data[i]=(a->data[i+sind+1]<<(DATA_WIDTH-soff))|(a->data[i+sind]>>soff);
	}
	r->data[end]=a->data[end+sind]>>soff;

	return IPECAC_SUCCESS;
}
