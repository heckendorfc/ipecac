#ifndef IPECAC_BITWISE_H
#define IPECAC_BITWISE_H

#include "ipecac.h"

int ipecac_bit_and(ipint_t*, ipint_t*, ipint_t*);
int ipecac_bit_or(ipint_t*, ipint_t*, ipint_t*);
int ipecac_bit_xor(ipint_t*, ipint_t*, ipint_t*);
int ipecac_bit_lshift(ipint_t*, ipint_t*, ipint_t*);
int ipecac_bit_rshift(ipint_t*, ipint_t*, ipint_t*);
int ipecac_bit_complement(ipint_t*, ipint_t*, ipint_t*);

#endif
