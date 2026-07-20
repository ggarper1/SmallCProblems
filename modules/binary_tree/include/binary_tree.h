#include <stddef.h>
#include <stdio.h>

#ifndef BINARYTREE_H
#define BINARYTREE_H

typedef enum {
  BINARY_TREE_NOT_FOUND,
  BINARY_TREE_DUPLICATE,
  BINARY_TREE_ERROR,
  BINARY_TREE_OK
} BINARY_TREE_STATUS;

typedef struct node {
  void *value;
  struct node *right;
  struct node *left;
} binary_tree_node_t;

typedef struct binary_tree {
  size_t size;
  size_t height;
  binary_tree_node_t *root;
  int (*compare_func)(const void *item1, const void *item2);
  void (*free_item)(void *item);
} binary_tree_t;

typedef struct binary_tree_result {
  BINARY_TREE_STATUS status;
  binary_tree_node_t *node;
} binary_tree_result_t;

typedef struct binary_tree_remove_result {
  BINARY_TREE_STATUS status;
  void *value;
} binary_tree_remove_result_t;

// --- Public Function Prototypes ---

/**
 * Creates a new Binary Tree.
 * @param compare_func Function to compare items.
 * @return A pointer to the new BinaryTree, or NULL on failure.
 */
binary_tree_t *new_binary_tree(int (*compare_func)(const void *item1,
                                                   const void *item2));

/**
 * Checks if a element is in the Binary Tree.
 * @param tree The binary Tree.
 * @param item Pointer to the item.
 * @param node Pointer to the resulting node.
 * @return status and the node if it was found, NULL otherwise.
 */
binary_tree_result_t binary_tree_find(binary_tree_t *tree, void *item);

/**
 * Inserts a item in the Binary Tree.
 * Pointers to the item are stored (caller manages memory).
 * @param tree The Binary Tree.
 * @param item The item to insert.
 * @param node The resulting node, if the node already was
 * in the tree it will return the node.
 * @return the result of the operation (BT_ERROR, BT_DUPLICATE or BT_OK).
 */
binary_tree_result_t binary_tree_insert(binary_tree_t *tree, void *item);

/**
 * Removes a item pair from the Binary Tree.
 * @param tree The Binary Tree.
 * @param item The item to remove.
 * @return status and a pointer to the value removed.
 */
binary_tree_remove_result_t binary_tree_remove(binary_tree_t *tree,
                                               void *value);

/**
 * Destroys the Binary Tree and frees all allocated memory.
 * @param tree The BinaryTree to destroy.
 */
void binary_tree_destroy(binary_tree_t *tree, void (*destroy_value)(void *));

#endif
