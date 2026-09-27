#include "../../queue/include/queue.h"
#include "../../stack/include/stack.h"
#include "../include/avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int NUM_TESTS = 600;
const int MAX = 10000000;
const int MIN = 0;

static inline int max_int(int a, int b) { return a > b ? a : b; }

static inline int abs_int(int a) { return a > 0 ? a : -a; }

int rand_int(int min, int max) {
  int range = max - min + 1;
  return (rand() % range) + min;
}

// --- Debug helpers ---
void debug_tree(avl_tree_node_t *root) {
  queue_t *queue = new_queue(10);
  queue_add(queue, root);
  int ctr = 0;
  while (queue_size(queue) > 0) {
    avl_tree_node_t *node = queue_remove(queue).value;
    if (node == NULL) {
      printf("%d: Node NULL.\n", ctr);
    } else {
      printf("%d: Node %d. r_height: %d, l_height: %d\n", ctr,
             *(int *)node->value, node->r_height, node->l_height);
      queue_add(queue, node->left);
      queue_add(queue, node->right);
    }
    ctr++;
  }
  queue_destroy(queue, NULL);
}

void print_from_node(avl_tree_node_t *root) {
  my_stack_t *stack = new_stack(10);
  stack_push(stack, root);
  while (stack_size(stack) > 0) {
    avl_tree_node_t *node = stack_pop(stack).value;
    printf("%d, ", *(int *)node->value);
    if (node->right != NULL) {
      stack_push(stack, node->right);
    }
    if (node->left != NULL) {
      stack_push(stack, node->left);
    }
  }
  stack_destroy(stack, NULL);
  printf("\n");
}

// --- Callbacks ---
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

void repr(avl_tree_node_t *node, char *buffer, size_t buffer_size) {
  snprintf(buffer, buffer_size, "%d", *((int *)node->value));
}

// --- Tests ---
avl_tree_t *create_tree(int size, void ***items) {
  avl_tree_t *tree = new_avl_tree(compare);
  if (tree == NULL) {
    return NULL;
  }

  *items = malloc(sizeof(void *) * size);
  int i = 0;
  while (i < size) {
    int *item = malloc(sizeof(int));
    *item = rand_int(MIN, MAX);
    avl_tree_result_t result = avl_tree_insert(tree, item);
    if (result.status == AVL_TREE_DUPLICATE) {
      if (result.node == NULL || *(int *)result.node->value != *item) {
        printf("🚨 Duplicate insertion did not return the existing node\n");
        return NULL;
      }
      free(item);
    } else if (result.status == AVL_TREE_ERROR) {
      printf("🚨 Error during insertion\n");
      return NULL;
    } else {
      if (result.node == NULL || result.node->value != item) {
        printf("🚨 Node returned in insertion is incorrect\n");
        return NULL;
      }
      (*items)[i] = item;
      i++;
    }
  }

  if (avl_tree_size(tree) != (size_t)size) {
    printf("🚨 Tree size %zu does not match %d\n", avl_tree_size(tree), size);
    return NULL;
  }

  return tree;
}

int test_find(avl_tree_t *tree, void **items, int size) {
  for (int i = 0; i < size; i++) {
    avl_tree_result_t result = avl_tree_find(tree, items[i]);
    if (result.status != AVL_TREE_OK || result.node == NULL ||
        *(int *)(result.node->value) != *(int *)(items[i])) {
      printf("Element %d was not found when it should have.\n",
             *(int *)items[i]);
      return 0;
    }
  }

  int missing = MAX + 1;
  avl_tree_result_t result = avl_tree_find(tree, &missing);
  if (result.status != AVL_TREE_NOT_FOUND || result.node != NULL) {
    printf("Element %d was found when it shouldn't have.\n", missing);
    return 0;
  }
  return 1;
}

