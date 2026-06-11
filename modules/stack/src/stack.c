#include "../include/stack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack {
  size_t capacity;
  int size;
  void **values;
} my_stack_t;

static STACK_STATUS reallocateStack(my_stack_t *stack) {
  size_t newCapacity = stack->capacity * 2;
  void **values = realloc(stack->values, sizeof(void *) * newCapacity);
  if (values == NULL) {
    return STACK_ERROR;
  }

  stack->capacity = newCapacity;
  stack->values = values;
  return STACK_OK;
}

my_stack_t *newStack(size_t capacity) {
  if (capacity < 1) {
    return NULL;
  }

  my_stack_t *stack = malloc(sizeof(stack_t));
  if (stack == NULL) {
    return NULL;
  }

  stack->values = malloc(sizeof(void *) * capacity);
  if (stack->values == NULL) {
    free(stack);
    return NULL;
  }

  stack->capacity = capacity;
  stack->size = -1;

  return stack;
}

size_t stackSize(my_stack_t *stack) { return stack->size + 1; }

stack_result_t stackPeek(my_stack_t *stack) {
  stack_result_t result;
  if (stack == NULL) {
    result.status = STACK_ERROR;
    result.value = NULL;
    return result;
  }

  if (stack->size < 0) {
    result.status = STACK_EMPTY;
    result.value = NULL;
    return result;
  }

  result.status = STACK_OK;
  result.value = stack->values[stack->size];
  return result;
}

STACK_STATUS stackPush(my_stack_t *stack, void *value) {
  if (stack == NULL) {
    return STACK_ERROR;
  }

  if (stack->size + 1 == stack->capacity) {
    if (reallocateStack(stack) == STACK_ERROR) {
      return STACK_ERROR;
    }
  }

  stack->size++;
  stack->values[stack->size] = value;
  return STACK_OK;
}

stack_result_t stackPop(my_stack_t *stack) {
  stack_result_t result;
  if (stack == NULL) {
    result.status = STACK_ERROR;
    result.value = NULL;
    return result;
  }

  if (stack->size == -1) {
    result.status = STACK_EMPTY;
    result.value = NULL;
    return result;
  }

  result.status = STACK_OK;
  result.value = stack->values[stack->size];
  stack->size--;
  return result;
}

void stackDestroy(my_stack_t *stack, void (*destroyValue)(void *)) {
  if (stack == NULL) {
    return;
  }

  if (destroyValue != NULL) {
    for (int i = 0; i <= stack->size; i++) {
      destroyValue(stack->values[i]);
    }
  }

  free(stack->values);
  free(stack);
}
