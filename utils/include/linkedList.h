#include <stddef.h>

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct LinkedList LinkedList_t;

// --- Public Function Prototypes ---

/**
 * Creates a new linked list.
 * @return A pointer to the new list, or NULL on failure.
 */
LinkedList_t *newLinkedList();

/**
 * Returns a linked list's length (number of values in it).
 * @param list The linked list.
 * @return the number of items in it.
 */
int llLength(LinkedList_t *list);

/**
 * Peeks the front element of a linked list.
 * @param list The linked list.
 * @return A pointer to the element, NULL if the list is empty.
 */
void *llPeekFirst(LinkedList_t *list);

/**
 * Peeks the last element of a linked list.
 * @param list The linked list.
 * @return A pointer to the element, NULL if the list is empty.
 */
void *llPeekLast(LinkedList_t *list);

/**
 * Adds a value at the end of the linked list.
 * Pointers to the item are stored (caller manages memory).
 * @param list The linked list.
 * @param value The value to add.
 */
int llAddLast(LinkedList_t *list, void *value);

/**
 * Adds a value at the start of the linked list.
 * Pointers to the item are stored (caller manages memory).
 * @param list The linked list.
 * @param value The value to add.
 */
int llAddFirst(LinkedList_t *list, void *value);

/**
 * Removes the front element of a linked list.
 * @param list The Stack.
 * @return NULL if the list is empty, the pointer to the value if it
 */
void *llPopFirst(LinkedList_t *list);

/**
 * Removes the last element of a linked list.
 * @param list The linked list.
 * @return NULL if the list is empty, the pointer to the value if it isn't.
 */
void *llPopLast(LinkedList_t *list);

/**
 * Destroys the linked list and frees all allocated memory
 * (but not the one for the values stored).
 * @param list The linked list to destroy.
 */
void llDestroy(LinkedList_t *list);

/**
 * Destroys the linked list and frees all allocated memory
 * including the memory allocated for the values.
 * @param list The linked list to destroy.
 */
void llDestroyAll(LinkedList_t *list);

/**
 * Prints the linked list.
 * @param list The Stack to print.
 */
void printLinkedList(LinkedList_t *list,
                     void (*repr)(void *value, char *buffer, int bufferSize),
                     int bufferSize);

#endif
