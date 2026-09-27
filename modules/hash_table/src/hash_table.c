#include "../include/hash_table.h"
#include "avl_tree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Average number of pairs per bucket that triggers a resize.
#define MAX_LOAD_FACTOR 10

typedef struct hash_table {
  size_t capacity;
  size_t size;
  avl_tree_t **buckets;
  int (*compare_key)(const void *key1, const void *key2);
  unsigned long (*hash)(const void *key);
} hash_table_t;

/**
 * What each bucket (an AVL tree) stores. The key comparator is kept in the
 * item because the AVL tree's compare function only receives the two items.
 */
typedef struct bucket_item {
  void *key;
  void *value;
  int (*compare_key)(const void *key1, const void *key2);
} bucket_item_t;

// --- Private Functions ---
static int compare_bucket_items(const void *item1, const void *item2) {
  const bucket_item_t *a = (const bucket_item_t *)item1;
  const bucket_item_t *b = (const bucket_item_t *)item2;
  return a->compare_key(a->key, b->key);
}

static size_t bucket_index(unsigned long (*hash)(const void *key),
                           size_t capacity, const void *key) {
  return (size_t)(hash(key) % capacity);
}

static avl_tree_t *find_bucket(hash_table_t *table, const void *key) {
  return table->buckets[bucket_index(table->hash, table->capacity, key)];
}

/**
 * Calls visit on every item stored under node (post-order, so visit may free
 * the item). Stops and returns 0 as soon as visit returns 0.
 * Recursion depth is bounded by the AVL tree height.
 */
static int for_each_item(avl_tree_node_t *node,
                         int (*visit)(bucket_item_t *item, void *context),
                         void *context) {
  if (node == NULL) {
    return 1;
  }
  return for_each_item(node->left, visit, context) &&
         for_each_item(node->right, visit, context) &&
         visit((bucket_item_t *)node->value, context);
}

static avl_tree_t **new_buckets(size_t capacity) {
  avl_tree_t **buckets = malloc(sizeof(avl_tree_t *) * capacity);
  if (buckets == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < capacity; i++) {
    buckets[i] = new_avl_tree(compare_bucket_items);
    if (buckets[i] == NULL) {
      for (size_t j = 0; j < i; j++) {
        avl_tree_destroy(buckets[j], NULL);
      }
      free(buckets);
      return NULL;
    }
  }
  return buckets;
}

// Destroys the buckets and their nodes, but not the items stored in them.
static void destroy_buckets(avl_tree_t **buckets, size_t capacity) {
  for (size_t i = 0; i < capacity; i++) {
    avl_tree_destroy(buckets[i], NULL);
  }
  free(buckets);
}

typedef struct rehash_context {
  avl_tree_t **buckets;
  size_t capacity;
  unsigned long (*hash)(const void *key);
} rehash_context_t;

static int rehash_item(bucket_item_t *item, void *context) {
  rehash_context_t *ctx = (rehash_context_t *)context;
  avl_tree_t *bucket =
      ctx->buckets[bucket_index(ctx->hash, ctx->capacity, item->key)];
  return avl_tree_insert(bucket, item).status == AVL_TREE_OK;
}

/**
 * Doubles the number of buckets. On failure the table is left untouched.
 */
static HASH_TABLE_STATUS resize_hash_table(hash_table_t *table) {
  size_t new_capacity = table->capacity * 2;
  avl_tree_t **buckets = new_buckets(new_capacity);
  if (buckets == NULL) {
    return HASH_TABLE_ERROR;
  }

  rehash_context_t context = {buckets, new_capacity, table->hash};
  for (size_t i = 0; i < table->capacity; i++) {
    if (!for_each_item(table->buckets[i]->root, rehash_item, &context)) {
      // Items are still owned by the old buckets.
      destroy_buckets(buckets, new_capacity);
      return HASH_TABLE_ERROR;
    }
  }

  destroy_buckets(table->buckets, table->capacity);
  table->buckets = buckets;
  table->capacity = new_capacity;
  return HASH_TABLE_OK;
}

typedef struct destroy_context {
  void (*destroy_key)(void *);
  void (*destroy_value)(void *);
} destroy_context_t;

static int destroy_item(bucket_item_t *item, void *context) {
  destroy_context_t *ctx = (destroy_context_t *)context;
  if (ctx->destroy_key != NULL) {
    ctx->destroy_key(item->key);
  }
  if (ctx->destroy_value != NULL) {
    ctx->destroy_value(item->value);
  }
  free(item);
  return 1;
}

typedef struct print_context {
  void (*repr_key)(const void *key, char *buffer, size_t buffer_size);
  char *key_buffer;
  size_t key_buffer_size;
  void (*repr_value)(const void *value, char *buffer, size_t buffer_size);
  char *value_buffer;
  size_t value_buffer_size;
  int first;
} print_context_t;

static int print_item(bucket_item_t *item, void *context) {
  print_context_t *ctx = (print_context_t *)context;
  ctx->repr_key(item->key, ctx->key_buffer, ctx->key_buffer_size);
  ctx->repr_value(item->value, ctx->value_buffer, ctx->value_buffer_size);
  printf("%s(%s, %s)", ctx->first ? " [" : ", ", ctx->key_buffer,
         ctx->value_buffer);
  ctx->first = 0;
  return 1;
}

