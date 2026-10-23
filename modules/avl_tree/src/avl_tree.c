#include "../include/avl_tree.h"
#include "../../bool_stack/include/bool_stack.h"
#include "../../stack/include/stack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define LEFT 0
#define RIGHT 1

// --- Private Functions ---
static inline int max_int(int a, int b) { return a > b ? a : b; }

static inline int abs_int(int a) { return a > 0 ? a : -a; }

static inline int node_height(avl_tree_node_t *node) {
  return max_int(node->l_height, node->r_height);
}

static inline int subtree_height(avl_tree_node_t *node) {
  return node == NULL ? 0 : node_height(node) + 1;
}

static avl_tree_node_t *new_node(void *value) {
  avl_tree_node_t *node = malloc(sizeof(avl_tree_node_t));
  if (node == NULL) {
    return NULL;
  }

  node->value = value;
  node->right = NULL;
  node->left = NULL;
  node->l_height = 0;
  node->r_height = 0;
  return node;
}

static avl_tree_node_t *pop_node(my_stack_t *path) {
  return (avl_tree_node_t *)stack_pop(path).value;
}

static avl_tree_node_t *peek_node(my_stack_t *path) {
  return (avl_tree_node_t *)stack_peek(path).value;
}

static int pop_dir(bool_stack_t *dirs) { return bool_stack_pop(dirs).value; }

static int peek_dir(bool_stack_t *dirs) { return bool_stack_peek(dirs).value; }

/**
 * Makes the parent on top of the path (or the root if the path is empty)
 * point to child, following the direction on top of dirs.
 */
static void replace_child(avl_tree_t *tree, my_stack_t *path,
                          bool_stack_t *dirs, avl_tree_node_t *child) {
  if (stack_size(path) == 0) {
    tree->root = child;
  } else if (peek_dir(dirs) == RIGHT) {
    peek_node(path)->right = child;
  } else {
    peek_node(path)->left = child;
  }
}

// Left-left case: single right rotation.
static avl_tree_node_t *rotate_ll(avl_tree_node_t *node) {
  avl_tree_node_t *x = node;
  avl_tree_node_t *y = node->left;
  avl_tree_node_t *z = y->left;

  avl_tree_node_t *c = y->right;

  x->left = c;
  x->l_height = subtree_height(c);

  y->left = z;
  y->l_height = subtree_height(z);
  y->right = x;
  y->r_height = subtree_height(x);

  return y;
}

// Right-right case: single left rotation.
static avl_tree_node_t *rotate_rr(avl_tree_node_t *node) {
  avl_tree_node_t *x = node;
  avl_tree_node_t *y = x->right;
  avl_tree_node_t *z = y->right;

  avl_tree_node_t *b = y->left;

  x->right = b;
  x->r_height = subtree_height(b);

  y->left = x;
  y->l_height = subtree_height(x);
  y->right = z;
  y->r_height = subtree_height(z);

  return y;
}

// Left-right case: double rotation.
static avl_tree_node_t *rotate_lr(avl_tree_node_t *node) {
  avl_tree_node_t *x = node;
  avl_tree_node_t *y = x->left;
  avl_tree_node_t *z = y->right;

  avl_tree_node_t *b = z->left;
  avl_tree_node_t *c = z->right;

  y->right = b;
  y->r_height = subtree_height(b);

  x->left = c;
  x->l_height = subtree_height(c);

  z->left = y;
  z->l_height = subtree_height(y);
  z->right = x;
  z->r_height = subtree_height(x);

  return z;
}

// Right-left case: double rotation.
static avl_tree_node_t *rotate_rl(avl_tree_node_t *node) {
  avl_tree_node_t *x = node;
  avl_tree_node_t *y = x->right;
  avl_tree_node_t *z = y->left;

  avl_tree_node_t *b = z->left;
  avl_tree_node_t *c = z->right;

  y->left = c;
  y->l_height = subtree_height(c);

  x->right = b;
  x->r_height = subtree_height(b);

  z->right = y;
  z->r_height = subtree_height(y);
  z->left = x;
  z->l_height = subtree_height(x);

  return z;
}

/**
 * Rebalances node.
 * @param dir Direction of the heavy child (LEFT or RIGHT).
 * @param child_dir Direction of the heavy grandchild inside that child.
 * @return The new root of the subtree.
 */
static avl_tree_node_t *rotate(avl_tree_node_t *node, int dir, int child_dir) {
  if (dir == RIGHT && child_dir == RIGHT) {
    return rotate_rr(node);
  } else if (dir == RIGHT) {
    return rotate_rl(node);
  } else if (child_dir == RIGHT) {
    return rotate_lr(node);
  } else {
    return rotate_ll(node);
  }
}

static void destroy_nodes(avl_tree_node_t *node,
                          void (*destroy_value)(void *)) {
  // Recursion depth is bounded by the tree height (~1.44 * log2(n)).
  if (node == NULL) {
    return;
  }

  destroy_nodes(node->left, destroy_value);
  destroy_nodes(node->right, destroy_value);
  if (destroy_value != NULL) {
    destroy_value(node->value);
  }
  free(node);
}

