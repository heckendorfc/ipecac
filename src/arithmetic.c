#include <stdlib.h>
#include "../include/ipecac.h"
#include "../include/arithmetic.h"
#include "../include/misc.h"
#include "../include/util.h"
#include "../include/bitwise.h"
#include "internal.h"

int basic_add(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->used;
	uint32_t ib = b->used;
	uint32_t rused=r->used;
	int i,sj,j,k,carrybit;
	ipint_t *large,*small;
	ipdata_t tmp;

	sj=(ib-1);
	j=(ia-1);
	large=a;
	small=b;
	r->used=ia;

	if(rused>r->used){
		for(i=r->used;i<rused;i++)
			r->data[i]=0;
		//r->data[r->used-1]&=(~0)>>(r->bits_used%DATA_WIDTH);
	}

	carrybit=r->used+1;
	r->used++;

	if(r->used>=r->allocated)
		if(resize_ipint(r,r->used)==IPECAC_ERROR)
			return IPECAC_ERROR;

	k=0;

	for(i=0;i<=sj;i++){
		// Set at tmp in case r==large || r==small
		tmp=large->data[i]+small->data[i]+k;
		if(tmp<large->data[i] && tmp<small->data[i])
			k=1;
		else
			k=0;
		r->data[i]=tmp;
	}

	for(;i<=j;i++){
		// Set at tmp in case r==large || r==small
		tmp=large->data[i]+k;
		if(tmp<large->data[i])
			k=1;
		else
			k=0;
		r->data[i]=tmp;
	}

	if(k){
		r->data[carrybit]|=k<<carrybit;
		r->used=carrybit;
	}
	else{
		r->used=carrybit-1;
	}

	return IPECAC_SUCCESS;
}

static int basic_sub(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->used;
	uint32_t ib = b->used;
	uint32_t rused=r->used;
	int i,j,k;
	ipint_t *large,*small;
	ipdata_t tmp;

	j=ia;
	large=a;
	small=b;
	r->used=ia;

	if(rused>r->used){
		for(i=r->used;i<rused;i++)
			r->data[i]=0;
		//r->data[r->used]&=(~0)>>(r->bits_used%DATA_WIDTH);
	}

	if(r->used>r->allocated)
		if(resize_ipint(r,r->used)==IPECAC_ERROR)
			return IPECAC_ERROR;

	k=0;

	for(i=0;i<j;i++){
		// Set at tmp in case r==large
		tmp=large->data[i]-small->data[i]-k;
		if(tmp>large->data[i])
			k=1;
		else
			k=0;
		r->data[i]=tmp;
	}

	for(i=j-1;i>0 && r->data[i]==0;i--);
	if(i>0)
		r->used=i+1;
	else
		r->used=1;

	return IPECAC_SUCCESS;
}

int ipecac_sub(ipint_t *r, ipint_t *a, ipint_t *b){
	int as=a->sign;
	int bs=b->sign;
	int c;
	int ret;

	/* TODO: Make an abs_cmp function */
	//a->sign=SIGN_POS;
	//b->sign=SIGN_POS;

	c=ipecac_abs_cmp(a,b);

	//a->sign=as;
	//b->sign=bs;

	if(as==bs){
		if(c>=0){
			ret=basic_sub(r,a,b);
			r->sign=as;
		}
		else{
			ret=basic_sub(r,b,a);
			r->sign=!as;
		}
	}
	else{
		if(c>=0)
			ret=basic_add(r,a,b);
		else
			ret=basic_add(r,b,a);
		r->sign=as;
	}

	return ret;
}

int ipecac_add(ipint_t *r, ipint_t *a, ipint_t *b){
	int as=a->sign;
	int bs=b->sign;
	int c;
	int ret;

	/* TODO: Make an abs_cmp function */
	//a->sign=SIGN_POS;
	//b->sign=SIGN_POS;

	c=ipecac_abs_cmp(a,b);

	//a->sign=as;
	//b->sign=bs;

	if(as==bs){
		if(c>=0)
			ret=basic_add(r,a,b);
		else
			ret=basic_add(r,b,a);
		r->sign=as;
	}
	else{
		if(c>=0){
			ret=basic_sub(r,a,b);
			r->sign=as;
		}
		else{
			ret=basic_sub(r,b,a);
			r->sign=bs;
		}
	}

	return ret;
}


