#include <stddef.h>
#include <stdint.h>

#ifndef TRIE_H
#define TRIE_H

typedef enum {
  TRIE_ERROR,
  TRIE_OK,
  TRIE_NOT_FOUND,
  TRIE_ALREADY_EXISTS
} TRIE_STATUS;

typedef struct linked_list_node {
  struct linked_list_node *next;
  struct linked_list_node *prev;
  struct trie_node *node;
} linked_list_node_t;

typedef struct trie_node {
  struct linked_list_node *children;
  char character;
  void *value;
} trie_node_t;

typedef struct trie {
  linked_list_node_t *roots;
  size_t size;
} trie_t;

union result_value {
  void *value;
  trie_node_t *node;
};

typedef struct trie_result {
  TRIE_STATUS status;
  union result_value value;
} trie_result_t;

typedef struct trie trie_t;

trie_t *new_trie();

size_t trie_size(trie_t *trie);

trie_result_t trie_add(trie_t *trie, char *key, void *value);

trie_result_t trie_get(trie_t *trie, char *key);

trie_result_t trie_remove(trie_t *trie, char *key);

void trie_destroy(trie_t *trie, void (*destroy_value)(void *));

#endif
