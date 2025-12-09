#include <stddef.h>

#ifndef STACK_H
#define STACK_H

typedef enum { S_ERROR, S_OK } S_STATUS;

typedef struct Stack Stack_t;

// --- Public Function Prototypes ---

/**
 * Creates a new Stack.
 * @param capacity Initial number of items.
 * @return A pointer to the new Stack, or NULL on failure.
 */
Stack_t *newStack(size_t capacity);

/**
 * Returns a Stack's length (number of values in it).
 * @param stack The Stack.
 * @return the number of items in it.
 */

int sLength(Stack_t *stack);
/**
 * Peeks the front element of a Stack.
 * @param stack The stack.
 * @return A pointer to the element, NULL if the stack is empty.
 */
void *sPeek(Stack_t *stack);

/**
 * Adds a value in a Stack.
 * Pointers to the item are stored (caller manages memory).
 * @param stack The Stack.
 * @param value The value to add.
 * @return the status of the operation
 */
S_STATUS sPush(Stack_t *stack, void *value);

/**
 * Removes the front element of a Stack.
 * @param stack The Stack.
 * @return NULL if the Stack is empty, the pointer to the value if it
 */
void *sPop(Stack_t *stack);

/**
 * Destroys the Stack and frees all allocated memory.
 * @param stack The Stack to destroy.
 */
void sDestroy(Stack_t *stack);

/**
 * Destroys the Stack and along with all the values in it.
 * After calling this method, accessing a element that had
 * been stored in the list can lead to erros due to it being
 * freed.
 * @param stack The Stack to destroy.
 */
void sDestroyAll(Stack_t *stack);

/**
 * Prints the Stack.
 * @param stack The Stack to print.
 */
void printStack(Stack_t *stack,
                void (*repr)(void *value, char *buffer, int bufferSize),
                int bufferSize);

#endif