/*
static int roundup(ipint_t *a, unsigned int p){

}

static int reciprocal_rec(ipint_t *r, ipint_t *a, unsigned int p, ipint_t *t, ipint_t *s){
	int ret;
	if(p>2){
		ret=reciprocal_rec(r,a,(p>>1)+1,t,s); // r: z
		ret|=ipecac_clone(t,r); // t: z
		ret|=ipecac_clone(s,r); // s: z

		ret|=ipecac_lshift(r,t,1); // r: 2z

		ret|=truncate(t,x,p+2); // t: trunc(x,p+2)

		ret|=ipecac_mul(t,t,s); // r: trunc(x,p+2)*z
		ret|=ipecac_mul(t,t,s); // r: trunc(x,p+2)*z^2

		ret|=ipecac_sub(r,r,t); // r: 2z-trunc(x,p+2)*z^2
		if(ret==IPECAC_ERROR)
			return ret;

		return roundup(r,p);
	}
	else{
		if(p==2)
			return ipecac_set(t,150);
		else
			return ipecac_set(t,15);
	}
}

static int reciprocal(ipint_t *r, ipint_t *a, unsigned int p){
	int ret;
	ipdat_t tmp1,tmp2;

	ipecac_init(&tmp1,0);
	ipecac_init(&tmp2,0);

	ret=reciprocal_rec(r,a,p,&tmp1,&tmp2);

	ipecac_free(&tmp1);
	ipecac_free(&tmp2);

	return ret;
}
*/

/*
int basic_div(ipint_t *q, ipint_t *r, ipint_t *qp, ipint_t *rp, ipint_t *a, ipint_t *b, ipint_t *ap, ipint_t *t){
	uint32_t ind;
	uint32_t m,n;

	m=(a->bits_used-1)/DATA_WIDTH+1;
	n=(b->bits_used-1)/DATA_WIDTH+1;

	if(m<n){
		ipecac_set(q,0);
		ipecac_clone(r,a);
		return IPECAC_SUCCESS;
	}
	else if(m==n){
		ind=n-1;
		if(a->data[ind]<b->data[ind]){
			ipecac_set(q,0);
			ipecac_clone(r,a);
			return IPECAC_SUCCESS;
		}
		else{
			ipecac_set(q,1);
			ipecac_sub(r,a,b);
			return IPECAC_SUCCESS;
		}
	}
	else if(m==n+1){
		return apb_div(r,a,b);
	}

	ipecac_bit_rshift(ap,a,DATA_WIDTH*(m-n-1));
	apb_div(qp,rp,ap,b);
	basic_div(q,r,qp,rp,a,b,ap,t);

	ipecac_lshift(qp,qp,DATA_WIDTH*(m-n-1));
	ipecac_add(q,q,qp); // OR this?

	return IPECAC_SUCCESS;
}
*/

/* Divide a by single digit b */
/*
static int simple_div(ipint_t *q, ipint_t *a, ipdata_t b){
	uint32_t m = (a->bits_used-1)/DATA_WIDTH+1;
	int i;

	for(i=m;i>=0;i--){
		tmp=carry*
	}
}
*/

static int single_div(half_ipdata_t *q, half_ipdata_t *r, half_ipdata_t *a, uint32_t m, half_ipdata_t b){
	int j;
	ipdata_t carry=0;
	ipdata_t tmp=0;

	for(j=m-1;j>=0;j--){
		tmp=(carry<<(DATA_WIDTH/2))+a[j];
		q[j]=tmp/b;
		carry=tmp%b;
	}

	r[0]=carry;

	return IPECAC_SUCCESS;
}

