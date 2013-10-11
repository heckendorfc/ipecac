#include "../include/ipecac.h"
#include "../include/util.h"

int ipecac_abs_cmp(ipint_t *a, ipint_t *b){
	int i;
	int ret=0;

	if(a->used>b->used)
		ret=1;
	else if(b->used>a->used)
		ret=-1;
	else{
		for(i=a->used-1;i>=0;i--){
			if(a->data[i]>b->data[i]){
				ret=1;
				break;
			}
			else if(b->data[i]>a->data[i]){
				ret=-1;
				break;
			}
		}
	}

	return ret;
}

int ipecac_cmp(ipint_t *a, ipint_t *b){
	int i;
	int ret=0;
	if(a->sign!=b->sign){
		if(a->sign==SIGN_POS)
			return 1;
		else
			return -1;
	}

	ret=ipecac_abs_cmp(a,b);

	if(ret && a->sign==SIGN_NEG)
		ret=-ret;

	return ret;
}
