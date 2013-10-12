#ifndef IPECAC_ARITHMETIC_H
#define IPECAC_ARITHMETIC_H

#include "ipecac.h"

int ipecac_add(ipint_t *r, const ipint_t *a, const ipint_t *b);
int ipecac_sub(ipint_t *r, const ipint_t *a, const ipint_t *b);
int ipecac_mul(ipint_t *r, const ipint_t *a, const ipint_t *b);
int ipecac_div(ipint_t *q, ipint_t *r, const ipint_t *a, const ipint_t *b);

#endif
