#include <stdlib.h>
#include "../include/ipecac.h"
#include "../include/misc.h"
#include "internal.h"

int ipecac_init_b(ipint_t *s, unsigned int b){
	int i;

	s->sign=SIGN_POS;

	s->bits_allocated=b;
	s->data=malloc(sizeof(*(s->data))*(s->bits_allocated));
	if(s->data==NULL)
		return IPECAC_ERROR;

	for(i=1;i<INITIAL_BLOCK_SIZE;i++)
		s->data[i]=0;

	s->bits_allocated*=DATA_WIDTH;
	s->data[0]=0;
	s->bits_used=1;

	return IPECAC_SUCCESS;
}

int ipecac_init(ipint_t *s, int x){
	int i;
	if(x>=0)
		s->sign=SIGN_POS;
	else
		s->sign=SIGN_NEG;

	s->data=malloc(sizeof(*(s->data))*INITIAL_BLOCK_SIZE);
	if(s->data==NULL)
		return IPECAC_ERROR;

	for(i=1;i<INITIAL_BLOCK_SIZE;i++)
		s->data[i]=0;

	s->bits_allocated=DATA_WIDTH*INITIAL_BLOCK_SIZE;
	s->data[0]=x;
	s->bits_used=get_num_bits(s,0);

	return IPECAC_SUCCESS;
}

int ipecac_free(ipint_t *s){
	free(s->data);
	return IPECAC_SUCCESS;
}
