#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef MURMUR_HASH
#define MURMUR_HASH

uint64_t murmur_hash64_str(const char *key, uint64_t seed);

uint64_t murmur_hash64_uint64(const uint64_t *key, uint64_t seed);

uint32_t murmur_hash32_str(const char *key, uint32_t seed);

uint32_t murmur_hash32_uint32(const uint32_t *key, uint32_t seed);

#endif
