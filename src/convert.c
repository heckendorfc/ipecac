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
	tmp.allocated=1;
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
		tmp.used=1;
		ipecac_bit_or(s,s,&tmp);
	}while(*ptr);

	free(buf);

	return IPECAC_SUCCESS;
}

static int set_decimal_str(ipint_t *s, const char *str){
	const int nchars=log10(LONG_MAX)-1;
	ipdata_t shift;
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
	tmp.allocated=1;
	tmp.data=&ti;
	tmp.used=1;

	ipecac_set(s,0);
	ptr=str;
	do{
		for(i=0;i<nchars && *ptr;i++)
			buf[i]=*(ptr++);
		buf[i]=0;
		ret=strtol(buf,NULL,10);
		shift=pow(10,i);
		ti=shift;
		ipecac_mul(s,s,&tmp);
		ti=ret;
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
	int nchars=((s->used-1)*DATA_WIDTH+get_num_bits(s,s->used-1))/3.32192809488736234787031+2; // log2(s)/log10(2)
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
	d.allocated=1;
	d.data=&di;
	d.used=1;

	end.sign=SIGN_POS;
	end.allocated=1;
	end.data=&endi;
	end.used=1;

	i=nchars;
	ptr[--i]=0;

	do{
		ipecac_div(&q,&r,&q,&d);
		ptr[--i]='0'+r.data[0];
	}while(ipecac_cmp(&q,&end)>0);

	ptr[--i]='0'+q.data[0];
	while(i>0)ptr[--i]='0';

	*str=ptr;

	return IPECAC_SUCCESS;
}

int ipecac_get_str(ipint_t *s, char **str, int base){
	if(base==10)
		return get_decimal_str(s,str);
	else
		return IPECAC_ERROR;
}
