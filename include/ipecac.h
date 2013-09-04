#ifndef IPECAC_H
#define IPECAC_H

#include <stdint.h>

#define IPECAC_SUCCESS 0
#define IPECAC_ERROR 1

#define SIGN_POS 0
#define SIGN_NEG 1

#define INITIAL_BLOCK_SIZE 4
#define RESIZE_BLOCK_SIZE 4

#if UINTPTR_MAX == 0xffffffff
#define DATA_WIDTH 32
typedef uint32_t ipdata_t;
typedef uint16_t half_ipdata_t;
#define LOW_HALF(x) ((x)&0x0000FFFF)
#elif UINTPTR_MAX == 0xffffffffffffffff
#define DATA_WIDTH 64
typedef uint64_t ipdata_t;
typedef uint32_t half_ipdata_t;
#define LOW_HALF(x) ((x)&0x00000000FFFFFFFF)
#else
#error "Unsupported platform."
#endif

#define HIGH_HALF(x) ((x)>>(DATA_WIDTH/2))
#define UNHIGH_HALF(x) ((x)<<(DATA_WIDTH/2))
typedef struct ipint_t{
	uint8_t sign;
	ipdata_t *data;
	uint32_t bits_allocated;
	uint32_t bits_used;
}ipint_t;

#endif
