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

int ipecac_set(ipint_t *s, int x){
	if(x>=0)
		s->sign=SIGN_POS;
	else
		s->sign=SIGN_NEG;
	s->data[0]=x;
	s->bits_used=get_num_bits(s,0);
}

int ipecac_clone(ipint_t *r, ipint_t *s){
	int i,end;
	end=(s->bits_used+DATA_WIDTH)/DATA_WIDTH;
	if(s->bits_used>r->bits_allocated)
		if(resize_ipint(r,end+1)==IPECAC_ERROR)
			return IPECAC_ERROR;

	r->sign=s->sign;

	for(i=0;i<end;i++)
		r->data[i]=s->data[i];

	r->bits_used=s->bits_used;

	return IPECAC_SUCCESS;
}

int ipecac_free(ipint_t *s){
	free(s->data);
	return IPECAC_SUCCESS;
}
