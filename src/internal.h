#ifndef IPECAC_INTERNAL_H
#define IPECAC_INTERNAL_H

#include "../include/ipecac.h"

int resize_ipint(ipint_t *s, uint32_t minblocks);
uint32_t int_log2(ipdata_t n);
uint32_t get_num_bits(ipint_t *s, uint32_t offset);

#endif
