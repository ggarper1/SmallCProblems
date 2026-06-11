#include <stddef.h>
#include <stdint.h>

#ifndef STACK_H
#define STACK_H

typedef enum { STACK_ERROR, STACK_OK, STACK_EMPTY } STACK_STATUS;

typedef struct {
  STACK_STATUS status;
  void *value;
} stack_result_t;

typedef struct Stack my_stack_t;

/**
 * Creates a new Stack.
 * @param capacity: Initial number of items.
 * @return A pointer to the new Stack, or NULL on failure.
 */
my_stack_t *newStack(size_t capacity);

/**
 * Returns a Stack's size (number of values in it).
 * @param stack: The Stack.
 * @return the number of items in it.
 */
size_t stackSize(my_stack_t *stack);

/**
 * Peeks the front element of a Stack.
 * @param stack: The stack.
 * @return A stack_result_t struct with pointer to the element, if the stack is
 * not empty.
 */
stack_result_t stackPeek(my_stack_t *stack);

/**
 * Adds a value in a Stack.
 * Pointers to the item are stored (caller manages memory).
 * @param stack: The Stack.
 * @param value: The value to add.
 * @return the status of the operation (S_OK or S_ERROR).
 */
STACK_STATUS stackPush(my_stack_t *stack, void *value);

/**
 * Removes the front element of a Stack.
 * @param stack: The Stack.
 * @return A stack_result_t struct with pointer to the element, if the stack is
 * not empty.
 */
stack_result_t stackPop(my_stack_t *stack);

/**
 * Destroys the Stack and frees all allocated memory.
 * @param stack: The Stack to destroy.
 * @param destroyValue: The function with which ro destroy the values or NULL if
 * values should not be destroyed
 */
void stackDestroy(my_stack_t *stack, void (*destroyValue)(void *));

#endif
