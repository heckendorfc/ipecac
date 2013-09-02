#include "../include/ipecac.h"
#include "../include/util.h"

int ipecac_cmp(ipint_t *a, ipint_t *b){
	int i;
	int ret=0;
	if(a->sign!=b->sign){
		if(a->sign==SIGN_POS)
			return 1;
		else
			return -1;
	}
	if(a->bits_used>b->bits_used)
		ret=1;
	else if(b->bits_used>a->bits_used)
		ret=-1;
	else{
		for(i=(a->bits_used-1)/DATA_WIDTH;i>=0;i--){
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

	if(ret && a->sign==SIGN_NEG)
		ret=-ret;

	return ret;
}