// --- Public Functions ---
avl_tree_t *new_avl_tree(int (*compare_func)(const void *item1,
                                             const void *item2)) {
  if (compare_func == NULL) {
    return NULL;
  }

  avl_tree_t *tree = malloc(sizeof(avl_tree_t));
  if (tree == NULL) {
    return NULL;
  }

  tree->size = 0;
  tree->root = NULL;
  tree->compare_func = compare_func;

  return tree;
}

size_t avl_tree_size(avl_tree_t *tree) { return tree == NULL ? 0 : tree->size; }

avl_tree_result_t avl_tree_find(avl_tree_t *tree, const void *value) {
  avl_tree_result_t result = {AVL_TREE_ERROR, NULL};
  if (tree == NULL) {
    return result;
  }

  avl_tree_node_t *current = tree->root;
  while (current != NULL) {
    int comparison = tree->compare_func(value, current->value);
    if (comparison < 0) {
      current = current->left;
    } else if (comparison > 0) {
      current = current->right;
    } else {
      result.status = AVL_TREE_OK;
      result.node = current;
      return result;
    }
  }

  result.status = AVL_TREE_NOT_FOUND;
  return result;
}

avl_tree_result_t avl_tree_insert(avl_tree_t *tree, void *value) {
  avl_tree_result_t result = {AVL_TREE_ERROR, NULL};
  if (tree == NULL) {
    return result;
  }

  if (tree->root == NULL) {
    avl_tree_node_t *node = new_node(value);
    if (node == NULL) {
      return result;
    }
    tree->root = node;
    tree->size++;
    result.status = AVL_TREE_OK;
    result.node = node;
    return result;
  }

  int height = node_height(tree->root) + 1;
  my_stack_t *path = new_stack(height);
  bool_stack_t *dirs = new_bool_stack(height);
  if (path == NULL || dirs == NULL) {
    goto cleanup;
  }

  // Forward pass: find the parent of the new node, remembering the path.
  avl_tree_node_t *current = tree->root;
  int depth = 0;
  int dir;
  while (1) {
    int comparison = tree->compare_func(value, current->value);
    if (comparison == 0) {
      result.status = AVL_TREE_DUPLICATE;
      result.node = current;
      goto cleanup;
    }

    dir = comparison > 0 ? RIGHT : LEFT;
    if (stack_push(path, current) != STACK_OK ||
        bool_stack_push(dirs, dir) != BOOL_STACK_OK) {
      goto cleanup;
    }
    depth++;

    avl_tree_node_t *next = dir == RIGHT ? current->right : current->left;
    if (next == NULL) {
      break;
    }
    current = next;
  }

  avl_tree_node_t *node = new_node(value);
  if (node == NULL) {
    goto cleanup;
  }
  if (dir == RIGHT) {
    current->right = node;
  } else {
    current->left = node;
  }
  tree->size++;
  result.status = AVL_TREE_OK;
  result.node = node;

  // Backward pass: update heights and rebalance (at most one rotation).
  int prev_dir = LEFT;
  int remaining = depth;
  while (remaining > 0) {
    avl_tree_node_t *n = pop_node(path);
    int d = pop_dir(dirs);
    int child_height = depth - remaining + 1;
    if (d == RIGHT && n->r_height < child_height) {
      n->r_height++;
    } else if (d == LEFT && n->l_height < child_height) {
      n->l_height++;
    }

    if (abs_int(n->r_height - n->l_height) > 1) {
      replace_child(tree, path, dirs, rotate(n, d, prev_dir));
      break;
    }
    prev_dir = d;
    remaining--;
  }

cleanup:
  stack_destroy(path, NULL);
  if (dirs != NULL) {
    bool_stack_destroy(dirs);
  }
  return result;
}

