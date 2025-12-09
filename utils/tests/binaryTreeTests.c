#include "../include/binaryTree.h"
#include "../include/random.h"
#include <stdio.h>
#include <stdlib.h>

const int numTests = 1000;
const int max = 100000;
const int min = 0;

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

void repr(BTNode_t *node, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *((int *)node->value));
}

BinaryTree_t *createTree() {
  BinaryTree_t *tree = newBinaryTree(&compare);

  if (tree == NULL) {
    return NULL;
  }

  return tree;
}

int testBSTProperty(BinaryTree_t *tree) {
  BTNode_t **nodes = malloc(sizeof(BTNode_t *) * tree->length);
  if (nodes == NULL) {
    printf("Could not allocate memory for tree with length %lu", tree->length);
    return 0;
  }

  nodes[0] = tree->root;
  int i = 0;
  while (i > -1) {
    BTNode_t *node = nodes[i];
    int l = node->left != NULL;
    int r = node->right != NULL;
    if (l) {
      int comparison = tree->compare_func(node->left->value, node->value);
      if (!(comparison < 0)) {
        return 0;
      }
      nodes[i] = node->left;
    }
    if (r && l) {
      i++;
    }
    if (r) {
      int comparison = tree->compare_func(node->right->value, node->value);
      if (comparison <= 0) {
        return 0;
      }
      nodes[i] = node->right;
    }
    if (!r && !l) {
      i--;
    }
  }

  free(nodes);

  return 1;
}

int testInsert(BinaryTree_t *tree, int size, int ***items) {
  int i = 0;
  while (i < size) {
    int *item = malloc(sizeof(int));
    *item = randInt(min, max);

    BTNode_t *node;
    int status = btInsert(tree, item, &node);
    if (status == BT_ERROR) {
      printf("🚨 Error during insertion\n");
      return 0;

    } else if (status == BT_DUPLICATE) {
      int check = 0;
      for (int j = 0; j < i; j++) {
        check |= *(int *)(*items)[j] == *item;
        if (check) {
          break;
        }
      }

      if (!check) {
        printf("🚨 Error during insertion: duplicate status was returned "
               "when it shouldn't have. Item that tried to be inserted: %d\n",
               *item);
        return 0;
      }
      free(item);
    } else {
      if (*(int *)node->value != *item) {
        printf(
            "🚨 Error during insertion: inserted incorrect value in node\n.");
        return 0;
      }
      (*items)[i] = item;
      i++;
    }
  }
  return 1;
}

int testFind(BinaryTree_t *tree, int size, int **items) {
  for (int i = 0; i < size; i++) {
    BTNode_t *node = btFind(tree, items[i]);
    if (node == NULL || *(int *)node->value != *items[i]) {
      printf("🚨 Error in find: node has incorrect value.\n");
      return 0;
    }
  }

  for (int i = 1; i <= 20; i++) {
    int *item = malloc(sizeof(int));
    *item = max + i;
    BTNode_t *node = btFind(tree, item);
    if (node != NULL) {
      printf("🚨 Error in find: node has incorrect value.\n");
      return 0;
    }
    free(item);
  }
  return 1;
}

int testRemove(BinaryTree_t *tree, int size, int **items, int removeALL) {
  for (int i = 0; i < size - 5 * (1 - removeALL); i++) {
    int status = btRemove(tree, items[i]);

    if (status == BT_NOT_FOUND) {
      printf("🚨 Error in remove: item should have been found but wasn't.\n");
      return 0;
    }

    status = btRemove(tree, items[i]);
    if (status == BT_OK) {
      printf("🚨 Error in remove: item should have not been found after "
             "removal.\n");
      return 0;
    }

    free(items[i]);
  }
  return 1;
}

void testBinaryTree() {
  BinaryTree_t *tree = newBinaryTree(compare);
  btDestroyAll(tree);
  tree = newBinaryTree(compare);
  btDestroy(tree);

  for (int i = 1; i <= numTests; i++) {
    int size = i * 10;
    int **items = malloc(sizeof(int *) * size);
    BinaryTree_t *tree = createTree();
    if (tree == NULL) {
      printf("🚨 Error creating tree\n");
      return;
    }
    if (!testInsert(tree, size, &items)) {
      printf("🚨 Error during insert\n");
      return;
    }

    if (i < 2) {
      printTree(tree, repr, 7);
    }

    if (!testBSTProperty(tree)) {
      printf("🚨 Error, BST property not satisfied\n");
      return;
    }
    if (!testFind(tree, size, items)) {
      printf("🚨 Error, BST property not satisfied\n");
      return;
    }

    if (i % 2 == 0) {
      if (!testRemove(tree, size, items, 0)) {
        printf("🚨 Error in remove\n");
        return;
      }
      if (!testBSTProperty(tree)) {
        printf("🚨 Error, BST property not satisfied\n");
        return;
      }
      btDestroyAll(tree);
      free(items);
    } else {
      if (!testRemove(tree, size, items, 1)) {
        printf("🚨 Error in remove\n");
        return;
      }
      btDestroy(tree);
      free(items);
    }
  }
  printf("✅ All tests passed!\n");
}

int main(int argc, char *argv[]) {
  testBinaryTree();
  return 0;
}
