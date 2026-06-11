#include <stddef.h>

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef enum {
  LINKED_LIST_OK,
  LINKED_LIST_EMPTY,
  LINKED_LIST_ERROR
} LINKED_LIST_STATUS;

typedef struct linked_list_t linked_list_t;

typedef struct {
  LINKED_LIST_STATUS status;
  void *value;
} linked_list_result_t;

// --- Public Function Prototypes ---

/**
 * Creates a new linked list.
 * @return A pointer to the new list, or NULL on failure.
 */
linked_list_t *new_linked_list();

/**
 * Returns a linked list's length (number of values in it).
 * @param list The linked list.
 * @return the number of items in it.
 */
size_t linked_list_size(linked_list_t *list);

/**
 * Peeks the front element of a linked list.
 * @param list The linked list.
 * @return A result struct indicating if the list is empty. If it isn't ir
 * provides the value.
 */
linked_list_result_t linked_list_peek_first(linked_list_t *list);

/**
 * Peeks the last element of a linked list.
 * @param list The linked list.
 * @return A result struct indicating if the list is empty. If it isn't ir
 * provides the value.

 */
linked_list_result_t linked_list_peek_last(linked_list_t *list);

/**
 * Adds a value at the end of the linked list.
 * Pointers to the item are stored (caller manages memory).
 * @param list The linked list.
 * @param value The value to add.
 * @return the status of the operation (LL_OK or LL_ERROR).
 */
LINKED_LIST_STATUS linked_list_add_first(linked_list_t *list, void *value);

/**
 * Adds a value at the start of the linked list.
 * Pointers to the item are stored (caller manages memory).
 * @param list The linked list.
 * @param value The value to add.
 * @return the status of the operation (LL_OK or LL_ERROR).
 */
LINKED_LIST_STATUS linked_list_add_last(linked_list_t *list, void *value);

/**
 * Removes the front element of a linked list.
 * @param list The Stack.
 * @return A result struct indicating if the list is empty. If it isn't ir
 * provides the value.

 */
linked_list_result_t linked_list_pop_first(linked_list_t *list);

/**
 * Removes the last element of a linked list.
 * @param list The linked list.
 * @return A result struct indicating if the list is empty. If it isn't ir
 * provides the value.

 */
linked_list_result_t linked_list_pop_last(linked_list_t *list);

/**
 * Destroys the linked list and frees all allocated memory
 * (but not the one for the values stored).
 * @param list The linked list to destroy.
 * @param destroy_value The function used to destroy the values or NULL.
 */
void linked_list_destroy(linked_list_t *list, void (*destroy_value)(void *));

#endif
