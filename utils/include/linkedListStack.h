#include <stddef.h>

#ifndef LINKED_LIST_STACK_H
#define LINKED_LIST_STACK_H

typedef enum { LLS_ERROR, LLS_OK } LLS_STATUS;

typedef struct LLStack LLStack_t;

// --- Public Function Prototypes ---

/**
 * Creates a new Stack.
 * @param capacity Initial number of items.
 * @return A pointer to the new Stack, or NULL on failure.
 */
LLStack_t *newLLStack();

/**
 * Returns a Stack's length (number of values in it).
 * @param stack The Stack.
 * @return the number of items in it.
 */

int llsLength(LLStack_t *stack);
/**
 * Peeks the front element of a Stack.
 * @param stack The stack.
 * @return A pointer to the element, NULL if the stack is empty.
 */
void *llsPeek(LLStack_t *stack);

/**
 * Adds a value in a Stack.
 * Pointers to the item are stored (caller manages memory).
 * @param stack The Stack.
 * @param value The value to add.
 * @return the status of the operation (LLS_OK or LLS_ERROR).
 */
LLS_STATUS llsPush(LLStack_t *stack, void *value);

/**
 * Removes the front element of a Stack.
 * @param stack The Stack.
 * @return NULL if the Stack is empty, the pointer to the value if it
 */
void *llsPop(LLStack_t *stack);

/**
 * Destroys the Stack and frees all allocated memory.
 * @param stack The Stack to destroy.
 */
void llsDestroy(LLStack_t *stack);

/**
 * Destroys the Stack and along with all the values in it.
 * After calling this method, accessing a element that had
 * been stored in the list can lead to erros due to it being
 * freed.
 * @param stack The Stack to destroy.
 */
void llsDestroyAll(LLStack_t *stack);

/**
 * Prints the Stack.
 * @param stack The Stack to print.
 */
void printLLStack(LLStack_t *stack,
                  void (*repr)(void *value, char *buffer, int bufferSize),
                  int bufferSize);

#endif