int knuth_div(ipint_t *q, ipint_t *r, ipint_t *a, ipint_t *b){
	ipint_t u,d,dq;
	ipint_t ni,nj;
	uint32_t m,n;
	ipdata_t ns,nt;
	ipdata_t i;
	int j;
	int c;
	uint32_t f;
	half_ipdata_t *sq;
	half_ipdata_t *sr;
	half_ipdata_t *sa;
	half_ipdata_t *sb;

	ni.data=&ns;
	ni.sign=SIGN_POS;
	nj.data=&nt;
	nj.sign=SIGN_POS;

	ipecac_init(&d,0);
	ipecac_init(&dq,0);

	m=(a->used)*2;
	n=(b->used)*2;

	sa=(half_ipdata_t*)a->data;
	sb=(half_ipdata_t*)b->data;

	if(sa[m-1]==0)m--;
	if(sb[n-1]==0)n--;
	m++;

	ipecac_init_b(&u,m);

	if(n==1){
		sq=(half_ipdata_t*)q->data;
		sr=(half_ipdata_t*)r->data;
		sa=(half_ipdata_t*)a->data;
		sb=(half_ipdata_t*)b->data;
		j=single_div(sq,sr,sa,m-1,sb[0]);
		m=a->used-1;
		if(m>0 && q->data[m]==0)
			m--;
		q->used=m+1;
		r->used=1;
		return j;
	}
	//if(a->bits_used+DATA_WIDTH<a->bits_allocated

	/* D1: Normalize */
	i=((half_ipdata_t)~0)/(HIGH_HALF(b->data[(n-1)/2])+1);
	//i=((ipdata_t)1<<(DATA_WIDTH/2))/(LOW_HALF(b->data[n])+1);
	for(f=0;i;f++)i>>=1;
	f--;
	ipecac_bit_lshift(&u,a,f);
	ipecac_bit_lshift(r,b,f);

	sq=(half_ipdata_t*)q->data;
	sa=(half_ipdata_t*)u.data;
	sb=(half_ipdata_t*)r->data;

	while(sb[n-1]==0)n--;
	m=m-n;

	ni.used=1;
	nj.used=1;

	ipecac_set(q,0);

	/* D2: Init j */
	for(j=m+n-1;j>=n;j--){
		/* D3: Calculate ^q */
		if(sa[j]==sb[n-1]){
			ns=(half_ipdata_t)(~0);
		}
		else{
			ns=(((ipdata_t)sa[j]<<(DATA_WIDTH/2))|sa[j-1])/sb[n-1];
		}
		do{
			nt=sb[n-2];
			ipecac_mul(&d,&ni,&nj);

			nt=((ipdata_t)sa[j]<<(DATA_WIDTH/2))|sa[j-1];

			ipecac_sub(&dq,&nj,&d);
			ipecac_bit_lshift(&dq,&dq,DATA_WIDTH/2);
			nt=sa[j-2];
			ipecac_bit_or(&dq,&dq,&nj);
			c=ipecac_cmp(&d,&dq);
			if(c>0)
				ns--;
		}while(c>0);

		/* D4: Multiply and subtract */
		ipecac_mul(&d,r,&ni);
		i=(j)-(n); // Alignment
		if(i>0)
			ipecac_bit_lshift(&d,&d,i*(DATA_WIDTH/2));
		if((c=ipecac_cmp(&u,&d))>=0)
			ipecac_sub(&u,&u,&d);

		/* D5: Test remainder */
		sq[j-n]=ns;
		if(c<0){
			/* D6: Add back */
			sq[j-n+1]--;
			ipecac_add(&d,&u,r);
			for(i=0;i<n/2;i++) // To skip the carry
				u.data[i]=d.data[i];
			//u.used=n-1;
		}

		/* D7: Loop on j */
	}
	j=m/2;
	q->used=j;
	ipecac_bit_rshift(r,&u,f);
	r->used=n-1;

	ipecac_free(&u);
	ipecac_free(&d);
	ipecac_free(&dq);

	return IPECAC_SUCCESS;
}

/*
int basic_div(ipint_t *q, ipint_t *r, ipint_t *a, ipint_t *b){
	ipint_t d,dq;
	ipint_t ni;
	uint32_t m,n;
	ipdata_t nt;
	int k;

	ni.data=&nt;

	ipecac_init(d,0);
	ipecac_init(dq,0);

	m=(a->bits_used-1)/DATA_WIDTH+1;
	n=(b->bits_used-1)/DATA_WIDTH+1;

	ipecac_lshift(r,a,DATA_WIDTH/2);
	ipecac_lshift(d,b,DATA_WIDTH/2);

	ipecac_set(q,0);

	for(k=n-m;k>=0;k--){
		nt=trialdigit(r,d,k,m);
		basic_mul(dq,d,ni);
		if(smaller(r,dq,k,m)){
			nt--;
			basic_mul(dq,d,ni);
		}
		q->data[k]=nt;
		sub(r,r,dq);
	}

	ipecac_rshift(r,a,DATA_WIDTH/2);
}
*/

