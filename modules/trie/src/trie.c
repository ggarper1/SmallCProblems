#include "../include/trie.h"
#include "../../linked_list/include/linked_list.h"
#include <stdio.h>
#include <stdlib.h>

#define RETURN_TRIE_NOT_FOUND(res)                                             \
  do {                                                                         \
    (res).status = TRIE_NOT_FOUND;                                             \
    (res).value.node = NULL;                                                   \
    return (res);                                                              \
  } while (0)

#define RETURN_TRIE_ERROR(res)                                                 \
  do {                                                                         \
    (res).status = TRIE_ERROR;                                                 \
    (res).value.node = NULL;                                                   \
    return (res);                                                              \
  } while (0)

trie_t *new_trie() {
  trie_t *trie = malloc(sizeof(trie_t));
  if (trie == NULL) {
    return NULL;
  }

  trie->size = 0;
  trie->roots = NULL;
  return trie;
}

size_t trie_size(trie_t *trie) { return trie->size; }

trie_node_t *find_trie_node(linked_list_node_t *node, char character) {
  if (node == NULL) {
    return NULL;
  }

  while (node->node->character < character) {
    if (node->next == NULL) {
      return NULL;
    }

    node = node->next;
  }
  return node->node->character != character ? NULL : node->node;
  return node->node;
}

trie_result_t trie_get(trie_t *trie, char *key) {
  trie_result_t result;
  if (trie->roots == NULL) {
    RETURN_TRIE_NOT_FOUND(result);
  }

  trie_node_t *trie_node;
  int idx = 0;
  linked_list_node_t *nodes = trie->roots;
  while (key[idx] != '\0') {
    trie_node = find_trie_node(nodes, key[idx]);

    if (trie_node == NULL) {
      RETURN_TRIE_NOT_FOUND(result);
    }

    nodes = trie_node->children;
    idx++;
  }
  result.status = TRIE_OK;
  result.value.node = find_trie_node(nodes, '\0');

  return result;
}

linked_list_node_t *create_ll_node(char character, void *value) {
  linked_list_node_t *ll_node = malloc(sizeof(linked_list_node_t));
  if (ll_node == NULL) {
    return NULL;
  }

  trie_node_t *trie_node = malloc(sizeof(trie_node_t));
  if (trie_node == NULL) {
    return NULL;
  }

  ll_node->prev = NULL;
  ll_node->next = NULL;
  ll_node->node = trie_node;

  trie_node->character = character;
  trie_node->value = value;
  trie_node->children = NULL;

  return ll_node;
}

linked_list_node_t *add_new_nodes(char *key, void *value, int idx) {
  int is_done = key[idx] == '\0';

  linked_list_node_t *first_ll_node =
      create_ll_node(key[idx], is_done ? NULL : value);
  if (is_done) {
    return first_ll_node;
  }

  idx++;
  linked_list_node_t *ll_node = first_ll_node;
  linked_list_node_t *new_node;
  while (key[idx] != '\0') {
    new_node = create_ll_node(key[idx], NULL);
    ll_node->next = new_node;
    new_node->prev = ll_node;

    ll_node = new_node;
    idx++;
  }

  new_node = create_ll_node(key[idx], value);
  ll_node->next = new_node;
  new_node->prev = ll_node;

  return first_ll_node;
}

// TODO: return indx too
linked_list_node_t *traverse_trie(trie_t *trie, char *key) {
  linked_list_node_t *node = trie->roots;

  int idx = 0;
  while (key[idx] != '\0') {
    while (node->next != NULL && node->node->character < key[idx]) {
      node = node->next;
    }

    if (node)
      idx++;
  }
}

trie_node_t *find_or_add_trie_node(linked_list_node_t **node, char character,
                                   void *value) {
  if (*node == NULL) {
    *node = create_ll_node(character, value);
    return (*node)->node;
  }
  // TODO: Fix this. You're using *node to iterate. THis changes the trie->roots
  // and makes it that the root id dimplye the node with character '/0''
  linked_list_node_t *n = *node;
  while (n->node->character < character) {
    if (n->next == NULL) {
      linked_list_node_t *ll_node = create_ll_node(character, value);
      n->next = ll_node;
      ll_node->prev = n;
      return ll_node->node;
    }

    n = n->next;
  }
  if (n->node->character != character) {
    linked_list_node_t *ll_node = create_ll_node(character, value);

    ll_node->prev = n->prev;
    ll_node->next = n;

    n->prev->next = ll_node;
    n->prev = ll_node;

    return ll_node->node;
  }
  return n->node;
}

trie_result_t trie_add(trie_t *trie, char *key, void *value) {
  trie_result_t result;
  if (trie == NULL) {
    RETURN_TRIE_ERROR(result);
  }

  trie_node_t *node;
  linked_list_node_t **nodes = &(trie->roots);
  int idx = 0;
  while (key[idx] != '\0') {
    node = find_or_add_trie_node(nodes, key[idx], NULL);
    if (nodes == NULL) {
      RETURN_TRIE_ERROR(result);
    }
    *nodes = node->children;
    idx++;
  }

  node = find_or_add_trie_node(nodes, key[idx], value);
  if (node == NULL) {
    RETURN_TRIE_ERROR(result);
  }
  if (node->value == value) {
    result.status = TRIE_OK;
    result.value.node = node;
  } else {
    result.status = TRIE_ALREADY_EXISTS;
    result.value.node = NULL;
  }

  printf("What was added: \n");
  linked_list_node_t *nn = trie->roots;
  while (nn != NULL) {
    if (nn->node->character == '\0') {

      printf("\tChar: \\0\n");
    } else {
      printf("\tChar: %c\n", nn->node->character);
    }
    nn = nn->node->children;
  }

  return result;
}

// trie_result_t trie_remove(trie_t *trie, char *key) {}

void trie_destroy(trie_t *trie, void (*destroy_value)(void *)) {}
