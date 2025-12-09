#include <stddef.h>
#include <stdio.h>

#ifndef BINARYTREE_H
#define BINARYTREE_H

typedef enum { BT_NOT_FOUND, BT_DUPLICATE, BT_ERROR, BT_OK } BT_STATUS;

typedef struct BTNode {
  void *value;
  struct BTNode *right;
  struct BTNode *left;
} BTNode_t;

typedef struct BinaryTree {
  size_t length;
  int height;
  BTNode_t *root;
  int (*compare_func)(const void *item1, const void *item2);
} BinaryTree_t;

// --- Public Function Prototypes ---

/**
 * Creates a new Binary Tree.
 * @param compare_func Function to compare items.
 * @return A pointer to the new BinaryTree, or NULL on failure.
 */
BinaryTree_t *newBinaryTree(int (*compare_func)(const void *item1,
                                                const void *item2));

/**
 * Checks if a element is in the Binary Tree.
 * @param tree The binary Tree.
 * @param item Pointer to the item.
 * @param node Pointer to the resulting node.
 * @return The result of the operation
 */
BT_STATUS btFind(BinaryTree_t *tree, void *item, BTNode_t **node);

/**
 * Inserts a item in the Binary Tree.
 * Pointers to the item are stored (caller manages memory).
 * @param tree The Binary Tree.
 * @param item The item to insert.
 * @param node The resulting node, if the node already was
 * in the tree it will return the node.
 * @return the result of the operation.
 */
BT_STATUS btInsert(BinaryTree_t *tree, void *item, BTNode_t **node);

/**
 * Removes a item pair from the Binary Tree.
 * @param tree The Binary Tree.
 * @param item The item to remove.
 * @return the result of the operation.
 */
int btRemove(BinaryTree_t *tree, void *value);

/**
 * Destroys the Binary Tree and frees all allocated memory.
 * @param tree The BinaryTree to destroy.
 */
void btDestroy(BinaryTree_t *tree);

/**
 * Destroys the Binary Tree and along with all the values in it.
 * @param tree The Binary Tree to destroy.
 */
void btDestroyAll(BinaryTree_t *tree);

/**
 * Prints the Binary Tree.
 * @param tree The Binary Tree to print.
 */
void printTree(BinaryTree_t *tree,
               void (*repr)(BTNode_t *value, char *buffer, int bufferSize),
               int bufferSize);

#endif
