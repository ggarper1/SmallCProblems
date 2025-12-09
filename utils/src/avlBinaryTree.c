#include "../include/avlBinaryTree.h"
#include "../include/boolStack.h"
#include "../include/queue.h"
#include "../include/stack.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

#define abs(a)                                                                 \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    _a > 0 ? _a : -_a;                                                         \
  })
// TODO:remove
void repr2(AVLNode_t *node, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *((int *)node->value));
}
void repr3(void *value, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *(int *)((AVLNode_t *)value)->value);
}
// --- Private Function Prototypes ---
AVLNode_t *leftRotate(AVLNode_t *node) {
  AVLNode_t *x = node;
  AVLNode_t *y = node->left;
  AVLNode_t *z = y->left;

  AVLNode_t *c = y->right;

  x->left = c;
  x->lHeight = c == NULL ? 0 : max(c->lHeight, c->rHeight) + 1;

  y->left = z;
  y->lHeight = max(z->rHeight, z->lHeight) + 1;
  y->right = x;
  y->rHeight = max(x->rHeight, x->lHeight) + 1;

  return y;
}

AVLNode_t *rightRotate(AVLNode_t *node) {
  AVLNode_t *x = node;
  AVLNode_t *y = x->right;
  AVLNode_t *z = y->right;

  AVLNode_t *b = y->left;

  x->right = b;
  x->rHeight = b == NULL ? 0 : max(b->lHeight, b->rHeight) + 1;

  y->left = x;
  y->lHeight = max(x->rHeight, x->lHeight) + 1;
  y->right = z;
  y->rHeight = max(z->rHeight, z->lHeight) + 1;

  return y;
}

AVLNode_t *leftRightRotate(AVLNode_t *node) {
  AVLNode_t *x = node;
  AVLNode_t *y = x->left;
  AVLNode_t *z = y->right;

  AVLNode_t *b = z->left;
  AVLNode_t *c = z->right;

  y->right = b;
  y->rHeight = b != NULL ? max(b->lHeight, b->rHeight) + 1 : 0;

  x->left = c;
  x->lHeight = c != NULL ? max(c->lHeight, c->rHeight) + 1 : 0;

  z->left = y;
  z->lHeight = max(y->rHeight, y->lHeight) + 1;
  z->right = x;
  z->rHeight = max(x->rHeight, x->lHeight) + 1;

  return z;
}

AVLNode_t *rightLeftRotate(AVLNode_t *node) {
  AVLNode_t *x = node;
  AVLNode_t *y = x->right;
  AVLNode_t *z = y->left;

  AVLNode_t *b = z->left;
  AVLNode_t *c = z->right;

  y->left = c;
  y->lHeight = c != NULL ? max(c->lHeight, c->rHeight) + 1 : 0;

  x->right = b;
  x->rHeight = b != NULL ? max(b->lHeight, b->rHeight) + 1 : 0;

  z->right = y;
  z->rHeight = max(y->rHeight, y->lHeight) + 1;
  z->left = x;
  z->lHeight = max(x->rHeight, x->lHeight) + 1;

  return z;
}

AVLNode_t *rotate(AVLNode_t *node, int side1, int side2) {
  if (side1 && side2) { // right right
    return rightRotate(node);
  } else if (side1) { // right left
    return rightLeftRotate(node);
  } else if (side2) { // left right
    return leftRightRotate(node);
  } else { // left left
    return leftRotate(node);
  }
}

void backward_pass(AVLBinaryTree_t *tree, Stack_t *stack, BoolStack_t *bstack,
                   int depth) {
  int prevBool;

  int length = depth;
  while (length > 0) {
    AVLNode_t *node = sPop(stack);
    int b = bsPop(bstack);
    if (b && node->rHeight < depth - length + 1) {
      node->rHeight++;
    } else if (!b && node->lHeight < depth - length + 1) {
      node->lHeight++;
    }
    if (abs(node->rHeight - node->lHeight) > 1) {
      AVLNode_t *change = rotate(node, b, prevBool);
      if (length == 1) {
        tree->root = change;
      } else if (bsPop(bstack)) {
        ((AVLNode_t *)sPop(stack))->right = change;
      } else {
        ((AVLNode_t *)sPop(stack))->left = change;
      }
      break;
    }
    prevBool = b;
    length--;
  }
}