int test_bst_property(avl_tree_t *tree) {
  if (tree->root == NULL) {
    return 1;
  }

  my_stack_t *stack = new_stack(10);
  stack_push(stack, tree->root);
  while (stack_size(stack) > 0) {
    avl_tree_node_t *node = stack_pop(stack).value;
    if (node->left != NULL) {
      if (!(tree->compare_func(node->left->value, node->value) < 0)) {
        printf("Node %d is smaller than its left child %d\n",
               *(int *)node->value, *(int *)node->left->value);
        stack_destroy(stack, NULL);
        return 0;
      }
      stack_push(stack, node->left);
    }
    if (node->right != NULL) {
      if (!(tree->compare_func(node->right->value, node->value) > 0)) {
        printf("Node %d is bigger than its right child %d\n",
               *(int *)node->value, *(int *)node->right->value);
        stack_destroy(stack, NULL);
        return 0;
      }
      stack_push(stack, node->right);
    }
  }

  stack_destroy(stack, NULL);
  return 1;
}

int test_balance(avl_tree_t *tree) {
  if (tree->root == NULL) {
    return 1;
  }

  my_stack_t *stack = new_stack(10);
  stack_push(stack, tree->root);
  while (stack_size(stack) > 0) {
    avl_tree_node_t *node = stack_pop(stack).value;
    if (abs_int(node->l_height - node->r_height) > 1) {
      printf("🚨 Node %d, with r_height %d and l_height %d is unbalanced\n",
             *(int *)node->value, node->r_height, node->l_height);
      stack_destroy(stack, NULL);
      return 0;
    }

    if (node->right != NULL) {
      stack_push(stack, node->right);
    }
    if (node->left != NULL) {
      stack_push(stack, node->left);
    }
  }

  stack_destroy(stack, NULL);
  return 1;
}

int find_max_depth(avl_tree_node_t *node, int depth) {
  int l_height =
      node->left != NULL ? find_max_depth(node->left, depth + 1) : depth;
  int r_height =
      node->right != NULL ? find_max_depth(node->right, depth + 1) : depth;

  return max_int(l_height, r_height);
}

int check_height(avl_tree_node_t *node, avl_tree_node_t *child, int height,
                 char side) {
  int expected = child != NULL ? find_max_depth(child, 1) : 0;
  if (height != expected) {
    printf("🚨 Incorrect balance: node %d should have %c_height %d, instead "
           "has %c_height %d\n",
           *(int *)node->value, side, expected, side, height);
    return 0;
  }
  return 1;
}

int test_balance_values(avl_tree_t *tree) {
  if (tree->root == NULL) {
    return 1;
  }

  my_stack_t *stack = new_stack(10);
  stack_push(stack, tree->root);
  while (stack_size(stack) > 0) {
    avl_tree_node_t *node = stack_pop(stack).value;
    if (!check_height(node, node->left, node->l_height, 'l') ||
        !check_height(node, node->right, node->r_height, 'r')) {
      stack_destroy(stack, NULL);
      return 0;
    }

    if (node->right != NULL) {
      stack_push(stack, node->right);
    }
    if (node->left != NULL) {
      stack_push(stack, node->left);
    }
  }

  stack_destroy(stack, NULL);
  return 1;
}

int test_insert(avl_tree_t *tree, int count) {
  int i = 0;
  while (i < count) {
    int *item = malloc(sizeof(int));
    *item = rand_int(MIN, MAX);
    avl_tree_result_t result = avl_tree_insert(tree, item);
    if (result.status == AVL_TREE_ERROR) {
      printf("🚨 Error during insertion\n");
      free(item);
      return 0;
    } else if (result.status == AVL_TREE_DUPLICATE) {
      free(item);
    } else {
      if (result.node->value != item) {
        printf("🚨 Error in insertion\n");
        return 0;
      }
      i++;
    }
  }
  return 1;
}

