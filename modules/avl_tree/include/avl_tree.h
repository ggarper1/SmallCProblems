#include <stddef.h>

#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef enum {
  AVL_TREE_NOT_FOUND,
  AVL_TREE_DUPLICATE,
  AVL_TREE_ERROR,
  AVL_TREE_OK
} AVL_TREE_STATUS;

typedef struct avl_tree_node {
  void *value;
  int l_height;
  int r_height;
  struct avl_tree_node *right;
  struct avl_tree_node *left;
} avl_tree_node_t;

typedef struct avl_tree {
  size_t size;
  avl_tree_node_t *root;
  int (*compare_func)(const void *item1, const void *item2);
} avl_tree_t;

typedef struct avl_tree_result {
  AVL_TREE_STATUS status;
  avl_tree_node_t *node;
} avl_tree_result_t;

typedef struct avl_tree_remove_result {
  AVL_TREE_STATUS status;
  void *value;
} avl_tree_remove_result_t;

// --- Public Function Prototypes ---

/**
 * Creates a new AVL Tree.
 * @param compare_func Function to compare items.
 * @return A pointer to the new AVL Tree, or NULL on failure.
 */
avl_tree_t *new_avl_tree(int (*compare_func)(const void *item1,
                                             const void *item2));

/**
 * Returns an AVL Tree's size (number of values in it).
 * @param tree The AVL Tree.
 * @return the number of items in it (0 if tree is NULL).
 */
size_t avl_tree_size(avl_tree_t *tree);

/**
 * Checks if an element is in the AVL Tree.
 * @param tree The AVL Tree.
 * @param value Pointer to the value to search for.
 * @return AVL_TREE_OK and the node containing the value if found,
 * AVL_TREE_NOT_FOUND and NULL otherwise (AVL_TREE_ERROR if tree is NULL).
 */
avl_tree_result_t avl_tree_find(avl_tree_t *tree, const void *value);

/**
 * Inserts a value in the AVL Tree.
 * Pointers to the value are stored (caller manages memory).
 * @param tree The AVL Tree.
 * @param value The value to insert.
 * @return AVL_TREE_OK and the new node, AVL_TREE_DUPLICATE and the node that
 * already held an equal value, or AVL_TREE_ERROR and NULL.
 */
avl_tree_result_t avl_tree_insert(avl_tree_t *tree, void *value);

/**
 * Removes a value from the AVL Tree.
 * @param tree The AVL Tree.
 * @param value The value to remove.
 * @return AVL_TREE_OK and the pointer that was stored in the tree (caller
 * frees it if needed), or AVL_TREE_NOT_FOUND / AVL_TREE_ERROR and NULL.
 */
avl_tree_remove_result_t avl_tree_remove(avl_tree_t *tree, const void *value);

/**
 * Destroys the AVL Tree and frees all allocated memory.
 * @param tree The AVL Tree to destroy.
 * @param destroy_value The function used to destroy the values, or NULL if
 * values should not be destroyed.
 */
void avl_tree_destroy(avl_tree_t *tree, void (*destroy_value)(void *));

/**
 * Prints the AVL Tree (only trees with height <= 6).
 * @param tree The AVL Tree to print.
 * @param repr Function that writes a node's representation into buffer.
 * @param buffer_size Size of the buffer passed to repr.
 */
void avl_tree_print(avl_tree_t *tree,
                    void (*repr)(avl_tree_node_t *node, char *buffer,
                                 size_t buffer_size),
                    size_t buffer_size);

#endif
