#include "../include/ipecac.h"
#include "../include/bitwise.h"
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
