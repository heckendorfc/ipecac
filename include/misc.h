#ifndef IPECAC_MISC_H
#define IPECAC_MISC_H

#include "ipecac.h"

int ipecac_init_b(ipint_t*, unsigned int);
int ipecac_init(ipint_t*, int);
int ipecac_set(ipint_t*, int);
int ipecac_clone(ipint_t*, const ipint_t*);
int ipecac_free(ipint_t*);

#endif
