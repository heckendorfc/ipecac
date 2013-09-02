#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include "../include/ipecac.h"
#include "../include/convert.h"
#include "../include/arithmetic.h"
#include "../include/bitwise.h"
#include "../include/misc.h"
#include "../include/util.h"
#include "internal.h"

long ipecac_get_long(ipint_t *s){
	return s->data[0]; // TODO: fix this
}

/* Note: ipdata_t should always be >= long */
static int set_hex_str(ipint_t *s, const char *str){
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

static int set_decimal_str(ipint_t *s, const char *str){
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
		return set_hex_str(s,str);
	else if(base==10)
		return set_decimal_str(s,str);
	else
		return IPECAC_ERROR;
}


static int get_decimal_str(ipint_t *s, char **str){
	int i;
	int nchars=s->bits_used*log10(2)+1;
	ipint_t q;
	ipint_t r;
	ipint_t d;
	ipint_t end;
	ipdata_t di=10,endi=9;
	char *ptr;

	ptr=malloc(nchars);
	if(ptr==NULL)
		return IPECAC_ERROR;

	ipecac_init(&q,0);
	ipecac_init(&r,0); // Can we fake this?
	ipecac_clone(&q,s);

	d.sign=SIGN_POS;
	d.bits_allocated=DATA_WIDTH;
	d.data=&di;
	d.bits_used=get_num_bits(&d,0);

	end.sign=SIGN_POS;
	end.bits_allocated=DATA_WIDTH;
	end.data=&endi;
	end.bits_used=get_num_bits(&end,0);

	i=nchars;
	ptr[--i]=0;

	do{
		ipecac_div(&q,&r,&q,&d);
		ptr[--i]='0'+r.data[0];
	}while(ipecac_cmp(&q,&end)>0);

	ptr[--i]='0'+q.data[0];
	while(i)ptr[--i]='0';

	*str=ptr;

	return IPECAC_SUCCESS;
}

int ipecac_get_str(ipint_t *s, char **str, int base){
	if(base==10)
		return get_decimal_str(s,str);
	else
		return IPECAC_ERROR;
}
