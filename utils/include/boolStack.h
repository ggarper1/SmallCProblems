#include <stddef.h>

#ifndef BOOLSTACK_H
#define BOOLSTACK_H

typedef struct Stack BoolStack_t;

// --- Public Function Prototypes ---

/**
 * Creates a new Stack.
 * @param capacity Initial number of items.
 * @return A pointer to the new Stack, or NULL on failure.
 */
BoolStack_t *newBoolStack(size_t capacity);

/**
 * Returns a Stack's length (number of values in it).
 * @param stack The Stack.
 * @return the number of items in it.
 */

int bsLength(BoolStack_t *stack);
/**
 * Peeks the front element of a Stack.
 * @param stack The stack.
 * @return A pointer to the element, NULL if the stack is empty.
 */
int bsPeek(BoolStack_t *stack);

/**
 * Adds a value in a Stack.
 * Pointers to the item are stored (caller manages memory).
 * @param stack The Stack.
 * @param value The value to add.
 */
void bsPush(BoolStack_t *stack, int value);

/**
 * Removes the front element of a Stack.
 * @param stack The Stack.
 * @return NULL if the Stack is empty, the pointer to the value if it
 */
int bsPop(BoolStack_t *stack);

/**
 * Destroys the Stack and frees all allocated memory.
 * @param stack The Stack to destroy.
 */
void bsDestroy(BoolStack_t *stack);

/**
 * Prints the Stack.
 * @param stack The Stack to print.
 */
void printBoolStack(BoolStack_t *stack);

#endif
