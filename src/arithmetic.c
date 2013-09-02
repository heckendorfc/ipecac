#include "../include/ipecac.h"
#include "../include/arithmetic.h"
#include "../include/misc.h"
#include "../include/util.h"
#include "../include/bitwise.h"
#include "internal.h"

int basic_add(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->bits_used;
	uint32_t ib = b->bits_used;
	uint32_t rused=r->bits_used;
	int i,sj,j,k,carrybit;
	ipint_t *large,*small;
	ipdata_t tmp;

	sj=(ib-1)/DATA_WIDTH;
	j=(ia-1)/DATA_WIDTH;
	large=a;
	small=b;
	r->bits_used=ia;

	if(rused>r->bits_used){
		for(i=r->bits_used+DATA_WIDTH;i<rused;i+=DATA_WIDTH)
			r->data[i/DATA_WIDTH]=0;
		r->data[r->bits_used/DATA_WIDTH]&=(~0)>>(r->bits_used%DATA_WIDTH);
	}

	carrybit=((r->bits_used-1)+DATA_WIDTH)/DATA_WIDTH;
	carrybit*=DATA_WIDTH;
	r->bits_used++;

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,(r->bits_used+DATA_WIDTH)/DATA_WIDTH)==IPECAC_ERROR)
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
		r->data[carrybit/DATA_WIDTH]|=k<<(carrybit%DATA_WIDTH);
		r->bits_used=carrybit+get_num_bits(r,carrybit/DATA_WIDTH);
	}
	else{
		r->bits_used=carrybit-DATA_WIDTH+get_num_bits(r,j);
	}

	return IPECAC_SUCCESS;
}

static int basic_sub(ipint_t *r, ipint_t *a, ipint_t *b){
	uint32_t ia = a->bits_used;
	uint32_t ib = b->bits_used;
	uint32_t rused=r->bits_used;
	int i,j,k;
	ipint_t *large,*small;
	ipdata_t tmp;

	j=(ia-1)/DATA_WIDTH;
	large=a;
	small=b;
	r->bits_used=ia;

	if(rused>r->bits_used){
		for(i=r->bits_used+DATA_WIDTH;i<rused;i+=DATA_WIDTH)
			r->data[i/DATA_WIDTH]=0;
		r->data[r->bits_used/DATA_WIDTH]&=(~0)>>(r->bits_used%DATA_WIDTH);
	}

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,(r->bits_used+DATA_WIDTH)/DATA_WIDTH)==IPECAC_ERROR)
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

	for(i=j;i>=0 && r->data[i]==0;i--);
	if(i>=0)
		r->bits_used=i*DATA_WIDTH+get_num_bits(r,i);
	else
		r->bits_used=1;

	return IPECAC_SUCCESS;
}

