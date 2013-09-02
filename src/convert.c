#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include "../include/ipecac.h"
#include "../include/convert.h"
#include "../include/arithmetic.h"
#include "../include/bitwise.h"
#include "../include/misc.h"
#include "internal.h"

long ipecac_get_long(ipint_t *s){
	return s->data[0]; // TODO: fix this
}

/* Note: ipdata_t should always be >= long */
static int hex_str(ipint_t *s, const char *str){
	ipint_t tmp;
	ipdata_t ti;
	long ret;
	int i;
	int shift;
	const char *ptr;
	const int nchars=sizeof(ret)*2;
	char *buf;

	buf=malloc(nchars+1);
	if(buf==NULL)
		return IPECAC_ERROR;

	tmp.sign=SIGN_POS;
	tmp.bits_allocated=DATA_WIDTH;
	tmp.data=&ti;

	ipecac_set(s,0);
	ptr=str;
	do{
		for(i=0;i<nchars && *ptr;i++)
			buf[i]=*(ptr++);
		buf[i]=0;
		ret=strtol(buf,NULL,16);
		ti=ret;
		shift=i*4;
		ipecac_bit_lshift(s,s,shift);
		tmp.bits_used=get_num_bits(&tmp,0);
		ipecac_bit_or(s,s,&tmp);
	}while(*ptr);

	free(buf);

	return IPECAC_SUCCESS;
}

static int decimal_str(ipint_t *s, const char *str){
	const int nchars=log10(LONG_MAX);
	int shift;
	ipint_t tmp;
	ipdata_t ti;
	long ret;
	int i;
	const char *ptr;
	char *buf;

	buf=malloc(nchars+1);
	if(buf==NULL)
		return IPECAC_ERROR;

	tmp.sign=SIGN_POS;
	tmp.bits_allocated=DATA_WIDTH;
	tmp.data=&ti;

	ipecac_set(s,0);
	ptr=str;
	do{
		for(i=0;i<nchars && *ptr;i++)
			buf[i]=*(ptr++);
		buf[i]=0;
		ret=strtol(buf,NULL,10);
		shift=pow(10,i);
		ti=shift;
		tmp.bits_used=get_num_bits(&tmp,0);
		ipecac_mul(s,s,&tmp);
		ti=ret;
		tmp.bits_used=get_num_bits(&tmp,0);
		ipecac_add(s,s,&tmp);
	}while(*ptr);

	free(buf);

	return IPECAC_SUCCESS;
}

int ipecac_set_str(ipint_t *s, const char *str, int base){
	if(base==16)
		return hex_str(s,str);
	else if(base==10)
		return decimal_str(s,str);
	else
		return IPECAC_ERROR;
}