// --- Public Function Prototypes ---
AVLBinaryTree_t *newAVLBinaryTree(int (*compare_func)(const void *value1,
                                                      const void *value2)) {
  AVLBinaryTree_t *tree = malloc(sizeof(AVLBinaryTree_t));
  if (tree == NULL) {
    return NULL;
  }

  tree->length = 0;
  tree->compare_func = compare_func;
  tree->root = NULL;

  return tree;
}

AVLNode_t *avlFind(AVLBinaryTree_t *tree, void *value) {
  AVLNode_t *current = tree->root;

  while (current != NULL) {
    int comparison = tree->compare_func(value, current->value);
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

AVL_STATUS avlInsert(AVLBinaryTree_t *tree, void *value, AVLNode_t **node) {
  AVLNode_t *newNode = malloc(sizeof(AVLNode_t));
  if (newNode == NULL) {
    return AVL_ERROR;
  }

  newNode->value = value;
  newNode->right = NULL;
  newNode->left = NULL;
  newNode->lHeight = 0;
  newNode->rHeight = 0;

  tree->length++;
  if (tree->root == NULL) {
    tree->root = newNode;
    *node = newNode;
    return AVL_OK;
  }

  // Forward pass
  int height = max(tree->root->rHeight, tree->root->lHeight);
  Stack_t *bstack = newBoolStack(height + 1);
  Stack_t *stack = newStack(height + 1);

  int depth = 0;
  AVLNode_t *current = tree->root;
  while (1) {
    depth++;
    sPush(stack, current);
    int comparison = tree->compare_func(value, current->value);
    if (comparison < 0) {
      bsPush(bstack, 0);
      if (current->left == NULL) {
        current->left = newNode;
        break;
      } else {
        current = current->left;
      }
    } else if (comparison > 0) {
      bsPush(bstack, 1);
      if (current->right == NULL) {
        current->right = newNode;
        break;
      } else {
        current = current->right;
      }
    } else {
      tree->length--;
      free(newNode);
      sDestroy(stack);
      bsDestroy(bstack);
      return AVL_DUPLICATE;
    }
  }

  backward_pass(tree, stack, bstack, depth);

  bsDestroy(bstack);
  sDestroy(stack);
  *node = newNode;
  return AVL_OK;
}

void debugTree(AVLNode_t *node) {
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

AVL_STATUS avlRemove(AVLBinaryTree_t *tree, const void *value) {
  if (tree->root == NULL) {
    return AVL_NOT_FOUND;
  }

  int height = max(tree->root->rHeight, tree->root->lHeight);
  Stack_t *stack = newStack(height);
  if (stack == NULL) {
    return AVL_ERROR;
  }
  BoolStack_t *bstack = newBoolStack(height);
  if (bstack == NULL) {
    sDestroy(stack);
    return AVL_ERROR;
  }

  AVLNode_t *node = tree->root;
  int comparison = tree->compare_func(value, node->value);
  while (comparison != 0) {
    sPush(stack, node);
    if (comparison < 0) {
      bsPush(bstack, 0);
      node = node->left;
    } else {
      bsPush(bstack, 1);
      node = node->right;
    }
    if (node == NULL) {
      break;
    }
    comparison = tree->compare_func(value, node->value);
  }

  if (node == NULL) {
    sDestroy(stack);
    bsDestroy(bstack);
    return AVL_NOT_FOUND;
  }
  void *ret = node->value;

  int rNull = node->right == NULL;
  int lNull = node->left == NULL;
  int stackLength = sLength(stack);
  if (rNull && lNull) {
    if (stackLength == 0) {
      tree->root = NULL;
    } else {
      AVLNode_t *prev = sPeek(stack);
      if (bsPeek(bstack)) {
        prev->right = NULL;
      } else {
        prev->left = NULL;
      }
    }
    free(node);
  } else if (lNull) {
    AVLNode_t *replacement = node->right;
    if (stackLength == 0) {
      tree->root = replacement;
    } else {
      AVLNode_t *prev = sPeek(stack);
      if (bsPeek(bstack)) {
        prev->right = replacement;
      } else {
        prev->left = replacement;
      }
    }
    free(node);
  } else if (rNull) {
    AVLNode_t *replacement = node->left;
    if (stackLength == 0) {
      tree->root = replacement;
    } else {
      AVLNode_t *prev = sPeek(stack);
      if (bsPeek(bstack)) {
        prev->right = replacement;
      } else {
        prev->left = replacement;
      }
    }
    free(node);
  } else {
    sPush(stack, node);
    AVLNode_t *replacement = node->right;
    bsPush(bstack, 1);
    while (replacement->left != NULL) {
      sPush(stack, replacement);
      replacement = replacement->left;
      bsPush(bstack, 0);
    }

    if (node->right == replacement) {
      node->value = replacement->value;
      node->right = replacement->right;
    } else {
      node->value = replacement->value;
      AVLNode_t *prev = sPeek(stack);
      if (bsPeek(bstack)) {
        prev->right = replacement->right;
      } else {
        prev->left = replacement->right;
      }
    }
    free(replacement);
  }

  while (sLength(stack) > 0) {
    AVLNode_t *node = sPop(stack);

    if (bsPop(bstack)) {
      node->rHeight = node->right == NULL
                          ? 0
                          : max(node->right->lHeight, node->right->rHeight) + 1;
    } else {
      node->lHeight = node->left == NULL
                          ? 0
                          : max(node->left->lHeight, node->left->rHeight) + 1;
    }

    if (abs(node->rHeight - node->lHeight) > 1) {
      int side1 = node->rHeight > node->lHeight;
      int side2;
      if (side1) {
        int unbalance = node->right->rHeight - node->right->lHeight;
        side2 = unbalance == 0 ? side1 : unbalance > 0;
      } else {
        int unbalance = node->left->rHeight - node->left->lHeight;
        side2 = unbalance == 0 ? side1 : unbalance > 0;
      }

      AVLNode_t *rotated = rotate(node, side1, side2);
      if (sLength(stack) == 0) {
        tree->root = rotated;
      } else {
        AVLNode_t *prev = sPeek(stack);
        if (bsPeek(bstack)) {
          prev->right = rotated;
        } else {
          prev->left = rotated;
        }
      }
    }
  }

  tree->length--;
  sDestroy(stack);
  bsDestroy(bstack);
  return AVL_OK;
}

void avlDestroy(AVLBinaryTree_t *tree) {
  if (tree->root == NULL) {
    free(tree);
    return;
  }

  int height = max(tree->root->rHeight, tree->root->lHeight);
  AVLNode_t **nodes = malloc(sizeof(AVLNode_t *) * (height + 1));
  nodes[0] = tree->root;
  int i = 0;

  while (i > -1) {
    AVLNode_t *l = nodes[i]->left;
    AVLNode_t *r = nodes[i]->right;
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

void avlDestroyAll(AVLBinaryTree_t *tree) {
  if (tree->root == NULL) {
    free(tree);
    return;
  }

  int height = max(tree->root->rHeight, tree->root->lHeight);
  AVLNode_t **nodes = malloc(sizeof(AVLNode_t *) * (height + 1));
  nodes[0] = tree->root;

  int i = 0;
  while (i > -1) {
    AVLNode_t *l = nodes[i]->left;
    AVLNode_t *r = nodes[i]->right;
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

void avlPrintTree(AVLBinaryTree_t *tree,
                  void (*repr)(AVLNode_t *value, char *buffer, int bufferSize),
                  int bufferSize) {
  if (tree->root == NULL) {
    printf("Empty tree\n");
    return;
  }
  int height = max(tree->root->rHeight, tree->root->lHeight);
  if (height > 6) {
    printf("Tree too big to print\n");
    return;
  }

  AVLNode_t **nodes = calloc(pow(2, height + 1) - 1, sizeof(AVLNode_t *));
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
  int maxSpace = pow(2, height) * bufferSize;
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
