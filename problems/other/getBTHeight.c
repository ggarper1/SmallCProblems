#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/include/binaryTree.h"
#include "../../utils/include/random.h"
#include "../../utils/include/stack.h"

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

// Recursive solution
int getHeightRec(BTNode_t *root) {
  if (root == NULL) {
    return -1;
  }
  return 1 + max(getHeightRec(root->left), getHeightRec(root->right));
}

// Iterative solution
int getHeightIt(BTNode_t *root) {
  if (root == NULL) {
    return -1;
  }

  Stack_t *stack = newStack(15);
  Stack_t *depthStack = newStack(15);
  sPush(stack, root);
  int *height = malloc(sizeof(int));
  *height = 0;
  sPush(depthStack, height);
  int maxHeight = -1;
  while (sLength(stack) > 0) {
    BTNode_t *node = sPop(stack);
    int *height = sPop(depthStack);

    int l = node->left != NULL;
    int r = node->right != NULL;
    if (r) {
      sPush(stack, node->right);
      int *h = malloc(sizeof(int));
      *h = *height + 1;
      sPush(depthStack, h);
    }
    if (l) {
      int *h = malloc(sizeof(int));
      *h = *height + 1;
      sPush(depthStack, h);
      sPush(stack, node->left);
    }
    if (!l & !r) {
      maxHeight = max(*height, maxHeight);
    }
    free(height);
  }

  sDestroy(stack);
  sDestroyAll(depthStack);

  return maxHeight;
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

void repr(BTNode_t *node, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *((int *)node->value));
}

int main(void) {
  for (int i = 0; i < 100; i++) {
    BinaryTree_t *tree = newBinaryTree(compare);
    for (int j = 0; j < i * 10; j++) {
      int *item = malloc(sizeof(int));
      *item = randInt(0, 99);
      btInsert(tree, item);
    }

    printTree(tree, repr, 3);

    int recFail = getHeightRec(tree->root) != tree->height;
    if (recFail) {
      printf("🚨 Recursive height id Failing\n");
      return 0;
    }

    int itFail = getHeightIt(tree->root) != tree->height;
    if (itFail) {
      printf("🚨 Iterative height id Failing\n");
      return 0;
    }
    btDestroyAll(tree);
  }
  printf("✅ Tests passed!\n");
  return 0;
}