// --- Public Functions ---
hash_table_t *new_hash_table(size_t capacity,
                             int (*compare_key)(const void *key1,
                                                const void *key2),
                             unsigned long (*hash)(const void *key)) {
  if (capacity < 1 || compare_key == NULL || hash == NULL) {
    return NULL;
  }

  hash_table_t *table = malloc(sizeof(hash_table_t));
  if (table == NULL) {
    return NULL;
  }

  table->buckets = new_buckets(capacity);
  if (table->buckets == NULL) {
    free(table);
    return NULL;
  }

  table->capacity = capacity;
  table->size = 0;
  table->compare_key = compare_key;
  table->hash = hash;

  return table;
}

size_t hash_table_size(hash_table_t *table) {
  return table == NULL ? 0 : table->size;
}

hash_table_result_t hash_table_get(hash_table_t *table, const void *key) {
  hash_table_result_t result = {HASH_TABLE_ERROR, NULL};
  if (table == NULL) {
    return result;
  }

  bucket_item_t lookup = {(void *)key, NULL, table->compare_key};
  avl_tree_result_t found = avl_tree_find(find_bucket(table, key), &lookup);
  if (found.status != AVL_TREE_OK) {
    result.status = HASH_TABLE_NOT_FOUND;
    return result;
  }

  result.status = HASH_TABLE_OK;
  result.value = ((bucket_item_t *)found.node->value)->value;
  return result;
}

HASH_TABLE_STATUS hash_table_put(hash_table_t *table, void *key, void *value) {
  if (table == NULL) {
    return HASH_TABLE_ERROR;
  }

  // A failed resize is not fatal: the table keeps working, just more loaded.
  if (table->size >= table->capacity * MAX_LOAD_FACTOR) {
    resize_hash_table(table);
  }

  bucket_item_t *item = malloc(sizeof(bucket_item_t));
  if (item == NULL) {
    return HASH_TABLE_ERROR;
  }
  item->key = key;
  item->value = value;
  item->compare_key = table->compare_key;

  avl_tree_result_t inserted = avl_tree_insert(find_bucket(table, key), item);
  switch (inserted.status) {
  case AVL_TREE_OK:
    table->size++;
    return HASH_TABLE_OK;
  case AVL_TREE_DUPLICATE:
    free(item);
    return HASH_TABLE_DUPLICATE;
  default:
    free(item);
    return HASH_TABLE_ERROR;
  }
}

hash_table_remove_result_t hash_table_remove(hash_table_t *table,
                                             const void *key) {
  hash_table_remove_result_t result = {HASH_TABLE_ERROR, NULL, NULL};
  if (table == NULL) {
    return result;
  }

  bucket_item_t lookup = {(void *)key, NULL, table->compare_key};
  avl_tree_remove_result_t removed =
      avl_tree_remove(find_bucket(table, key), &lookup);
  switch (removed.status) {
  case AVL_TREE_OK: {
    bucket_item_t *item = (bucket_item_t *)removed.value;
    result.status = HASH_TABLE_OK;
    result.key = item->key;
    result.value = item->value;
    free(item);
    table->size--;
    return result;
  }
  case AVL_TREE_NOT_FOUND:
    result.status = HASH_TABLE_NOT_FOUND;
    return result;
  default:
    return result;
  }
}

void hash_table_destroy(hash_table_t *table, void (*destroy_key)(void *),
                        void (*destroy_value)(void *)) {
  if (table == NULL) {
    return;
  }

  destroy_context_t context = {destroy_key, destroy_value};
  for (size_t i = 0; i < table->capacity; i++) {
    for_each_item(table->buckets[i]->root, destroy_item, &context);
  }
  destroy_buckets(table->buckets, table->capacity);
  free(table);
}

void hash_table_print(hash_table_t *table,
                      void (*repr_key)(const void *key, char *buffer,
                                       size_t buffer_size),
                      size_t key_buffer_size,
                      void (*repr_value)(const void *value, char *buffer,
                                         size_t buffer_size),
                      size_t value_buffer_size) {
  if (table == NULL || repr_key == NULL || repr_value == NULL ||
      key_buffer_size == 0 || value_buffer_size == 0) {
    return;
  }
  if (table->size == 0) {
    printf("Empty hash table\n");
    return;
  }

  print_context_t context = {repr_key,
                             malloc(sizeof(char) * key_buffer_size),
                             key_buffer_size,
                             repr_value,
                             malloc(sizeof(char) * value_buffer_size),
                             value_buffer_size,
                             1};
  if (context.key_buffer == NULL || context.value_buffer == NULL) {
    free(context.key_buffer);
    free(context.value_buffer);
    return;
  }

  for (size_t i = 0; i < table->capacity; i++) {
    printf(i == table->capacity / 2 ? "Hash table:\t|" : "\t\t|");
    context.first = 1;
    for_each_item(table->buckets[i]->root, print_item, &context);
    printf(context.first ? "\n" : "]\n");
  }

  free(context.key_buffer);
  free(context.value_buffer);
}
