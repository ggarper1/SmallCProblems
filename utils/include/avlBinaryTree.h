#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifndef AVLBINARYTREE_H
#define AVLBINARYTREE_H

typedef enum { AVL_OK, AVL_ERROR, AVL_NOT_FOUND, AVL_DUPLICATE } AVL_STATUS;

typedef struct AVLNode {
  void *value;
  int lHeight;
  int rHeight;
  struct AVLNode *right;
  struct AVLNode *left;
} AVLNode_t;

typedef struct AVLBinaryTree {
  size_t length;
  int height;
  int capacity;
  AVLNode_t *root;
  int (*compare_func)(const void *item1, const void *item2);
} AVLBinaryTree_t;

// --- Public Function Prototypes ---

/**
 * Creates a new AVL Binary Tree.
 * @param compare_func Function to compare items.
 * @return A pointer to the new BinaryTree, or NULL on failure.
 */
AVLBinaryTree_t *newAVLBinaryTree(int (*compare_func)(const void *item1,
                                                      const void *item2));

/**
 * Checks if a element is in the AVL Binary Tree.
 * @param tree The binary Tree.
 * @param item Pointer to the item.
 * @return The node containing the element if it's in the tree, NULL otherwise.
 */
AVLNode_t *avlFind(AVLBinaryTree_t *tree, void *item);

/**
 * Inserts a item in the AVL Binary Tree.
 * Pointers to the item are stored (caller manages memory).
 * @param tree The AVL Binary Tree.
 * @param item The item to insert.
 * @param a pointer referencing to the newly create node.
 * @return the status of the operation.
 */
AVL_STATUS avlInsert(AVLBinaryTree_t *tree, void *item, AVLNode_t **node);

/**
 * Removes a item pair from the AVL Binary Tree.
 * @param tree The AVL Binary Tree.
 * @param item The item to remove.
 * @return the status of the operation.
 */
AVL_STATUS avlRemove(AVLBinaryTree_t *tree, const void *value);

/**
 * Destroys the AVL Binary Tree and frees all allocated memory.
 * @param tree The BinaryTree to destroy.
 */
void avlDestroy(AVLBinaryTree_t *tree);

/**
 * Destroys the AVL Binary Tree and along with all the values in it.
 * @param tree The AVL Binary Tree to destroy.
 */
void avlDestroyAll(AVLBinaryTree_t *tree);

/**
 * Prints the AVL Binary Tree.
 * @param tree The AVL Binary Tree to print.
 */
void avlPrintTree(AVLBinaryTree_t *tree,
                  void (*repr)(AVLNode_t *value, char *buffer, int bufferSize),
                  int bufferSize);

#endif
