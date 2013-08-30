#include "../include/ipecac.h"
#include "../include/arithmetic.h"
#include "../include/misc.h"
#include "internal.h"

int ipecac_add(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->bits_used;
	uint32_t ib = b->bits_used;
	uint32_t rused=r->bits_used;
	int i,j,k,carrybit;
	ipint_t *large,*small;
	ipdata_t tmp;
	
	if(ia>ib){
		j=(ia-1)/DATA_WIDTH;
		large=a;
		small=b;
		r->bits_used=ia;
	}
	else{
		j=(ib-1)/DATA_WIDTH;
		large=b;
		small=a;
		r->bits_used=ib;
	}

	if(rused>r->bits_used){
		for(i=r->bits_used+DATA_WIDTH;i<rused;i+=DATA_WIDTH)
			r->data[i/DATA_WIDTH]=0;
		r->data[r->bits_used/DATA_WIDTH]&=(~0)>>(r->bits_used%DATA_WIDTH);
	}

	carrybit=r->bits_used++;

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,r->bits_used/DATA_WIDTH)==IPECAC_ERROR)
			return IPECAC_ERROR;

	k=0;

	for(i=0;i<=j;i++){
		// Set at tmp in case r==large || r==small
		tmp=large->data[i]+small->data[i]+k;
		if(tmp<large->data[i] && tmp<small->data[i])
			k=1;
		else
			k=0;
		r->data[i]=tmp;
	}

	if(k){
		r->data[carrybit/DATA_WIDTH]|=k<<(carrybit%DATA_WIDTH);
	}

	return IPECAC_SUCCESS;
}

int ipecac_sub(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->bits_used;
	uint32_t ib = b->bits_used;
	uint32_t rused=r->bits_used;
	int i,j,k;
	ipint_t *large,*small;
	ipdata_t tmp;
	
	if(ia>ib){
		j=(ia-1)/DATA_WIDTH;
		large=a;
		small=b;
		r->bits_used=ia;
	}
	else{
		j=(ib-1)/DATA_WIDTH;
		large=b;
		small=a;
		r->bits_used=ib;
	}

	if(rused>r->bits_used){
		for(i=r->bits_used+DATA_WIDTH;i<rused;i+=DATA_WIDTH)
			r->data[i/DATA_WIDTH]=0;
		r->data[r->bits_used/DATA_WIDTH]&=(~0)>>(r->bits_used%DATA_WIDTH);
	}

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,r->bits_used/DATA_WIDTH)==IPECAC_ERROR)
			return IPECAC_ERROR;

	k=0;

	for(i=0;i<=j;i++){
		// Set at tmp in case r==large
		tmp=large->data[i]-small->data[i]-k;
		if(tmp>large->data[i])
			k=1;
		else
			k=0;
		r->data[i]=tmp;
	}

	return IPECAC_SUCCESS;
}

static int basic_mul(ipint_t *r, ipint_t *a, ipint_t *b){
	int i,j;
	ipdata_t tmp;
	ipdata_t k;
	half_ipdata_t *sa=(half_ipdata_t*)a->data;
	half_ipdata_t *sb=(half_ipdata_t*)b->data;
	half_ipdata_t *sr=(half_ipdata_t*)r->data;

	r->bits_used=a->bits_used+b->bits_used;

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,r->bits_used/DATA_WIDTH)==IPECAC_ERROR)
			return IPECAC_ERROR;

	for(j=0;j<=r->bits_used;j+=DATA_WIDTH)
		r->data[j]=0;

	for(j=0;j<=(b->bits_used-1)/(DATA_WIDTH/2);j++){
		/* // Good for small DATA_WIDTH
		if(sb[j]==0){
			sr[j]=0;
			continue;
		}
		*/
		k=0;
		for(i=0;i<=(a->bits_used-1)/(DATA_WIDTH/2);i++){
			tmp=sa[i]*sb[j]+sr[i+j]+k;
			k=HIGH_HALF(tmp);
			sr[i+j]=tmp;
		}
		sr[i+j]=k;
	}

	return IPECAC_SUCCESS;
}

static void split_ipint(ipint_t *l, ipint_t *h, ipint_t *i){
	int j;
	const uint32_t mid=((i->bits_used/DATA_WIDTH)+1)/2;
	const uint32_t end=((i->bits_used/DATA_WIDTH)+1)-mid;

	for(j=0;j<mid;j++){
		l->data[j]=i->data[j];
	}
	l->bits_used=DATA_WIDTH*mid;
	for(j=0;j<end;j++){
		h->data[j]=i->data[mid+j];
	}
	h->bits_used=DATA_WIDTH*(end-mid);
}

static int karatsuba_mul(ipint_t *r, ipint_t *a, ipint_t *b){
	int i;
	ipint_t r1,r2;
	ipint_t ha,la,hb,lb;
	ipint_t p0,p1,p2;
	const uint32_t asize=(a->bits_used/DATA_WIDTH)+1;
	const uint32_t bsize=(b->bits_used/DATA_WIDTH)+1;
	const uint32_t m=(asize>bsize?asize:bsize);
	int ret=0;

	if(a->bits_used<=DATA_WIDTH){
		basic_mul(r,a,b);
		return IPECAC_SUCCESS;
	}

	ret|=ipecac_init_b(&r1,asize);
	ret|=ipecac_init_b(&r2,bsize);
	ret|=ipecac_init_b(&p0,asize+bsize);
	ret|=ipecac_init_b(&p1,m*(asize+bsize));
	ret|=ipecac_init_b(&p2,2*m*(asize+bsize));
	ret|=ipecac_init_b(&ha,asize);
	ret|=ipecac_init_b(&la,asize);
	ret|=ipecac_init_b(&hb,bsize);
	ret|=ipecac_init_b(&lb,bsize);
	if(ret)
		return ret;

	split_ipint(&la,&ha,a);
	split_ipint(&lb,&hb,b);

	ret|=karatsuba_mul(&p0,&la,&lb); // low*low
	ret|=ipecac_add(&r1,&la,&ha);
	ret|=ipecac_add(&r2,&lb,&hb);
	ret|=karatsuba_mul(&p1,&r1,&r2); // (low_a+high_a)(low_b+high_b)
	ret|=karatsuba_mul(&p2,&ha,&hb); // high*high

	if(ret)
		return ret;

	ret|=ipecac_sub(&p1,&p1,&p2);
	ret|=ipecac_sub(&p1,&p1,&p0);

	for(i=0;i<asize+bsize;i++){
		p2.data[2*m+i]=p2.data[i];
		p2.data[i]=0;
		p1.data[m+i]=p1.data[i];
		p1.data[i]=0;
	}

	ret|=ipecac_add(r,&p1,&p0);
	ret|=ipecac_add(r,r,&p2);

	ipecac_free(&r1);
	ipecac_free(&r2);
	ipecac_free(&p0);
	ipecac_free(&p1);
	ipecac_free(&p2);
	ipecac_free(&ha);
	ipecac_free(&la);
	ipecac_free(&hb);
	ipecac_free(&lb);

	return ret;
}

int ipecac_mul(ipint_t *r, ipint_t *a, ipint_t *b){
	r->bits_used=a->bits_used+b->bits_used;
	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,(r->bits_used/DATA_WIDTH)+1)==IPECAC_ERROR)
			return IPECAC_ERROR;

	return karatsuba_mul(r,a,b);
	//return basic_mul(r,a,b);
}
