#include <stdlib.h>
#include "../include/ipecac.h"
#include "internal.h"

int resize_ipint(ipint_t *s, uint32_t minblocks){
	uint32_t blocks=minblocks+RESIZE_BLOCK_SIZE;
	s->allocated=blocks;
	s->data=realloc(s->data,blocks*(DATA_WIDTH/8));
	if(s->data==NULL)
		return IPECAC_ERROR;
	return IPECAC_SUCCESS;
}

uint32_t int_log2(ipdata_t n){
	uint32_t shift=DATA_WIDTH/2;
	uint32_t size=shift;
	const ipdata_t mask=~0;

	if(n==0 || n==1)
		return 1;

	while(size){
		size>>=1;
		if((n&(mask<<shift))!=0){
			if(size)
				shift+=size;
			else
				shift++;
		}
		else{
			if(size)
				shift-=size;
			else
				shift--;
		}
	}
	return shift;
}

uint32_t get_num_bits(ipint_t *s, uint32_t offset){
	ipdata_t n = s->data[offset];

	return int_log2(n);
}
