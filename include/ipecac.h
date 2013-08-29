#ifndef IPECAC_H
#define IPECAC_H

#include <stdint.h>

#define IPECAC_SUCCESS 0
#define IPECAC_ERROR 1

#define SIGN_POS 0
#define SIGN_NEG 1

#define INITIAL_BLOCK_SIZE 4
#define RESIZE_BLOCK_SIZE 4

#define DATA_WIDTH 32
typedef uint32_t ipdata_t;
typedef struct ipint_t{
	uint8_t sign;
	ipdata_t *data;
	uint32_t bits_allocated;
	uint32_t bits_used;
}ipint_t;

#endif