int test_remove(avl_tree_t *tree, void **items, int size) {
  size_t initial_size = avl_tree_size(tree);
  int removed = size * 99 / 100;
  for (int i = 0; i < removed; i++) {
    avl_tree_remove_result_t result = avl_tree_remove(tree, items[i]);
    if (result.status == AVL_TREE_ERROR) {
      printf("🚨 Error during removal\n");
      return 0;
    } else if (result.status == AVL_TREE_NOT_FOUND) {
      printf("🚨 Item should have been found during removal\n");
      return 0;
    } else if (result.value != items[i]) {
      printf("🚨 Removed value is not the one expected\n");
      return 0;
    }
    free(items[i]);
    items[i] = NULL;
  }

  if (avl_tree_size(tree) != initial_size - removed) {
    printf("🚨 Tree size is wrong after removal\n");
    return 0;
  }

  for (int i = 1; i <= 10; i++) {
    int item = MAX + i;
    avl_tree_remove_result_t result = avl_tree_remove(tree, &item);
    if (result.status == AVL_TREE_ERROR) {
      printf("🚨 Error during removal\n");
      return 0;
    } else if (result.status != AVL_TREE_NOT_FOUND || result.value != NULL) {
      printf("🚨 Removed item that shouldn't be in tree\n");
      return 0;
    }
  }

  return 1;
}

int test_empty_tree() {
  avl_tree_t *tree = new_avl_tree(compare);
  int value = 1;
  if (avl_tree_size(tree) != 0 ||
      avl_tree_find(tree, &value).status != AVL_TREE_NOT_FOUND ||
      avl_tree_remove(tree, &value).status != AVL_TREE_NOT_FOUND) {
    printf("🚨 Empty tree misbehaves\n");
    return 0;
  }
  avl_tree_destroy(tree, NULL);

  tree = new_avl_tree(compare);
  avl_tree_destroy(tree, free);

  if (new_avl_tree(NULL) != NULL ||
      avl_tree_insert(NULL, &value).status != AVL_TREE_ERROR ||
      avl_tree_remove(NULL, &value).status != AVL_TREE_ERROR ||
      avl_tree_find(NULL, &value).status != AVL_TREE_ERROR) {
    printf("🚨 NULL handling misbehaves\n");
    return 0;
  }
  avl_tree_destroy(NULL, NULL);
  return 1;
}

int test_destroy_without_values(int size) {
  void **items;
  avl_tree_t *tree = create_tree(size, &items);
  if (tree == NULL) {
    return 0;
  }

  // Values are not freed by the tree, so the caller frees them.
  avl_tree_destroy(tree, NULL);
  for (int i = 0; i < size; i++) {
    free(items[i]);
  }
  free(items);
  return 1;
}

int run_tree_checks(avl_tree_t *tree, const char *stage) {
  if (!test_balance_values(tree)) {
    avl_tree_print(tree, repr, 9);
    printf("🚨 Failed balance values (%s)!\n", stage);
    return 0;
  }
  if (!test_balance(tree)) {
    avl_tree_print(tree, repr, 9);
    printf("🚨 Unbalanced tree (%s)!\n", stage);
    return 0;
  }
  if (!test_bst_property(tree)) {
    printf("🚨 Failed BST property (%s)!\n", stage);
    return 0;
  }
  return 1;
}

void test_avl_tree() {
  if (!test_empty_tree()) {
    return;
  }
  if (!test_destroy_without_values(100)) {
    printf("🚨 Failed destroy without values!\n");
    return;
  }

  for (int i = 1; i <= NUM_TESTS; i++) {
    void **items;
    int size = i * 10;
    avl_tree_t *tree = create_tree(size, &items);
    if (tree == NULL) {
      printf("🚨 Error creating tree\n");
      return;
    }

    if (i < 8) {
      avl_tree_print(tree, repr, 9);
    }

    if (!run_tree_checks(tree, "after creation")) {
      return;
    }
    if (!test_find(tree, items, size)) {
      printf("🚨 Failed find test!\n");
      return;
    }
    if (!test_insert(tree, 50)) {
      printf("🚨 Failed insert test!\n");
      return;
    }
    if (!run_tree_checks(tree, "after insert")) {
      return;
    }
    if (!test_remove(tree, items, size)) {
      printf("🚨 Failed remove test!\n");
      return;
    }
    if (!run_tree_checks(tree, "after removal")) {
      return;
    }

    // Remaining values (including the ones from test_insert) are freed by
    // the tree.
    avl_tree_destroy(tree, free);
    free(items);
  }
  printf("✅ All tests passed!\n");
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  test_avl_tree();
  return 0;
}
