#include "../include/hash_table.h"
#include "murmur_hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int NUM_TESTS = 100;
#define KEY_SIZE 16
const int MIN_VALUE = 0;
const int MAX_VALUE = 10000;

int rand_int(int min, int max) {
  int range = max - min + 1;
  return (rand() % range) + min;
}

// --- Callbacks ---
unsigned long hash(const void *key) {
  return (unsigned long)murmur_hash64_str((const char *)key, 0);
}

// Always the same bucket: forces every pair into one AVL tree.
unsigned long constant_hash(const void *key) {
  (void)key;
  return 42;
}

int compare_key(const void *key1, const void *key2) {
  return strcmp((const char *)key1, (const char *)key2);
}

void repr_key(const void *key, char *buffer, size_t buffer_size) {
  snprintf(buffer, buffer_size, "%s", (const char *)key);
}

void repr_value(const void *value, char *buffer, size_t buffer_size) {
  snprintf(buffer, buffer_size, "%d", *(const int *)value);
}

char *random_key() {
  char *key = malloc(sizeof(char) * KEY_SIZE);
  if (key == NULL) {
    return NULL;
  }
  for (int i = 0; i < KEY_SIZE - 1; i++) {
    key[i] = (char)rand_int('a', 'z');
  }
  key[KEY_SIZE - 1] = '\0';
  return key;
}

int *random_value() {
  int *value = malloc(sizeof(int));
  if (value != NULL) {
    *value = rand_int(MIN_VALUE, MAX_VALUE);
  }
  return value;
}

// --- Tests ---
/**
 * Fills a table with count random pairs. Returns 0 on failure.
 * keys and values receive the inserted pointers.
 */
int fill_table(hash_table_t *table, int count, char **keys, int **values) {
  int i = 0;
  while (i < count) {
    char *key = random_key();
    int *value = random_value();
    HASH_TABLE_STATUS status = hash_table_put(table, key, value);
    if (status == HASH_TABLE_DUPLICATE) {
      free(key);
      free(value);
      continue;
    }
    if (status != HASH_TABLE_OK) {
      printf("🚨 Error during put: status is %d\n", status);
      return 0;
    }
    keys[i] = key;
    values[i] = value;
    i++;
  }

  if (hash_table_size(table) != (size_t)count) {
    printf("🚨 Size is %zu, expected %d\n", hash_table_size(table), count);
    return 0;
  }
  return 1;
}

int test_duplicates(hash_table_t *table, char **keys, int **values,
                    int count) {
  for (int i = 0; i < count; i++) {
    int other = 0;
    HASH_TABLE_STATUS status = hash_table_put(table, keys[i], &other);
    if (status != HASH_TABLE_DUPLICATE) {
      printf("🚨 Duplicate put returned status %d\n", status);
      return 0;
    }
    // The stored value must not change.
    if (hash_table_get(table, keys[i]).value != values[i]) {
      printf("🚨 Duplicate put modified the stored value\n");
      return 0;
    }
  }
  return hash_table_size(table) == (size_t)count;
}

int test_get(hash_table_t *table, char **keys, int **values, int count) {
  for (int i = 0; i < count; i++) {
    // Use a copy of the key so lookups go through compare_key, not pointers.
    char copy[KEY_SIZE];
    strcpy(copy, keys[i]);
    hash_table_result_t result = hash_table_get(table, copy);
    if (result.status != HASH_TABLE_OK) {
      printf("🚨 Get returned status %d\n", result.status);
      return 0;
    }
    if (result.value != values[i]) {
      printf("🚨 Get returned the wrong value\n");
      return 0;
    }
  }

  hash_table_result_t result = hash_table_get(table, "not a key");
  if (result.status != HASH_TABLE_NOT_FOUND || result.value != NULL) {
    printf("🚨 Get found a key that isn't in the table\n");
    return 0;
  }
  return 1;
}

int test_remove(hash_table_t *table, char **keys, int **values, int count) {
  size_t initial_size = hash_table_size(table);
  for (int i = 0; i < count; i++) {
    hash_table_remove_result_t result = hash_table_remove(table, keys[i]);
    if (result.status != HASH_TABLE_OK) {
      printf("🚨 Remove returned status %d\n", result.status);
      return 0;
    }
    if (result.key != keys[i] || result.value != values[i]) {
      printf("🚨 Remove returned the wrong key or value\n");
      return 0;
    }
  }

  if (hash_table_size(table) != initial_size - count) {
    printf("🚨 Size is wrong after removal\n");
    return 0;
  }

  for (int i = 0; i < count; i++) {
    hash_table_result_t get = hash_table_get(table, keys[i]);
    hash_table_remove_result_t removed = hash_table_remove(table, keys[i]);
    if (get.status != HASH_TABLE_NOT_FOUND || get.value != NULL ||
        removed.status != HASH_TABLE_NOT_FOUND || removed.key != NULL ||
        removed.value != NULL) {
      printf("🚨 Removed key is still in the table\n");
      return 0;
    }
    free(keys[i]);
    free(values[i]);
  }
  return 1;
}

