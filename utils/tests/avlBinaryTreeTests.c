#include "../include/avlBinaryTree.h"
#include "../include/queue.h"
#include "../include/random.h"
#include "../include/stack.h"
#include <ranlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define abs(a)                                                                 \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    _a > 0 ? _a : -_a;                                                         \
  })

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

const int numTests = 200;
// TODO:
const int max = 1000; // 100000000;
const int min = -1000;

void debugTree1(AVLNode_t *node) {
  Queue_t *q = newQueue(10);
  qAdd(q, node);
  int ctr = 0;
  while (qLength(q) > 0) {
    AVLNode_t *node = qRemove(q);
    if (node == NULL) {
      printf("%d: Node NULL.\n", ctr);

    } else {
      printf("%d: Node %d. rHeight: %d, lHeight: %d\n", ctr,
             *(int *)node->value, node->rHeight, node->lHeight);
      if (node->left != NULL) {
        qAdd(q, node->left);
      } else {
        qAdd(q, NULL);
      }
      if (node->right != NULL) {
        qAdd(q, node->right);
      } else {
        qAdd(q, NULL);
      }
    }
    ctr++;
  }
  qDestroy(q);
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

void repr(AVLNode_t *node, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *((int *)node->value));
}

void printTreeBalances(AVLBinaryTree_t *tree) {
  Stack_t *stack = newStack(tree->height + 1);
  sPush(stack, tree->root);
  while (sLength(stack) > 0) {
    AVLNode_t *node = sPop(stack);
    printf("Node %d: rHeight: %d, lHeight: %d\n", *(int *)node->value,
           node->rHeight, node->lHeight);
    if (node->right != NULL) {
      sPush(stack, node->right);
    }
    if (node->left != NULL) {
      sPush(stack, node->left);
    }
  }
  sDestroy(stack);
}

void printFromNode(AVLNode_t *node) {
  Stack_t *stack = newStack(10);
  sPush(stack, node);
  while (sLength(stack) > 0) {
    AVLNode_t *n = sPop(stack);
    printf("%d, ", *(int *)n->value);
    if (n->right != NULL) {
      sPush(stack, n->right);
    }
    if (n->left != NULL) {
      sPush(stack, n->left);
    }
  }
  sDestroy(stack);
  printf("\n");
}

AVLBinaryTree_t *createTree(int size, void ***items) {
  AVLBinaryTree_t *tree = newAVLBinaryTree(&compare);
  if (tree == NULL) {
    return NULL;
  }

  *items = malloc(sizeof(void *) * size);
  for (int i = 0; i < size; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(min, max);
    (*items)[i] = item;
    AVLNode_t *node = avlInsert(tree, (void *)item);
    while (node == NULL) {
      free(item);
      item = malloc(sizeof(int));
      *item = randInt(min, max);
      (*items)[i] = item;
      node = avlInsert(tree, (void *)item);
    }
  }

  return tree;
}

bool testFind(AVLBinaryTree_t *tree, void **items, int size) {
  for (int i = 0; i < size; i++) {
    AVLNode_t *node = avlFind(tree, items[i]);
    if (node == NULL || *(int *)(node->value) != *(int *)(items[i])) {
      printf("Items: %d", *(int *)items[0]);
      for (int j = 1; j < size; j++) {
        printf(", %d", *(int *)items[j]);
      }
      printf("\n");

      if (node == NULL) {
        printf("Element %d was not found when it should have.\n",
               *(int *)items[i]);
      } else {

        printf("Node value returned (%d) does not match value being searched "
               "for (%d).\n",
               *(int *)node->value, *(int *)items[i]);
      }
      return false;
    }
  }
  return true;
}

bool testBSTProperty(AVLBinaryTree_t *tree) {
  AVLNode_t **nodes = malloc(sizeof(AVLNode_t *) * (tree->height + 1));
  if (nodes == NULL) {
    printf("Could not allocate memory for tree with length %d", tree->height);
    return false;
  }

  nodes[0] = tree->root;
  int i = 0;
  while (i > -1) {
    AVLNode_t *node = nodes[i];
    int l = node->left != NULL;
    int r = node->right != NULL;
    if (l) {
      int comparison = tree->compare_func(node->left->value, node->value);
      if (!(comparison < 0)) {
        printf("Node %d is smaller than it's left child %d\n",
               *(int *)node->value, *(int *)node->left->value);
        return false;
      }
      nodes[i] = node->left;
    }
    if (r && l) {
      i++;
    }
    if (r) {
      int comparison = tree->compare_func(node->right->value, node->value);
      if (comparison < 0) {
        printf("Node %d is bigger than it's right child %d\n",
               *(int *)node->value, *(int *)node->right->value);

        return false;
      }
      nodes[i] = node->right;
    }
    if (!r && !l) {
      i--;
    }
  }

  free(nodes);

  return true;
}

bool testBalance(AVLBinaryTree_t *tree) {
  Stack_t *stack = newStack(tree->height * 2 + 1);
  if (stack == NULL) {
    printf("Could not allocate memory for stack");
    return false;
  }

  if (tree->root == NULL) {
    return true;
  }

  sPush(stack, tree->root);
  while (sLength(stack) > 0) {
    AVLNode_t *node = sPop(stack);
    if (abs(node->lHeight - node->rHeight) > 1) {
      printf("🚨 Node %d, with rHeight %d and lHeight %d is unbalanced\n",
             *(int *)node->value, node->rHeight, node->lHeight);
      sDestroy(stack);
      return false;
    }

    if (node->right != NULL) {
      sPush(stack, node->right);
    }
    if (node->left != NULL) {
      sPush(stack, node->left);
    }
  }

  sDestroy(stack);

  return true;
}

