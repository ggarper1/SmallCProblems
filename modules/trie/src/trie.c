#include "../include/trie.h"
#include <stdbool.h>
#include <stdlib.h>

#define RESULT(status_) ((trie_result_t){.status = (status_)})

// ---------------------------------------------------------------------------
// Sorted child lists
// ---------------------------------------------------------------------------

// Returns the link (list head or some `next` field) where `c` is, or where it
// would have to be inserted to keep the list sorted.
static linked_list_node_t **find_link(linked_list_node_t **link, char c) {
  while (*link != NULL && (*link)->node->character < c) {
    link = &(*link)->next;
  }
  return link;
}

static bool link_holds(linked_list_node_t **link, char c) {
  return *link != NULL && (*link)->node->character == c;
}

static trie_node_t *find_child(linked_list_node_t *children, char c) {
  linked_list_node_t **link = find_link(&children, c);
  return link_holds(link, c) ? (*link)->node : NULL;
}

// Inserts a new trie node at `link`, returning it (NULL if out of memory)
static trie_node_t *insert_at(linked_list_node_t **link, char c, void *value) {
  linked_list_node_t *ll_node = malloc(sizeof(linked_list_node_t));
  trie_node_t *node = malloc(sizeof(trie_node_t));
  if (ll_node == NULL || node == NULL) {
    free(ll_node);
    free(node);
    return NULL;
  }

  node->character = c;
  node->value = value;
  node->children = NULL;

  ll_node->node = node;
  ll_node->next = *link;
  *link = ll_node;
  return node;
}

static trie_node_t *find_or_insert_child(linked_list_node_t **children,
                                         char c) {
  linked_list_node_t **link = find_link(children, c);
  return link_holds(link, c) ? (*link)->node : insert_at(link, c, NULL);
}

// Unlinks the list node at `link` and frees it along with its trie node
static void unlink_and_free(linked_list_node_t **link) {
  linked_list_node_t *ll_node = *link;
  *link = ll_node->next;
  free(ll_node->node);
  free(ll_node);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

trie_t *new_trie(void) {
  trie_t *trie = malloc(sizeof(trie_t));
  if (trie == NULL) {
    return NULL;
  }

  trie->size = 0;
  trie->roots = NULL;
  return trie;
}

size_t trie_size(const trie_t *trie) { return trie->size; }

trie_result_t trie_get(const trie_t *trie, const char *key) {
  if (trie == NULL) {
    return RESULT(TRIE_ERROR);
  }

  // Walk the key including its '\0', which is the node holding the value
  linked_list_node_t *children = trie->roots;
  for (const char *c = key;; c++) {
    trie_node_t *node = find_child(children, *c);
    if (node == NULL) {
      return RESULT(TRIE_NOT_FOUND);
    }
    if (*c == '\0') {
      return (trie_result_t){.status = TRIE_OK, .value.node = node};
    }
    children = node->children;
  }
}

trie_result_t trie_add(trie_t *trie, const char *key, void *value) {
  if (trie == NULL) {
    return RESULT(TRIE_ERROR);
  }

  linked_list_node_t **children = &trie->roots;
  for (const char *c = key; *c != '\0'; c++) {
    trie_node_t *node = find_or_insert_child(children, *c);
    if (node == NULL) {
      return RESULT(TRIE_ERROR);
    }
    children = &node->children;
  }

  linked_list_node_t **end = find_link(children, '\0');
  if (link_holds(end, '\0')) {
    return RESULT(TRIE_ALREADY_EXISTS);
  }

  trie_node_t *node = insert_at(end, '\0', value);
  if (node == NULL) {
    return RESULT(TRIE_ERROR);
  }

  trie->size++;
  return (trie_result_t){.status = TRIE_OK, .value.node = node};
}

// Removes `key` from the list `*children`, storing its value in `*value`.
// On the way back up, nodes left without children are freed too.
static bool remove_key(linked_list_node_t **children, const char *key,
                       void **value) {
  linked_list_node_t **link = find_link(children, *key);
  if (!link_holds(link, *key)) {
    return false;
  }

  trie_node_t *node = (*link)->node;
  if (*key == '\0') {
    *value = node->value;
  } else if (!remove_key(&node->children, key + 1, value)) {
    return false;
  } else if (node->children != NULL) {
    return true; // Still used by other keys
  }

  unlink_and_free(link);
  return true;
}

trie_result_t trie_remove(trie_t *trie, const char *key) {
  if (trie == NULL) {
    return RESULT(TRIE_ERROR);
  }

  void *value;
  if (!remove_key(&trie->roots, key, &value)) {
    return RESULT(TRIE_NOT_FOUND);
  }

  trie->size--;
  return (trie_result_t){.status = TRIE_OK, .value.value = value};
}

// Recursion depth is bounded by the longest key
static void destroy_list(linked_list_node_t *list,
                         void (*destroy_value)(void *)) {
  while (list != NULL) {
    linked_list_node_t *next = list->next;
    trie_node_t *node = list->node;

    destroy_list(node->children, destroy_value);
    if (destroy_value != NULL && node->value != NULL) {
      destroy_value(node->value);
    }
    free(node);
    free(list);

    list = next;
  }
}

void trie_destroy(trie_t *trie, void (*destroy_value)(void *)) {
  if (trie == NULL) {
    return;
  }
  destroy_list(trie->roots, destroy_value);
  free(trie);
}
