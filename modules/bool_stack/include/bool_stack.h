#include <stddef.h>

#ifndef BOOL_STACK_H
#define BOOL_STACK_H

typedef enum {
  BOOL_STACK_ERROR,
  BOOL_STACK_OK,
  BOOL_STACK_EMPTY
} BOOL_STACK_STATUS;

typedef struct stack_result {
  BOOL_STACK_STATUS status;
  int value;
} bool_stack_result_t;

typedef struct stack bool_stack_t;

// --- Public Function Prototypes ---

/**
 * Creates a new Stack.
 * @param capacity Initial number of items.
 * @return A pointer to the new Stack, or NULL on failure.
 */
bool_stack_t *new_bool_stack(size_t capacity);

/**
 * Returns a Stack's length (number of values in it).
 * @param stack The Stack.
 * @return the number of items in it.
 */

int bool_stack_size(bool_stack_t *stack);
/**
 * Peeks the front element of a Stack.
 * @param stack The stack.
 * @return A pointer to the element, NULL if the stack is empty.
 */
bool_stack_result_t bool_stack_peek(bool_stack_t *stack);

/**
 * Adds a value in a Stack.
 * Pointers to the item are stored (caller manages memory).
 * @param stack The Stack.
 * @param value The value to add.
 * @return the status of the operation (BOOL_STACK_OK or BS_ERROR).
 */
BOOL_STACK_STATUS bool_stack_push(bool_stack_t *stack, int value);

/**
 * Removes the front element of a Stack.
 * @param stack The Stack.
 * @return NULL if the Stack is empty, the pointer to the value if it
 */
bool_stack_result_t bool_stack_pop(bool_stack_t *stack);

/**
 * Destroys the Stack and frees all allocated memory.
 * @param stack The Stack to destroy.
 */
void bool_stack_destroy(bool_stack_t *stack);

#endif