int ipecac_div(ipint_t *q, ipint_t *r, ipint_t *a, ipint_t *b){
	int c;
	ipdata_t qi,ri;
	if((a->used<=1 && a->data[0]==0) || (b->used<=1 && b->data[0]==0)){
		ipecac_set(q,0);
		ipecac_set(r,0);
		return IPECAC_SUCCESS;
	}
	else if(a->used<=1 && b->used<=1){
		qi=(a->data[0])/(b->data[0]);
		ri=(a->data[0])%(b->data[0]);
		q->data[0]=qi;
		r->data[0]=ri;
		q->used=1;
		r->used=1;
		return IPECAC_SUCCESS;
	}
	c=ipecac_cmp(a,b);
	if(c==0){
		ipecac_set(q,1);
		ipecac_set(r,0);
		return IPECAC_SUCCESS;
	}
	else if(c<0){
		ipecac_set(q,0);
		ipecac_clone(r,a);
		return IPECAC_SUCCESS;
	}

	return knuth_div(q,r,a,b);
}

static int basic_mul(ipint_t *r, ipint_t *a, ipint_t *b){
	int i,j;
	ipdata_t tmp;
	ipdata_t k;
	half_ipdata_t *sa=(half_ipdata_t*)a->data;
	half_ipdata_t *sb=(half_ipdata_t*)b->data;
	half_ipdata_t *sr=(half_ipdata_t*)r->data;


	r->used=a->used+b->used;

	if(r->used>r->allocated)
		if(resize_ipint(r,r->used)==IPECAC_ERROR)
			return IPECAC_ERROR;


	for(j=0;j<r->used;j++)
		r->data[j]=0;

	for(j=0;j<b->used*2;j++){
		/* // Good for small DATA_WIDTH
		if(sb[j]==0){
			sr[j]=0;
			continue;
		}
		*/
		k=0;
		for(i=0;i<a->used*2;i++){
			tmp=(ipdata_t)sa[i]*(ipdata_t)sb[j]+(ipdata_t)sr[i+j]+k;
			k=HIGH_HALF(tmp);
			sr[i+j]=tmp;
		}
		sr[i+j]=k;
	}

	for(j=r->used-1;j>0 && r->data[j]==0;j--);
	if(j>=0)
		r->used=j+1;
	else
		r->used=1;

	return IPECAC_SUCCESS;
}

static void split_ipint(ipint_t *l, ipint_t *h, ipint_t *i){
	int j;
	const uint32_t mid=(i->used+1)/2;
	const uint32_t end=(i->used)-mid;

	for(j=0;j<mid+end;j++){
		l->data[j]=0;
		h->data[j]=0;
	}
	for(j=0;j<mid;j++){
		l->data[j]=i->data[j];
	}
	l->used=mid;
	for(j=0;j<end;j++){
		h->data[j]=i->data[mid+j];
	}
	h->used=end;

	if(l->used==0)l->used=1;
	if(h->used==0)h->used=1;
}

static void split_ipint_fake(ipint_t *l, ipint_t *h, ipint_t *i, ipdata_t *zero){
	int j;
	const uint32_t mid=(i->used)/2;
	const uint32_t end=(i->used)-mid;

	l->sign=SIGN_POS;
	l->data=i->data;
	l->used=mid;
	l->allocated=mid;
	h->sign=SIGN_POS;
	h->data=i->data+mid;
	h->used=end;
	h->allocated=end;

	if(l->used==0){
		l->used=1;
		l->data=zero;
	}
	if(h->used==0){
		h->used=1;
		h->data=zero;
	}
}

static ipdata_t* quick_init(ipint_t *s, uint32_t size, ipdata_t *d){
	s->sign=SIGN_POS;
	s->allocated=size;
	s->used=1;
	s->data=d;
	s->data[0]=0;
	return d+size;
}