bool testInsert(AVLBinaryTree_t *tree, int size) {
  for (int i = 0; i < 50; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(min, max);
    AVLNode_t *node = avlInsert(tree, (void *)item);
    if (node == NULL) {
      free(item);
    }
  }
  return true;
}

int findMaxDepth(AVLNode_t *node, int depth) {
  int lHeight =
      node->left != NULL ? findMaxDepth(node->left, depth + 1) : depth;
  int rHeight =
      node->right != NULL ? findMaxDepth(node->right, depth + 1) : depth;

  return max(lHeight, rHeight);
}

bool testBalanceValues(AVLBinaryTree_t *tree) {
  if (tree->root == NULL) {
    return true;
  }
  Stack_t *stack = newStack(20);
  sPush(stack, tree->root);
  while (sLength(stack) > 0) {
    AVLNode_t *node = sPop(stack);
    if (node->left != NULL && node->lHeight != findMaxDepth(node->left, 1)) {
      printf("🚨 Incorrect balance: node %d should have lHeight %d, instead "
             "has lHeight %d\n",
             *(int *)node->value, findMaxDepth(node->left, 1), node->lHeight);
      return false;
    }
    if (node->left == NULL && node->lHeight != 0) {
      printf("🚨 Incorrect balance: node %d should have lHeight %d, instead "
             "has lHeight %d\n",
             *(int *)node->value, 0, node->lHeight);
      return false;
    }

    if (node->right != NULL && node->rHeight != findMaxDepth(node->right, 1)) {
      printf("🚨 Incorrect balance: node %d should have rHeight %d, instead "
             "has rHeight %d\n",
             *(int *)node->value, findMaxDepth(node->right, 1), node->rHeight);
      return false;
    }
    if (node->right == NULL && node->rHeight != 0) {
      printf("🚨 Incorrect balance: node %d should have rHeight %d, instead "
             "has rHeight %d\n",
             *(int *)node->value, 0, node->rHeight);
      return false;
    }
  }
  sDestroy(stack);
  return true;
}

bool testRemove(AVLBinaryTree_t *tree, void **items, int size) {
  for (int i = 0; i < size * 99 / 100; i++) {
    void *value = avlRemove(tree, items[i]);
    // testBalanceValues(tree);
    // testBalance(tree);
    // debugTree1(tree->root);
    // avlPrintTree(tree, repr, 5);
    if (value == NULL || *((int *)value) != *((int *)items[i])) {
      if (value == NULL) {
        printf("Tried to remove value %d (which should be in the tree) yet "
               "NULL was returned.\n",
               *((int *)items[i]));
      } else {
        printf("Value removed (%d) is not the one expected (%d).\n",
               *((int *)value), *((int *)items[i]));
      }
      return false;
    }
    free(value);
  }

  return true;
}

bool testDestroy(AVLBinaryTree_t *tree) {
  avlDestroy(tree);
  return true;
}

bool testDestroyAll(AVLBinaryTree_t *tree) {
  avlDestroyAll(tree);
  return true;
}

void testBinaryTree() {
  AVLBinaryTree_t *tree = newAVLBinaryTree(compare);
  avlDestroyAll(tree);
  tree = newAVLBinaryTree(compare);
  avlDestroy(tree);

  for (int i = 1; i <= numTests; i++) {
    void **items;
    int size = i * 10;
    AVLBinaryTree_t *tree = createTree(size, &items);

    if (tree == NULL) {
      printf("🚨 Error creating tree\n");
      return;
    }

    if (0) {
      if (!testBalance(tree)) {
        printf("🚨 Unbalanced tree!\n");
        return;
      }
      if (!testBSTProperty(tree)) {
        printf("🚨 Failed BST Property!\n");
        return;
      }
      if (!testFind(tree, items, size)) {
        printf("🚨 Failed Find Test!\n");
        return;
      }
      testDestroy(tree);
      for (int j = 0; j < size; j++) {
        free(items[j]);
      }
    } else {
      // if (!testBalanceValues(tree)) {
      //   printf("🚨 Failed Balance values after insert!\n");
      //   return;
      // }
      // if (!testBalance(tree)) {
      //   printf("🚨 Unbalanced tree!\n");
      //   return;
      // }
      // if (!testBSTProperty(tree)) {
      //   printf("🚨 Failed BST Property!\n");
      //   return;
      // }
      // if (!testFind(tree, items, size)) {
      //   printf("🚨 Failed Find Test!\n");
      //   return;
      // }
      // if (!testInsert(tree, size)) {
      //   printf("🚨 Failed Insert Test!\n");
      //   return;
      // }
      // if (!testBalanceValues(tree)) {
      //   printf("🚨 Failed Balance values after insert!\n");
      //   return;
      // }
      // if (!testBalance(tree)) {
      //   printf("🚨 Unbalanced tree (after insert)!\n");
      //   return;
      // }
      // if (!testBSTProperty(tree)) {
      //   printf("🚨 Failed BST Property after insert!\n");
      //   return;
      // }
      avlPrintTree(tree, repr, 5);
      if (!testRemove(tree, items, size)) {
        printf("🚨 Failed Remove Test!\n");
        return;
      }
      if (!testBalanceValues(tree)) {
        avlPrintTree(tree, repr, 5);
        printf("🚨 Failed Balance values after insert!\n");
        return;
      }
      if (!testBalance(tree)) {
        avlPrintTree(tree, repr, 5);
        printf("🚨 Unbalanced tree (after removal)!\n");
        return;
      }
      if (!testBSTProperty(tree)) {
        printf("🚨 Failed BST Property after removal!\n");
        return;
      }
      testDestroyAll(tree);
    }
    free(items);
  }
  printf("✅ All tests passed!\n");
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  testBinaryTree();
  return 0;
}
