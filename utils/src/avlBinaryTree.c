#include "../include/avlBinaryTree.h"
#include "../include/boolStack.h"
#include "../include/stack.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: remove tree->height cause it cannot be computed quickly
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

// --- Private Function Prototypes ---
AVLNode_t *leftRotate(AVLNode_t *node) {
  AVLNode_t *x = node;
  AVLNode_t *y = node->left;
  AVLNode_t *z = y->left;

  AVLNode_t *c = y->right;
  AVLNode_t *d = x->right;

  x->left = c;
  x->lHeight = c != NULL ? max(c->lHeight, c->rHeight) + 1 : 0;

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

  AVLNode_t *a = x->left;
  AVLNode_t *b = y->left;

  x->right = b;
  x->rHeight = b != NULL ? max(b->lHeight, b->rHeight) + 1 : 0;

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
  printf("Rotation: %d, %d\n", side1, side2);
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

  tree->height = -1;
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

AVLNode_t *avlInsert(AVLBinaryTree_t *tree, void *value) {
  AVLNode_t *newNode = malloc(sizeof(AVLNode_t));
  if (newNode == NULL) {
    return NULL;
  }

  newNode->value = value;
  newNode->right = NULL;
  newNode->left = NULL;
  newNode->lHeight = 0;
  newNode->rHeight = 0;

  tree->length++;
  if (tree->root == NULL) {
    tree->root = newNode;
    tree->height = 0;
    return newNode;
  }

  // Forward pass
  Stack_t *bstack = newBoolStack(tree->height + 1);
  Stack_t *stack = newStack(tree->height + 1);

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
      return NULL;
    }
  }

  // Backward pass to uopdate balances and perform rotations
  if (depth > tree->height) {
    tree->height = depth;
  }

  backward_pass(tree, stack, bstack, depth);

  bsDestroy(bstack);
  sDestroy(stack);
  return newNode;
}

// TODO:remove
void repr2(AVLNode_t *node, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *((int *)node->value));
}
void repr3(void *value, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *(int *)((AVLNode_t *)value)->value);
}

void *avlRemove(AVLBinaryTree_t *tree, const void *value) {
  if (tree->root == NULL) {
    return NULL;
  }

  // Forward pass
  Stack_t *bstack = newBoolStack(tree->height + 1);
  Stack_t *stack = newStack(tree->height + 1);

  AVLNode_t *current = tree->root;
  int depth = 0;
  while (current != NULL) {
    sPush(stack, current);
    depth++;

    int comparison = tree->compare_func(value, current->value);
    if (comparison < 0) {
      bsPush(bstack, 0);
      current = current->left;
    } else if (comparison > 0) {
      bsPush(bstack, 1);
      current = current->right;
    } else {
      break;
    }
  }

  if (current == NULL) {
    sDestroy(stack);
    bsDestroy(bstack);
    return NULL;
  }
  tree->length--;

  void *ptr = current->value;

  // Node deletion - handle different cases
  if (current->left == NULL && current->right == NULL) {
    printf("Both NULL\n");
    // Case 1: Leaf node
    sPop(stack);
    depth--;

    if (sLength(stack) > 0) {
      AVLNode_t *prev = sPeek(stack);
      if (bsPeek(bstack)) {
        prev->right = NULL;
        prev->rHeight = 0;
      } else {
        prev->left = NULL;
        prev->lHeight = 0;
      }
    } else {
      tree->root = NULL;
    }
    free(current);

  } else if (current->right == NULL) {
    printf("Right NULL\n");
    // Case 2: Only left child
    sPop(stack);
    depth--;

    if (sLength(stack) > 0) {
      AVLNode_t *prev = sPeek(stack);
      if (bsPeek(bstack)) {
        prev->right = current->left;
      } else {
        prev->left = current->left;
      }
    } else {
      tree->root = current->left;
    }
    free(current);

  } else if (current->left == NULL) {
    printf("Left NULL\n");
    // Case 3: Only right child
    sPop(stack);
    depth--;

    if (sLength(stack) > 0) {
      AVLNode_t *prev = sPeek(stack);
      if (bsPeek(bstack)) {
        prev->right = current->right;
      } else {
        prev->left = current->right;
      }
    } else {
      tree->root = current->right;
    }
    free(current);

  } else {
    printf("None NULL\n");
    // Case 4: Two children - find successor
    bsPush(bstack, 1); // Going right first

    AVLNode_t *successor = current->right;
    sPush(stack, successor);
    depth++;

    // Go all the way left
    while (successor->left != NULL) {
      bsPush(bstack, 0); // Going left
      successor = successor->left;
      sPush(stack, successor);
      depth++;
    }

    // Swap values
    current->value = successor->value;

    // Remove successor (it has at most one right child)
    sPop(stack); // Pop the successor
    depth--;

    AVLNode_t *parent = sPeek(stack);
    AVLNode_t *child = successor->right;

    if (parent->left == successor) {
      parent->left = child;
    } else {
      parent->right = child;
    }

    free(successor);
  }

  // Backward pass - update heights and rebalance
  int length = depth;

  while (length > 0) {
    AVLNode_t *node = sPop(stack);
    bsPop(bstack);

    // Update heights based on children
    node->lHeight = (node->left == NULL)
                        ? 0
                        : max(node->left->lHeight, node->left->rHeight) + 1;
    node->rHeight = (node->right == NULL)
                        ? 0
                        : max(node->right->lHeight, node->right->rHeight) + 1;

    // Check if rotation is needed
    int balance = node->rHeight - node->lHeight;

    if (abs(balance) > 1) {
      int side1 = (balance > 0); // 1 if right-heavy, 0 if left-heavy
      int side2;

      if (side1) {
        // Right-heavy: check right child's balance
        int rightBalance = node->right->rHeight - node->right->lHeight;
        side2 = (rightBalance >= 0); // 1 for RR, 0 for RL
      } else {
        // Left-heavy: check left child's balance
        int leftBalance = node->left->rHeight - node->left->lHeight;
        side2 = (leftBalance >= 0); // 1 for LR, 0 for LL
      }

      AVLNode_t *change = rotate(node, side1, side2);

      if (length == 1) {
        // This was the root
        tree->root = change;
      } else {
        // Update parent's pointer
        if (bsPeek(bstack)) {
          ((AVLNode_t *)sPeek(stack))->right = change;
        } else {
          ((AVLNode_t *)sPeek(stack))->left = change;
        }
      }
    }

    length--;
  }

  sDestroy(stack);
  bsDestroy(bstack);

  return ptr;
}

void avlDestroy(AVLBinaryTree_t *tree) {
  if (tree->root == NULL) {
    free(tree);
    return;
  }

  AVLNode_t **nodes = malloc(sizeof(AVLNode_t *) * (tree->height + 1));
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

  AVLNode_t **nodes = malloc(sizeof(AVLNode_t *) * (tree->height + 1));
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
  if (tree->height > 6) {
    printf("Tree too big to print\n");
    return;
  }

  AVLNode_t **nodes = calloc(pow(2, tree->height + 1) - 1, sizeof(AVLNode_t *));
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
  int maxSpace = pow(2, tree->height) * bufferSize;
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
