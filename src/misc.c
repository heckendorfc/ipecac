#include <stdlib.h>
#include "../include/ipecac.h"
#include "../include/misc.h"
#include "internal.h"

int ipecac_init_b(ipint_t *s, unsigned int b){
	int i;

	s->sign=SIGN_POS;

	s->allocated=b;
	s->data=malloc(sizeof(*(s->data))*(s->allocated));
	if(s->data==NULL)
		return IPECAC_ERROR;

	for(i=0;i<b;i++)
		s->data[i]=0;

	s->used=1;

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

	s->allocated=INITIAL_BLOCK_SIZE;
	s->data[0]=x;
	s->used=1;

	return IPECAC_SUCCESS;
}

int ipecac_set(ipint_t *s, int x){
	if(x>=0)
		s->sign=SIGN_POS;
	else
		s->sign=SIGN_NEG;
	s->data[0]=x;
	s->used=1;

	return IPECAC_SUCCESS;
}

int ipecac_clone(ipint_t *r, ipint_t *s){
	int i,end;
	end=s->used;
	if(s->used>r->allocated)
		if(resize_ipint(r,end+1)==IPECAC_ERROR)
			return IPECAC_ERROR;

	r->sign=s->sign;

	for(i=0;i<=end;i++)
		r->data[i]=s->data[i];

	r->used=s->used;

	return IPECAC_SUCCESS;
}

int ipecac_free(ipint_t *s){
	if(s->data)
		free(s->data);
	s->data=NULL;
	s->allocated=0;
	return IPECAC_SUCCESS;
}