avl_tree_remove_result_t avl_tree_remove(avl_tree_t *tree, const void *value) {
  avl_tree_remove_result_t result = {AVL_TREE_ERROR, NULL};
  if (tree == NULL) {
    return result;
  }
  if (tree->root == NULL) {
    result.status = AVL_TREE_NOT_FOUND;
    return result;
  }

  int height = node_height(tree->root) + 1;
  my_stack_t *path = new_stack(height);
  bool_stack_t *dirs = new_bool_stack(height);
  if (path == NULL || dirs == NULL) {
    goto cleanup;
  }

  // Find the node, remembering the path to it.
  avl_tree_node_t *node = tree->root;
  int comparison = tree->compare_func(value, node->value);
  while (comparison != 0) {
    int dir = comparison > 0 ? RIGHT : LEFT;
    if (stack_push(path, node) != STACK_OK ||
        bool_stack_push(dirs, dir) != BOOL_STACK_OK) {
      goto cleanup;
    }
    node = dir == RIGHT ? node->right : node->left;
    if (node == NULL) {
      result.status = AVL_TREE_NOT_FOUND;
      goto cleanup;
    }
    comparison = tree->compare_func(value, node->value);
  }

  result.value = node->value;

  if (node->left == NULL || node->right == NULL) {
    // Zero or one child: link the parent directly to the child.
    avl_tree_node_t *child = node->left != NULL ? node->left : node->right;
    replace_child(tree, path, dirs, child);
    free(node);
  } else {
    // Two children: move the in-order successor's value into node.
    if (stack_push(path, node) != STACK_OK ||
        bool_stack_push(dirs, RIGHT) != BOOL_STACK_OK) {
      result.value = NULL;
      goto cleanup;
    }
    avl_tree_node_t *successor = node->right;
    while (successor->left != NULL) {
      if (stack_push(path, successor) != STACK_OK ||
          bool_stack_push(dirs, LEFT) != BOOL_STACK_OK) {
        result.value = NULL;
        goto cleanup;
      }
      successor = successor->left;
    }

    node->value = successor->value;
    replace_child(tree, path, dirs, successor->right);
    free(successor);
  }
  tree->size--;
  result.status = AVL_TREE_OK;

  // Walk back up updating heights and rebalancing where needed.
  while (stack_size(path) > 0) {
    avl_tree_node_t *n = pop_node(path);
    if (pop_dir(dirs) == RIGHT) {
      n->r_height = subtree_height(n->right);
    } else {
      n->l_height = subtree_height(n->left);
    }

    if (abs_int(n->r_height - n->l_height) > 1) {
      int dir = n->r_height > n->l_height ? RIGHT : LEFT;
      avl_tree_node_t *child = dir == RIGHT ? n->right : n->left;
      int unbalance = child->r_height - child->l_height;
      int child_dir = unbalance == 0 ? dir : (unbalance > 0 ? RIGHT : LEFT);

      replace_child(tree, path, dirs, rotate(n, dir, child_dir));
    }
  }

cleanup:
  stack_destroy(path, NULL);
  if (dirs != NULL) {
    bool_stack_destroy(dirs);
  }
  return result;
}

void avl_tree_destroy(avl_tree_t *tree, void (*destroy_value)(void *)) {
  if (tree == NULL) {
    return;
  }

  destroy_nodes(tree->root, destroy_value);
  free(tree);
}

void avl_tree_print(avl_tree_t *tree,
                    void (*repr)(avl_tree_node_t *node, char *buffer,
                                 size_t buffer_size),
                    size_t buffer_size) {
  if (tree == NULL || repr == NULL || buffer_size == 0) {
    return;
  }
  if (tree->root == NULL) {
    printf("Empty tree\n");
    return;
  }
  int height = node_height(tree->root);
  if (height > 6) {
    printf("Tree too big to print\n");
    return;
  }

  // Lay the tree out as a heap-ordered array (children of i at 2i+1, 2i+2).
  avl_tree_node_t **nodes =
      calloc((1 << (height + 1)) - 1, sizeof(avl_tree_node_t *));
  char *buffer = malloc(sizeof(char) * buffer_size);
  if (nodes == NULL || buffer == NULL) {
    free(nodes);
    free(buffer);
    return;
  }
  nodes[0] = tree->root;

  size_t count = 1;
  int i = 0, j = 1;
  while (count < tree->size) {
    if (nodes[i] != NULL) {
      if (nodes[i]->left != NULL) {
        nodes[j] = nodes[i]->left;
        count++;
      }
      j++;
      if (nodes[i]->right != NULL) {
        nodes[j] = nodes[i]->right;
        count++;
      }
    } else {
      j++;
    }
    j++;
    i++;
  }

  int max_space = (1 << height) * (int)buffer_size;
  int start = 0, end = 1;
  int depth = 1;
  int node_sep = max_space / 2, conn_sep = max_space / 4;
  count = 0;
  while (count < tree->size) {
    // Print nodes
    for (int k = start; k < end; k++) {
      if (nodes[k] != NULL) {
        count++;
        repr(nodes[k], buffer, buffer_size);
        printf("%*s", node_sep, buffer);
        printf("%*s", node_sep, "");
        if (count == tree->size) {
          printf("\n");
          free(buffer);
          free(nodes);
          return;
        }
      } else {
        printf("%*s", node_sep * 2, "");
      }
    }
    printf("\n%*s", conn_sep, "");

    // Print node connections
    for (int k = start; k < end; k++) {
      if (nodes[k] != NULL) {
        if (nodes[k]->left != NULL) {
          for (int s = 0; s < conn_sep - 1; s++) {
            printf("_");
          }
          printf("/");
        } else {
          printf("%*s", conn_sep, "");
        }
        if (nodes[k]->right != NULL) {
          printf("\\");
          for (int s = 0; s < conn_sep - 1; s++) {
            printf("_");
          }
        } else {
          printf("%*s", conn_sep, "");
        }
        printf("%*s", conn_sep * 2, "");
      } else {
        printf("%*s", conn_sep * 4, "");
      }
    }
    printf("\n");

    start = end;
    end = end * 2 + 1;
    depth++;

    node_sep = max_space / (1 << depth);
    conn_sep = max_space / (1 << (depth + 1));
  }

  free(buffer);
  free(nodes);
}
