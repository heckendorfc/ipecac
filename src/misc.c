#include <stdlib.h>
#include "../include/ipecac.h"
#include "../include/misc.h"
#include "internal.h"

int ipecac_init(ipint_t *s, int x){
	if(x>=0)
		s->sign=SIGN_POS;
	else
		s->sign=SIGN_NEG;

	s->data=malloc(sizeof(*(s->data))*INITIAL_BLOCK_SIZE);
	if(s->data==NULL)
		return IPECAC_ERROR;

	s->bits_allocated=DATA_WIDTH*INITIAL_BLOCK_SIZE;
	s->data[0]=x;
	s->bits_used=get_num_bits(s,0);

	return IPECAC_SUCCESS;
}

int ipecac_free(ipint_t *s){
	free(s->data);
	return IPECAC_SUCCESS;
}