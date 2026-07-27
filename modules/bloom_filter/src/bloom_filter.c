#include "../include/bloom_filter.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct bloom_filter {
  size_t num_hashes;
  size_t bit_array_size;
  uint32_t (*hash1)(void *);
  uint32_t (*hash2)(void *);
  uint32_t *bit_array;
} bloom_filter_t;

size_t nth_hash(size_t n, bloom_filter_t *filter, void *item) {
  uint32_t hash = filter->hash1(item) + n * filter->hash2(item);
  return ((uint64_t)hash * filter->bit_array_size) >> 32;
}

bloom_filter_t *new_bloom_filter(size_t num_items, double desired_fp_rate,
                                 uint32_t (*hash1)(void *),
                                 uint32_t (*hash2)(void *)) {

  bloom_filter_t *filter = malloc(sizeof(bloom_filter_t));
  if (filter == NULL) {
    return NULL;
  }

  double num_bits =
      ceil(-(double)num_items * log(desired_fp_rate) / pow(log(2), 2));

  size_t num_words = num_bits / 32 + 1;

  filter->bit_array_size = num_words * 32;
  filter->num_hashes = (size_t)ceil(log(2) * num_bits / (double)num_items);

  filter->hash1 = hash1;
  filter->hash2 = hash2;

  filter->bit_array = calloc(num_words, 4);
  if (filter->bit_array == NULL) {
    return NULL;
  }

  return filter;
}

void bloom_filter_add(bloom_filter_t *filter, void *item) {
  for (size_t i = 0; i < filter->num_hashes; i++) {
    size_t nth = nth_hash(i, filter, item);
    size_t word_idx = nth >> 5;
    size_t bit_idx = nth & 31;

    filter->bit_array[word_idx] |= (uint32_t)1 << bit_idx;
  }
}

int bloom_filter_check(bloom_filter_t *filter, void *item) {
  for (size_t i = 0; i < filter->num_hashes; i++) {
    size_t nth = nth_hash(i, filter, item);
    size_t word_idx = nth >> 5;
    size_t bit_idx = nth & 31;

    if (!((filter->bit_array[word_idx] & ((uint32_t)1 << bit_idx)))) {
      return 0;
    }
  }
  return 1;
}

void bloom_filter_destroy(bloom_filter_t *filter) {
  free(filter->bit_array);
  free(filter);
}