static int karatsuba_mul(ipint_t *r, ipint_t *a, ipint_t *b, ipdata_t *d){
	int i;
	ipint_t r1,r2;
	ipint_t ha,la,hb,lb;
	ipint_t p0,p1,p2;
	const uint32_t asize=a->used;
	const uint32_t bsize=b->used;
	const uint32_t m=(asize>bsize?asize:bsize);
	const uint32_t n=(m+1)/2;
	const uint32_t sum=asize+bsize;
	const uint32_t hsum=(sum+1)/2;
	int ret=0;
	ipdata_t zero=0;

	if(asize<=1 || bsize<=1){
		return basic_mul(r,a,b);
	}

	d=quick_init(&r1,sum,d);
	d=quick_init(&r2,sum,d);
	d=quick_init(&p0,(sum-hsum)*2,d);
	d=quick_init(&p1,sum+(sum-hsum),d);
	d=quick_init(&p2,(sum-hsum)*4,d);
	/*
	ret|=ipecac_init_b(&r1,asize);
	ret|=ipecac_init_b(&r2,bsize);
	ret|=ipecac_init_b(&p0,asize+bsize);
	ret|=ipecac_init_b(&p1,m*(asize+bsize));
	ret|=ipecac_init_b(&p2,2*m*(asize+bsize));
	*/
	split_ipint_fake(&la,&ha,a,&zero);
	split_ipint_fake(&lb,&hb,b,&zero);

	ret|=karatsuba_mul(&p0,&la,&lb,d); // low*low
	ret|=ipecac_add(&r1,&la,&ha);
	ret|=ipecac_add(&r2,&lb,&hb);
	ret|=karatsuba_mul(&p1,&r1,&r2,d); // (low_a+high_a)(low_b+high_b)
	ret|=karatsuba_mul(&p2,&ha,&hb,d); // high*high

	if(ret)
		return ret;

	ret|=ipecac_sub(&p1,&p1,&p2);
	ret|=ipecac_sub(&p1,&p1,&p0);

	ipecac_bit_lshift(&p1,&p1,(n-1)*DATA_WIDTH);
	ipecac_bit_lshift(&p2,&p2,(2*(n-1))*DATA_WIDTH);
#if 0
	/* Replace with shift? */
	for(i=asize+bsize;i>=0;i--){
		p2.data[2*m+i]=p2.data[i];
		p1.data[m+i]=p1.data[i];
	}
	for(i=0;i<2*m;i++)
		p2.data[i]=0;
	for(i=0;i<m;i++)
		p1.data[i]=0;


	p2.bits_used+=2*m*DATA_WIDTH;
	p1.bits_used+=m*DATA_WIDTH;
#endif

	ret|=ipecac_add(r,&p1,&p0);
	ret|=ipecac_add(r,r,&p2);

	/*
	ipecac_free(&r1);
	ipecac_free(&r2);
	ipecac_free(&p0);
	ipecac_free(&p1);
	ipecac_free(&p2);
	*/
	return ret;
}

int ipecac_mul(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t newsize=a->used+b->used;
	int ret=0;
	int i;
	ipint_t hold;
	ipint_t *or=r;
	ipdata_t *d;

	if(a->used==1 || b->used==1){
		if(a->data[0]==0 || b->data[0]==0){
			return ipecac_set(r,0);
		}
		else if(a->data[0]==1){
			return ipecac_clone(r,b);
		}
		else if(b->data[0]==1){
			return ipecac_clone(r,a);
		}
	}

	if(r==a || r==b){ // workaround for x=x*y, etc
		r=&hold;
		ipecac_init(&hold,0);
		ipecac_clone(&hold,or);
	}

	if(newsize>r->allocated)
		if(resize_ipint(r,newsize)==IPECAC_ERROR)
			return IPECAC_ERROR;

	if(0 && a->used>5 && b->used>5){ // Change this threshold
		d=malloc(sizeof(*d)*3*(4*newsize+2+(3*(int)(newsize/2))));
		if(d==NULL)
			return IPECAC_ERROR;
		ret=karatsuba_mul(r,a,b,d);
		free(d);
	}
	else
		ret=basic_mul(r,a,b);

	if(r==&hold){
		ipecac_clone(or,&hold);
		ipecac_free(&hold);
	}
	/*
	if(ret==IPECAC_SUCCESS){
		for(i=newsize-1;i>=0 && or->data[i]==0;i--);
		if(i>=0)
			or->used=i+1;
		else
			or->used=1;
	}
*/
	return IPECAC_SUCCESS;
}
