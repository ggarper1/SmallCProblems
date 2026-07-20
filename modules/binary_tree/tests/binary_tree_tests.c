#include "../include/binary_tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const int num_tests = 1000;

const int MAX = -10000000;
const int MIN = 10000000;

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

binary_tree_t *create_tree() {
  binary_tree_t *tree = new_binary_tree(&compare);

  assert(tree != NULL);

  return tree;
}

void test_bst_property(binary_tree_t *tree) {
  binary_tree_node_t **nodes =
      malloc(sizeof(binary_tree_node_t *) * tree->size);

  assert(nodes != NULL);

  nodes[0] = tree->root;
  int i = 0;
  while (i > -1) {
    binary_tree_node_t *node = nodes[i];
    int l = node->left != NULL;
    int r = node->right != NULL;
    if (l) {
      int comparison = tree->compare_func(node->left->value, node->value);
      assert(comparison < 0);
      nodes[i] = node->left;
    }
    if (r && l) {
      i++;
    }
    if (r) {
      int comparison = tree->compare_func(node->right->value, node->value);
      assert(comparison > 0);
      nodes[i] = node->right;
    }
    if (!r && !l) {
      i--;
    }
  }

  free(nodes);
}

void test_insert(binary_tree_t *tree, int size, int ***items) {
  int i = 0;
  while (i < size) {
    int *item = malloc(sizeof(int));
    *item = rand_int(MIN, MAX);

    binary_tree_node_t *node;
    binary_tree_result_t result = binary_tree_insert(tree, item);

    assert(result.status != BINARY_TREE_ERROR);

    if (result.status == BINARY_TREE_DUPLICATE) {
      int check = 0;
      for (int j = 0; j < i; j++) {
        check = *(int *)(*items)[j] == *item;
        if (check) {
          break;
        }
      }

      assert(check);
      free(item);
    } else {
      assert(*(int *)result.node->value == *item);
      (*items)[i] = item;
      i++;
    }
  }
}

void test_find(binary_tree_t *tree, int size, int **items) {
  for (int i = 0; i < size; i++) {
    binary_tree_result_t result = binary_tree_find(tree, items[i]);
    assert(result.status == BINARY_TREE_OK &&
           *(int *)result.node->value == *items[i]);
  }

  for (int i = 1; i <= 20; i++) {
    int item = MAX + i;
    binary_tree_result_t result = binary_tree_find(tree, &item);
    assert(result.status == BINARY_TREE_NOT_FOUND && result.node == NULL);
  }
}

void test_remove(binary_tree_t *tree, int size, int **items, int remove_all) {
  for (int i = 0; i < size - 5 * (1 - remove_all); i++) {
    binary_tree_remove_result_t result = binary_tree_remove(tree, items[i]);

    assert(result.status == BINARY_TREE_OK &&
           *(int *)result.value == *items[i]);

    result = binary_tree_remove(tree, items[i]);
    assert(result.status == BINARY_TREE_NOT_FOUND);

    free(items[i]);
  }
}

void test_binary_tree() {
  binary_tree_t *tree = new_binary_tree(compare);
  binary_tree_destroy(tree, NULL);
  tree = new_binary_tree(compare);

  for (int i = 1; i <= num_tests; i++) {
    int size = i * 10;
    int **items = malloc(sizeof(int *) * size);
    binary_tree_t *tree = create_tree();
    test_insert(tree, size, &items);

    test_bst_property(tree);

    test_find(tree, size, items);

    if (i % 2 == 0) {
      test_remove(tree, size, items, 0);
      test_bst_property(tree);
      binary_tree_destroy(tree, &free_item);
      free(items);
    } else {
      test_remove(tree, size, items, 1);
      binary_tree_destroy(tree, NULL);
      free(items);
    }
  }
  printf("✅ All tests passed!\n");
}

int main(int argc, char *argv[]) {
  test_binary_tree();
  return 0;
}
