#include <stdlib.h>
#include "../include/ipecac.h"
#include "internal.h"

int resize_ipint(ipint_t *s, uint32_t minblocks){
	uint32_t blocks=minblocks-(s->bits_allocated/DATA_WIDTH)+RESIZE_BLOCK_SIZE;
	s->bits_allocated=blocks*DATA_WIDTH;
	s->data=realloc(s->data,blocks);
	if(s->data==NULL)
		return IPECAC_ERROR;
	return IPECAC_SUCCESS;
}

uint32_t get_num_bits(ipint_t *s, uint32_t offset){
	uint32_t ret=1;
	ipdata_t n = s->data[offset];

	for(ret=0;n;ret++)n>>=1;

	return ret;
}
