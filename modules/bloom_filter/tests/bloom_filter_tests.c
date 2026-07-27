#include "../../binary_tree/include/binary_tree.h"
#include "../../murmur_hash/include/murmur_hash.h"
#include "../include/bloom_filter.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

const int num_tests = 1000;

int rand_int(int min, int max) {
  int range = max - min + 1;
  return (rand() % range) + min;
}

int compare(const void *item1, const void *item2) {
  const int *a = (const int *)item1;
  const int *b = (const int *)item2;

  if (*a < *b) {
    return -1;
  } else if (*b < *a) {
    return 1;
  } else {
    return 0;
  }
}

void free_item(void *item) { free(item); }

uint32_t hash1(void *item) { return (uint32_t)*(int *)item; }
uint32_t hash2(void *item) { return murmur_hash32_uint32(item, 42); }

void test_bloom_filter(int num_items) {
  double desired_fp_rate = 0.2;
  bloom_filter_t *filter =
      new_bloom_filter(num_items, desired_fp_rate, &hash1, &hash2);
  binary_tree_t *set = new_binary_tree(&compare);

  const uint32_t min = 0;
  const uint32_t max = num_items * 2;

  int num_fp = 0;

  for (int i = 0; i < num_items; i++) {
    uint32_t *item = malloc(sizeof(uint32_t));
    *item = rand_int(min, max);

    binary_tree_result_t result = binary_tree_insert(set, item);
    assert(result.status == BINARY_TREE_OK ||
           result.status == BINARY_TREE_DUPLICATE);

    bloom_filter_add(filter, item);

    if (result.status == BINARY_TREE_DUPLICATE) {
      free(item);
    }
  }

  for (int i = 0; i < num_items; i++) {
    uint32_t item = rand_int(min, max);

    binary_tree_result_t result = binary_tree_find(set, &item);
    assert(result.status != BINARY_TREE_ERROR);

    int tree_is_in = result.status == BINARY_TREE_OK;

    int is_in = bloom_filter_check(filter, &item);
    if (tree_is_in) {
      assert(is_in);
    } else if (!tree_is_in && is_in) {
      num_fp += 1;
    }
  }

  double fp_rate = (double)num_fp / num_items;
  if (fp_rate - desired_fp_rate > 0.0) {
    printf(
        "Warning: for %d number of tests the desired rate was %f and got %f\n",
        num_items, desired_fp_rate, fp_rate);
  }

  bloom_filter_destroy(filter);
  binary_tree_destroy(set, &free_item);
}

int main(int argc, char *argv[]) {
  for (int i = 1; i <= num_tests; i++) {
    test_bloom_filter(i * 10);
  }
  printf("✅ All tests passed!\n");
}
