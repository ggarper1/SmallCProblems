#include "../include/murmur_hash.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void test_murmur_hash64_str() {

  uint64_t seed = 42;

  assert(murmur_hash64_str("Hello, World!", seed) == 14813370296679411524ULL);

  assert(murmur_hash64_str("0ndoein0nms02iedneije", seed) ==
         5547075220588178254ULL);

  assert(murmur_hash64_str("Walk in the park", seed) == 8923756905261532861ULL);

  assert(murmur_hash64_str("eid3binkwmd084nr3", seed) ==
         13973210370853256709ULL);

  assert(murmur_hash64_str("skbwidbeod08844/)*)$@$", seed) ==
         1803130296497343249ULL);

  assert(murmur_hash64_str("95bwnwienx93839nnkhwh", seed) ==
         9221034147779666828ULL);
}

void test_murmur_hash64_uint64() {
  uint64_t seed = 42;

  uint64_t input = 0;
  memcpy(&input, "Hello", 5);

  uint64_t hash = murmur_hash64_uint64(&input, seed);
  assert(hash == 6270458304369945523ULL);

  hash = murmur_hash64_uint64(&hash, seed);
  assert(hash == 17626504337409460374ULL);

  hash = murmur_hash64_uint64(&hash, seed);
  assert(hash == 4337624720400496681ULL);

  hash = murmur_hash64_uint64(&hash, seed);
  assert(hash == 402814009646682563ULL);

  hash = murmur_hash64_uint64(&hash, seed);
  assert(hash == 6335593642779021504ULL);

  hash = murmur_hash64_uint64(&hash, seed);
  assert(hash == 4402297641184007905ULL);

  hash = murmur_hash64_uint64(&hash, seed);
  assert(hash == 15870452706708948939ULL);
}

void test_murmur_hash32_str() {

  uint32_t seed = 42;

  assert(murmur_hash32_str("Hell", seed) == 3505326030);

  assert(murmur_hash32_str("0ndo", seed) == 3713415773);

  assert(murmur_hash32_str("Walk", seed) == 377314908);

  assert(murmur_hash32_str("eid3", seed) == 3905233526);

  assert(murmur_hash32_str("skbw", seed) == 1184101196);

  assert(murmur_hash32_str("95bw", seed) == 3324069167);
}

void test_murmur_hash32_uint32() {
  uint32_t seed = 42;

  uint32_t input = 0;
  memcpy(&input, "Hell", 4);

  uint32_t hash = murmur_hash32_uint32(&input, seed);
  assert(hash == 3505326030);

  hash = murmur_hash32_uint32(&hash, seed);
  assert(hash == 1089728007);

  hash = murmur_hash32_uint32(&hash, seed);
  assert(hash == 2152926454);

  hash = murmur_hash32_uint32(&hash, seed);
  assert(hash == 3553049396);

  hash = murmur_hash32_uint32(&hash, seed);
  assert(hash == 1467818681);

  hash = murmur_hash32_uint32(&hash, seed);
  assert(hash == 4225190786);

  hash = murmur_hash32_uint32(&hash, seed);
  assert(hash == 3269945050);
}

int main(int argc, char *argv[]) {
  test_murmur_hash64_str();
  test_murmur_hash64_uint64();

  test_murmur_hash32_str();
  test_murmur_hash32_uint32();

  printf("✅ All tests passed!\n");
}
