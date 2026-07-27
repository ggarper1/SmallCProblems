#include <stddef.h>
#include <stdint.h>

#ifndef BLOOM_FILTER
#define BLOOM_FILTER

typedef struct bloom_filter bloom_filter_t;

bloom_filter_t *new_bloom_filter(size_t num_items, double desired_fp_rate,
                                 uint32_t (*hash1)(void *),
                                 uint32_t (*hash2)(void *));

void bloom_filter_add(bloom_filter_t *filter, void *item);

int bloom_filter_check(bloom_filter_t *filter, void *item);

void bloom_filter_destroy(bloom_filter_t *filter);

#endif
