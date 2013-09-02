#ifndef IPECAC_CONVERT_H
#define IPECAC_CONVERT_H

#include "../include/ipecac.h"

long ipecac_get_long(ipint_t *s);
int ipecac_set_str(ipint_t *s, const char *str, int base);
int ipecac_get_str(ipint_t *s, char **str, int base);

#endif
