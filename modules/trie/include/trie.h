#ifndef TRIE_H
#define TRIE_H

#include <stddef.h>

typedef enum {
  TRIE_ERROR,
  TRIE_OK,
  TRIE_NOT_FOUND,
  TRIE_ALREADY_EXISTS
} TRIE_STATUS;

// Children of a trie node, kept as a singly linked list sorted by character.
// A key ends at a child whose character is '\0'; that child holds the value
// and, since '\0' sorts first, is always the head of its list.
typedef struct linked_list_node {
  struct linked_list_node *next;
  struct trie_node *node;
} linked_list_node_t;

typedef struct trie_node {
  linked_list_node_t *children;
  char character;
  void *value;
} trie_node_t;

typedef struct trie {
  linked_list_node_t *roots;
  size_t size;
} trie_t;

union result_value {
  void *value;       // trie_remove: the removed value, for the caller to free
  trie_node_t *node; // trie_add / trie_get: the '\0' node holding the value
};

typedef struct trie_result {
  TRIE_STATUS status;
  union result_value value;
} trie_result_t;

trie_t *new_trie(void);

size_t trie_size(const trie_t *trie);

trie_result_t trie_add(trie_t *trie, const char *key, void *value);

trie_result_t trie_get(const trie_t *trie, const char *key);

trie_result_t trie_remove(trie_t *trie, const char *key);

void trie_destroy(trie_t *trie, void (*destroy_value)(void *));

#endif