int ipecac_sub(ipint_t *r, ipint_t *a, ipint_t *b){
	int as=a->sign;
	int bs=b->sign;
	int c;
	int ret;

	/* TODO: Make an abs_cmp function */
	a->sign=SIGN_POS;
	b->sign=SIGN_POS;

	c=ipecac_cmp(a,b);

	a->sign=as;
	b->sign=bs;

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
	a->sign=SIGN_POS;
	b->sign=SIGN_POS;

	c=ipecac_cmp(a,b);

	a->sign=as;
	b->sign=bs;

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
	int i;
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

	m=((a->bits_used-1)/(DATA_WIDTH/2)+2);
	n=((b->bits_used-1)/(DATA_WIDTH/2)+1);

	ipecac_init_b(&u,m);

	if(n==1){
		sq=(half_ipdata_t*)q->data;
		sr=(half_ipdata_t*)r->data;
		sa=(half_ipdata_t*)a->data;
		sb=(half_ipdata_t*)b->data;
		j=single_div(sq,sr,sa,m,sb[0]);
		m=m/2-2;
		if(m>0 && q->data[m]==0)
			m--;
		q->bits_used=m*DATA_WIDTH+get_num_bits(q,m);
		r->bits_used=get_num_bits(r,0);
		return j;
	}
	//if(a->bits_used+DATA_WIDTH<a->bits_allocated

	/* D1: Normalize */
	i=(1<<(DATA_WIDTH/2))/(LOW_HALF(b->data[n])+1);
	for(f=0;i;f++)i>>=1;
	f--;
	ipecac_bit_lshift(&u,a,f);
	ipecac_bit_lshift(r,b,f);

	sq=(half_ipdata_t*)q->data;
	sa=(half_ipdata_t*)u.data;
	sb=(half_ipdata_t*)r->data;

	while(sb[n-1]==0)n--;
	m=m-n;


	ipecac_set(q,0);

	/* D2: Init j */
	for(j=0;j<m;j++){
		/* D3: Calculate ^q */
		if(sa[m+n-1-j]==sb[n-1]){
			ns=(half_ipdata_t)(~0);
		}
		else{
			ns=((sa[m+n-1-j]<<(DATA_WIDTH/2))|sa[m+n-1-j-1])/sb[n-1];
		}
		do{
			ni.bits_used=get_num_bits(&ni,0);
			nt=sb[n-2];
			nj.bits_used=get_num_bits(&nj,0);
			ipecac_mul(&d,&ni,&nj);

			nt=(sa[m+n-1-j]<<(DATA_WIDTH/2))|sa[m+n-1-j-1];
			nj.bits_used=get_num_bits(&nj,0);

			ipecac_sub(&dq,&nj,&d);
			ipecac_bit_lshift(&dq,&dq,DATA_WIDTH/2);
			nt=sa[m+n-1-j-2];
			ipecac_bit_or(&dq,&dq,&nj);
			c=ipecac_cmp(&d,&dq);
			if(c>0)
				ns--;
		}while(c>0);

		/* D4: Multiply and subtract */
		ipecac_mul(&d,r,&ni);
		i=(m+n-1)-((d.bits_used-1)/(DATA_WIDTH/2)+1); // Alignment
		if(i)
			ipecac_bit_lshift(&d,&d,i*(DATA_WIDTH/2));
		if((c=ipecac_cmp(&u,&d))>=0)
			ipecac_sub(&u,&u,&d);

		/* D5: Test remainder */
		sq[m-1-j]=ns;
		if(c<0){
			/* D6: Add back */
			sq[m-j]--;
			ipecac_add(&d,&u,r);
			for(i=0;i<n;i++) // To skip the carry
				u.data[i]=d.data[i];
			u.bits_used=(DATA_WIDTH*(n-1))+get_num_bits(&u,n-1);
		}

		/* D7: Loop on j */
	}
	i=m/2-1;
	q->bits_used=i*DATA_WIDTH+get_num_bits(q,i);
	ipecac_bit_rshift(r,&u,f);
	r->bits_used=(n-2)*DATA_WIDTH+get_num_bits(r,n-2);

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
	int qi,ri;
	if((a->bits_used<=DATA_WIDTH && a->data[0]==0) || (b->bits_used<=DATA_WIDTH && b->data[0]==0)){
		ipecac_set(q,0);
		ipecac_set(r,0);
		return IPECAC_SUCCESS;
	}
	else if(a->bits_used<=DATA_WIDTH && b->bits_used<=DATA_WIDTH){
		qi=(a->data[0])/(b->data[0]);
		ri=(a->data[0])%(b->data[0]);
		ipecac_set(q,qi);
		ipecac_set(r,ri);
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


	r->bits_used=a->bits_used+b->bits_used;

	if(r->bits_used>r->bits_allocated)
		if(resize_ipint(r,(r->bits_used+DATA_WIDTH)/DATA_WIDTH)==IPECAC_ERROR)
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

	for(j=(r->bits_used-1)/DATA_WIDTH+1;j>=0 && r->data[j]==0;j--);
	if(j>=0)
		r->bits_used=j*DATA_WIDTH+get_num_bits(r,j);
	else
		r->bits_used=1;

	return IPECAC_SUCCESS;
}

static void split_ipint(ipint_t *l, ipint_t *h, ipint_t *i){
	int j;
	const uint32_t mid=((i->bits_used/DATA_WIDTH)+1)/2;
	const uint32_t end=((i->bits_used/DATA_WIDTH)+1)-mid;

	for(j=0;j<mid+end;j++){
		l->data[j]=0;
		h->data[j]=0;
	}
	for(j=0;j<mid;j++){
		l->data[j]=i->data[j];
	}
	l->bits_used=DATA_WIDTH*mid;
	for(j=0;j<end;j++){
		h->data[j]=i->data[mid+j];
	}
	h->bits_used=DATA_WIDTH*(end);

	if(l->bits_used==0)l->bits_used=1;
	if(h->bits_used==0)h->bits_used=1;
}

static int karatsuba_mul(ipint_t *r, ipint_t *a, ipint_t *b){
	int i;
	ipint_t r1,r2;
	ipint_t ha,la,hb,lb;
	ipint_t p0,p1,p2;
	const uint32_t asize=(a->bits_used/DATA_WIDTH)+1;
	const uint32_t bsize=(b->bits_used/DATA_WIDTH)+1;
	const uint32_t m=(asize>bsize?asize:bsize)-1;
	int ret=0;

	if(a->bits_used<=DATA_WIDTH){
		return basic_mul(r,a,b);
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
	uint32_t newsize=a->bits_used+b->bits_used;
	int ret=0;
	int i;
	ipint_t hold;
	ipint_t *or=r;

	if(a->bits_used==1 || b->bits_used==1){
		if(a->data[0]==0 || b->data[0]==0){
			ipecac_set(r,0);
		}
		else if(a->data[0]==1){
			ipecac_clone(r,b);
		}
		else{
			ipecac_clone(r,a);
		}
		return IPECAC_SUCCESS;
	}

	if(newsize>r->bits_allocated)
		if(resize_ipint(r,((newsize+DATA_WIDTH)/DATA_WIDTH)+1)==IPECAC_ERROR)
			return IPECAC_ERROR;

	if(r==a || r==b){ // workaround for x=x*y, etc
		r=&hold;
		ipecac_init(&hold,0);
		ipecac_clone(&hold,or);
	}

	ret=karatsuba_mul(r,a,b);

	if(r==&hold){
		ipecac_clone(or,&hold);
		ipecac_free(&hold);
	}

	if(ret==IPECAC_SUCCESS){
		for(i=(r->bits_allocated-1)/DATA_WIDTH;i>=0 && r->data[i]==0;i--);
		if(i>=0)
			r->bits_used=i*DATA_WIDTH+get_num_bits(r,i);
		else
			r->bits_used=1;
	}

	return IPECAC_SUCCESS;
}
