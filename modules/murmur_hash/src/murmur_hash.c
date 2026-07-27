#include "../include/murmur_hash.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline uint32_t rotl32(uint32_t x, int8_t r) {
  return (x << r) | (x >> (32 - r));
}

// 32 Bits
uint32_t murmur_hash32(const void *key, size_t len, uint32_t seed) {
  const uint8_t *data = (const uint8_t *)key;
  const int nblocks = (int)(len / 4);

  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  const uint32_t *blocks = (const uint32_t *)(data + nblocks * 4);

  for (int i = -nblocks; i; i++) {
    uint32_t k1;
    memcpy(&k1, blocks + i, sizeof(k1)); // avoid unaligned-access UB

    k1 *= c1;
    k1 = rotl32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1 = rotl32(h1, 13);
    h1 = h1 * 5 + 0xe6546b64;
  }

  const uint8_t *tail = data + nblocks * 4;
  uint32_t k1 = 0;

  switch (len & 3) {
  case 3:
    k1 ^= (uint32_t)tail[2] << 16; /* fallthrough */
  case 2:
    k1 ^= (uint32_t)tail[1] << 8; /* fallthrough */
  case 1:
    k1 ^= (uint32_t)tail[0];
    k1 *= c1;
    k1 = rotl32(k1, 15);
    k1 *= c2;
    h1 ^= k1;
  }

  h1 ^= (uint32_t)len;

  h1 ^= h1 >> 16;
  h1 *= 0x85ebca6b;
  h1 ^= h1 >> 13;
  h1 *= 0xc2b2ae35;
  h1 ^= h1 >> 16;

  return h1;
}

uint32_t murmur_hash32_str(const char *key, uint32_t seed) {
  size_t len = strlen(key);

  return murmur_hash32(key, len, seed);
}

uint32_t murmur_hash32_uint32(const uint32_t *key, uint32_t seed) {
  return murmur_hash32(key, 4, seed);
}

// 64 Bits
uint64_t murmur_hash64(const void *key, size_t len, uint64_t seed) {
  const uint64_t m = 0xc6a4a7935bd1e995ULL;
  const int r = 47;

  uint64_t h = seed ^ (len * m);

  const uint64_t *data = (const uint64_t *)key;
  const uint64_t *end = data + (len / 8);

  // Process 8-byte blocks
  while (data != end) {
    uint64_t k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  // Process remaining bytes
  const unsigned char *data2 = (const unsigned char *)data;

  switch (len & 7) {
  case 7:
    h ^= (uint64_t)data2[6] << 48; // fallthrough
  case 6:
    h ^= (uint64_t)data2[5] << 40; // fallthrough
  case 5:
    h ^= (uint64_t)data2[4] << 32; // fallthrough
  case 4:
    h ^= (uint64_t)data2[3] << 24; // fallthrough
  case 3:
    h ^= (uint64_t)data2[2] << 16; // fallthrough
  case 2:
    h ^= (uint64_t)data2[1] << 8; // fallthrough
  case 1:
    h ^= (uint64_t)data2[0];
    h *= m;
  };

  // Finalize hash mix
  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

uint64_t murmur_hash64_str(const char *key, uint64_t seed) {
  size_t len = strlen(key);

  return murmur_hash64(key, len, seed);
}

uint64_t murmur_hash64_uint64(const uint64_t *key, uint64_t seed) {
  return murmur_hash64(key, 8, seed);
}