int test_edge_cases() {
  int value = 0;
  if (new_hash_table(0, compare_key, hash) != NULL ||
      new_hash_table(5, NULL, hash) != NULL ||
      new_hash_table(5, compare_key, NULL) != NULL) {
    printf("🚨 new_hash_table accepted invalid arguments\n");
    return 0;
  }

  if (hash_table_size(NULL) != 0 ||
      hash_table_put(NULL, "key", &value) != HASH_TABLE_ERROR ||
      hash_table_get(NULL, "key").status != HASH_TABLE_ERROR ||
      hash_table_remove(NULL, "key").status != HASH_TABLE_ERROR) {
    printf("🚨 NULL table handling misbehaves\n");
    return 0;
  }
  hash_table_destroy(NULL, NULL, NULL);

  hash_table_t *table = new_hash_table(5, compare_key, hash);
  if (hash_table_size(table) != 0 ||
      hash_table_get(table, "key").status != HASH_TABLE_NOT_FOUND ||
      hash_table_remove(table, "key").status != HASH_TABLE_NOT_FOUND) {
    printf("🚨 Empty table misbehaves\n");
    return 0;
  }
  hash_table_print(table, repr_key, KEY_SIZE, repr_value, 6);
  hash_table_destroy(table, free, free);
  return 1;
}

int test_destroy_without_freeing(int count) {
  hash_table_t *table = new_hash_table(3, compare_key, hash);
  char **keys = malloc(sizeof(char *) * count);
  int **values = malloc(sizeof(int *) * count);
  if (!fill_table(table, count, keys, values)) {
    return 0;
  }

  // The table doesn't own the pairs, so the caller frees them.
  hash_table_destroy(table, NULL, NULL);
  for (int i = 0; i < count; i++) {
    free(keys[i]);
    free(values[i]);
  }
  free(keys);
  free(values);
  return 1;
}

int run_table_tests(hash_table_t *table, int insertions) {
  char **keys = malloc(sizeof(char *) * (insertions + 1));
  int **values = malloc(sizeof(int *) * (insertions + 1));

  if (!fill_table(table, insertions, keys, values)) {
    return 0;
  }
  if (insertions < 40) {
    hash_table_print(table, repr_key, KEY_SIZE, repr_value, 6);
  }
  if (!test_duplicates(table, keys, values, insertions)) {
    printf("🚨 Failed duplicates test!\n");
    return 0;
  }
  if (!test_get(table, keys, values, insertions)) {
    printf("🚨 Failed get test!\n");
    return 0;
  }

  // Remove all but the last 5 pairs; the table frees those on destroy.
  int removals = insertions > 5 ? insertions - 5 : 0;
  if (!test_remove(table, keys, values, removals)) {
    printf("🚨 Failed remove test!\n");
    return 0;
  }
  if (!test_get(table, keys + removals, values + removals,
                insertions - removals)) {
    printf("🚨 Failed get test after removal!\n");
    return 0;
  }

  hash_table_destroy(table, free, free);
  free(keys);
  free(values);
  return 1;
}

void test_hash_table() {
  if (!test_edge_cases()) {
    return;
  }
  if (!test_destroy_without_freeing(200)) {
    printf("🚨 Failed destroy without freeing!\n");
    return;
  }

  // All pairs collide in one bucket.
  if (!run_table_tests(new_hash_table(4, compare_key, constant_hash), 500)) {
    printf("🚨 Failed single-bucket test!\n");
    return;
  }

  // Starting with 1 bucket forces several resizes.
  for (int i = 0; i < NUM_TESTS; i++) {
    hash_table_t *table = new_hash_table(i % 2 == 0 ? 1 : 5, compare_key, hash);
    if (table == NULL) {
      printf("🚨 Error creating hash table\n");
      return;
    }
    if (!run_table_tests(table, i * 10)) {
      return;
    }
  }
  printf("✅ All tests passed!\n");
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  test_hash_table();
  return 0;
}
