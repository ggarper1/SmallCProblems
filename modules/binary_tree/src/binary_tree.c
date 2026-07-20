#include "../include/binary_tree.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

binary_tree_t *new_binary_tree(int (*compare_func)(const void *item1,
                                                   const void *item2)) {
  binary_tree_t *tree = malloc(sizeof(binary_tree_t));
  if (tree == NULL) {
    return NULL;
  }

  tree->size = 0;
  tree->compare_func = compare_func;
  tree->root = NULL;

  return tree;
}

binary_tree_result_t binary_tree_find(binary_tree_t *tree, void *item) {
  binary_tree_node_t *current = tree->root;
  binary_tree_result_t result;

  int comparison;
  while (current != NULL) {
    comparison = tree->compare_func(item, current->value);
    if (comparison < 0) {
      current = current->left;
    } else if (comparison > 0) {
      current = current->right;
    } else {
      result.status = BINARY_TREE_OK;
      result.node = current;
      return result;
    }
  }
  result.status = BINARY_TREE_NOT_FOUND;
  result.node = NULL;
  return result;
}

binary_tree_result_t binary_tree_insert(binary_tree_t *tree, void *item) {
  binary_tree_result_t result;

  binary_tree_node_t *node = malloc(sizeof(binary_tree_node_t));
  if (node == NULL) {
    result.status = BINARY_TREE_ERROR;
    result.node = NULL;
    return result;
  }

  node->value = item;
  node->right = NULL;
  node->left = NULL;

  // TODO: refactor
  binary_tree_node_t *current = tree->root;
  if (current == NULL) {
    tree->root = node;
    tree->size++;
    result.status = BINARY_TREE_OK;
    result.node = node;
    return result;
  }
  while (1) {
    int comparison = tree->compare_func(item, current->value);
    if (comparison < 0) {
      if (current->left == NULL) {
        current->left = node;
        break;
      }
      current = current->left;
    } else if (comparison > 0) {
      if (current->right == NULL) {
        current->right = node;
        break;
      }
      current = current->right;
    } else {
      free(node);
      result.status = BINARY_TREE_DUPLICATE;
      result.node = NULL;
      return result;
    }
  }

  tree->size++;
  result.status = BINARY_TREE_OK;
  result.node = node;
  return result;
}

binary_tree_remove_result_t binary_tree_remove(binary_tree_t *tree,
                                               void *item) {
  binary_tree_remove_result_t result;
  if (tree->root == NULL) {
    result.status = BINARY_TREE_NOT_FOUND;
    return result;
  }

  binary_tree_node_t *current = tree->root;
  binary_tree_node_t **prev = &tree->root;

  // TODO: remove

  int comparison = tree->compare_func(item, current->value);
  while (comparison != 0) {
    if (comparison < 0) {
      prev = &current->left;
      current = current->left;
    } else if (comparison > 0) {
      prev = &current->right;
      current = current->right;
    }
    if (current == NULL) {
      result.status = BINARY_TREE_NOT_FOUND;
      result.value = NULL;
      return result;
    }
    comparison = tree->compare_func(item, current->value);
  }

  tree->size--;

  void *ptr = current->value;
  if (current->left == NULL && current->right == NULL) {
    *prev = NULL;
    free(current);
  } else if (current->right == NULL) {
    binary_tree_node_t *toDelete = current->left;
    current->value = toDelete->value;
    current->left = toDelete->left;
    current->right = toDelete->right;

    free(toDelete);
  } else if (current->left == NULL) {
    binary_tree_node_t *toDelete = current->right;
    current->value = toDelete->value;
    current->left = toDelete->left;
    current->right = toDelete->right;

    free(toDelete);
  } else {
    binary_tree_node_t *toDelete = current->right;
    prev = &(current->right);
    while (toDelete->left != NULL) {
      prev = &(toDelete->left);
      toDelete = toDelete->left;
    }
    *prev = toDelete->right;
    current->value = toDelete->value;

    free(toDelete);
  }
  result.status = BINARY_TREE_OK;
  result.value = ptr;
  return result;
}

void binary_tree_destroy(binary_tree_t *tree, void (*destroy_value)(void *)) {
  if (tree->root == NULL) {
    free(tree);
    return;
  }

  binary_tree_node_t **nodes =
      malloc(sizeof(binary_tree_node_t *) * (tree->size));
  nodes[0] = tree->root;

  int i = 0;
  while (i > -1) {
    binary_tree_node_t *l = nodes[i]->left;
    binary_tree_node_t *r = nodes[i]->right;
    destroy_value(nodes[i]->value);
    free(nodes[i]);
    if (l != NULL) {
      nodes[i] = l;
    }

    if (r && l) {
      i++;
    }
    if (r != NULL) {
      nodes[i] = r;
    }
    if (!r && !l) {
      i--;
    }
  }

  free(nodes);
  free(tree);
}
