#include "../include/binaryTree.h"
#include <math.h>
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

BinaryTree_t *newBinaryTree(int (*compare_func)(const void *item1,
                                                const void *item2)) {
  BinaryTree_t *tree = malloc(sizeof(BinaryTree_t));
  if (tree == NULL) {
    return NULL;
  }

  tree->length = 0;
  tree->compare_func = compare_func;
  tree->root = NULL;

  return tree;
}

BTNode_t *btFind(BinaryTree_t *tree, void *item) {
  BTNode_t *current = tree->root;

  int comparison;
  while (current != NULL) {
    comparison = tree->compare_func(item, current->value);
    if (comparison < 0) {
      current = current->left;
    } else if (comparison > 0) {
      current = current->right;
    } else {
      return current;
    }
  }
  return NULL;
}

BT_STATUS btInsert(BinaryTree_t *tree, void *item, BTNode_t **node) {
  BTNode_t *newNode = malloc(sizeof(BTNode_t));
  if (newNode == NULL) {
    return BT_ERROR;
  }

  newNode->value = item;
  newNode->right = NULL;
  newNode->left = NULL;

  if (tree->root == NULL) {
    tree->length = 1;
    tree->root = newNode;
    *node = newNode;
    return BT_OK;
  }

  BTNode_t *current = tree->root;
  BTNode_t *prev = NULL;
  int comparison;
  while (current != NULL) {
    comparison = tree->compare_func(item, current->value);
    if (comparison < 0) {
      prev = current;
      current = current->left;
    } else if (comparison > 0) {
      prev = current;
      current = current->right;
    } else {
      *node = current;
      free(newNode);
      return BT_DUPLICATE;
    }
  }

  if (comparison < 0) {
    prev->left = newNode;
  } else {
    prev->right = newNode;
  }
  current = newNode;
  *node = newNode;

  tree->length++;
  return BT_OK;
}

int btRemove(BinaryTree_t *tree, void *item) {
  if (tree->root == NULL) {
    return BT_NOT_FOUND;
  }

  BTNode_t *current = tree->root;
  BTNode_t **prev = &tree->root;

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
      return BT_NOT_FOUND;
    }
    comparison = tree->compare_func(item, current->value);
  }

  tree->length--;

  if (current->left == NULL && current->right == NULL) {
    *prev = NULL;
    free(current);
  } else if (current->right == NULL) {
    BTNode_t *toDelete = current->left;
    current->value = toDelete->value;
    current->left = toDelete->left;
    current->right = toDelete->right;

    free(toDelete);
  } else if (current->left == NULL) {
    BTNode_t *toDelete = current->right;
    current->value = toDelete->value;
    current->left = toDelete->left;
    current->right = toDelete->right;

    free(toDelete);
  } else {
    BTNode_t *toDelete = current->right;
    prev = &(current->right);
    while (toDelete->left != NULL) {
      prev = &(toDelete->left);
      toDelete = toDelete->left;
    }
    *prev = toDelete->right;
    current->value = toDelete->value;

    free(toDelete);
  }
  return BT_OK;
}

void btDestroy(BinaryTree_t *tree) {
  if (tree->root == NULL) {
    free(tree);
    return;
  }

  BTNode_t **nodes = malloc(sizeof(BTNode_t *) * (tree->length));
  nodes[0] = tree->root;
  int i = 0;

  while (i > -1) {
    BTNode_t *l = nodes[i]->left;
    BTNode_t *r = nodes[i]->right;
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

void btDestroyAll(BinaryTree_t *tree) {
  if (tree->root == NULL) {
    free(tree);
    return;
  }

  BTNode_t **nodes = malloc(sizeof(BTNode_t *) * (tree->length));
  nodes[0] = tree->root;

  int i = 0;
  while (i > -1) {
    BTNode_t *l = nodes[i]->left;
    BTNode_t *r = nodes[i]->right;
    free(nodes[i]->value);
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

int getTreeHeight(BinaryTree_t *tree) {
  if (tree->root == NULL) {
    return -1;
  }

  BTNode_t **nodes = malloc(tree->length * sizeof(BTNode_t *));
  int *heights = malloc(tree->length * sizeof(int));

  int maxHeight = -1;

  int i = 0, j = 0;
  int ctr = 0;
  nodes[i] = tree->root;
  heights[i] = 0;
  int height;
  BTNode_t *node;
  while (i > -1) {
    node = nodes[i];
    height = heights[i];

    int lNull = node->left == NULL, rNull = node->right == NULL;
    if (lNull && rNull) {
      i--;
      j = i;
      maxHeight = max(height, maxHeight);
    } else {
      if (!rNull) {
        heights[j] = height + 1;
        nodes[j] = node->right;
        j++;
      }
      if (!lNull) {
        heights[j] = height + 1;
        nodes[j] = node->left;
      } else {
        j--;
      }
      i = j;
    }
  }

  free(nodes);
  free(heights);
  return maxHeight;
}

void printTree(BinaryTree_t *tree,
               void (*repr)(BTNode_t *node, char *buffer, int bufferSize),
               int bufferSize) {
  if (tree->root == NULL) {
    printf("Empty tree\n");
    return;
  }
  if (tree->length > 12) {
    printf("Tree too big to print\n");
    return;
  }

  int treeHeight = getTreeHeight(tree);

  BTNode_t **nodes = calloc(pow(2, treeHeight + 1) - 1, sizeof(BTNode_t *));
  nodes[0] = tree->root;

  int i = 0, j = 1, ctr = 1;
  while (ctr < tree->length) {
    if (nodes[i] != NULL) {
      if (nodes[i]->left != NULL) {
        nodes[j] = nodes[i]->left;
        ctr++;
      }
      j++;
      if (nodes[i]->right != NULL) {
        nodes[j] = nodes[i]->right;
        ctr++;
      }
    } else {
      j++;
    }
    j++;
    i++;
  }

  char *buffer = malloc(sizeof(char) * bufferSize);
  int maxSpace = pow(2, treeHeight) * bufferSize;
  int start = 0, end = 1;
  int depth = 1;
  int nSep = maxSpace / 2, cSep = maxSpace / 4;
  ctr = 0;
  while (ctr < tree->length) {
    // Print nodes:
    for (int i = start; i < end; i++) {
      if (nodes[i] != NULL) {
        ctr++;
        repr(nodes[i], buffer, bufferSize);
        printf("%*s", nSep, buffer);
        printf("%*s", nSep, "");
        if (ctr == tree->length) {
          printf("\n");
          free(buffer);
          free(nodes);
          return;
        }
      } else {
        printf("%*s", nSep * 2, "");
      }
    }
    printf("\n%*s", cSep, "");

    // Print node connections
    for (int i = start; i < end; i++) {
      if (nodes[i] != NULL) {
        if (nodes[i]->left != NULL) {
          for (int j = 0; j < cSep - 1; j++) {
            printf("_");
          }
          printf("/");
        } else {
          printf("%*s", cSep, "");
        }
        if (nodes[i]->right != NULL) {
          printf("\\");
          for (int j = 0; j < cSep - 1; j++) {
            printf("_");
          }
        } else {
          printf("%*s", cSep, "");
        }
        printf("%*s", cSep * 2, "");
      } else {
        printf("%*s", cSep * 4, "");
      }
    }
    printf("\n");

    start = end;
    end = end * 2 + 1;
    depth++;

    nSep = maxSpace / ((int)pow(2, depth));
    cSep = maxSpace / ((int)pow(2, depth + 1));
  }
}
