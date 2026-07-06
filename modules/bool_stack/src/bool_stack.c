#include "../include/bool_stack.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack {
  size_t capacity;
  size_t size;
  unsigned char *values;
} bool_stack_t;

// --- Private Function ---
static BOOL_STACK_STATUS reallocate_bool_stack(bool_stack_t *stack) {
  size_t new_capacity = stack->capacity * 2;
  unsigned char *temp = malloc(new_capacity >> 3);
  if (temp == NULL) {
    return BOOL_STACK_ERROR;
  }

  memcpy(temp, stack->values,
         (stack->size >> 3) + (int)((stack->size & 7) != 0));
  free(stack->values);
  stack->values = temp;
  stack->capacity = new_capacity;
  return BOOL_STACK_OK;
}

// --- Public Function Prototypes ---
bool_stack_t *new_bool_stack(size_t capacity) {
  bool_stack_t *stack = malloc(sizeof(bool_stack_t));
  if (stack == NULL) {
    return stack;
  }

  stack->size = 0;
  stack->capacity = capacity + CHAR_BIT - (capacity % CHAR_BIT);

  stack->values =
      malloc((stack->capacity >> 3) + (int)((stack->capacity & 7) != 0));
  if (stack->values == NULL) {
    free(stack);
    return NULL;
  }

  return stack;
}

int bool_stack_size(bool_stack_t *stack) { return stack->size; }

bool_stack_result_t bool_stack_peek(bool_stack_t *stack) {
  bool_stack_result_t result;
  if (stack->size == 0) {
    result.status = BOOL_STACK_EMPTY;
    result.value = 0;
    return result;
  }

  result.status = BOOL_STACK_OK;
  int byte = (stack->size - 1) >> 3;
  int bit = (stack->size - 1) & 7;
  result.value = (((unsigned char)1 << bit) & *(stack->values + byte)) != 0;

  return result;
}

BOOL_STACK_STATUS bool_stack_push(bool_stack_t *stack, int value) {
  if (stack->size == stack->capacity) {
    if (reallocate_bool_stack(stack) == BOOL_STACK_ERROR) {
      return BOOL_STACK_ERROR;
    }
  }

  stack->size++;
  int byte = (stack->size - 1) >> 3;
  int bit = (stack->size - 1) & 7;

  unsigned char and_mask = ~((unsigned char)(1) << bit);
  unsigned char or_mask = (unsigned char)(value != 0) << bit;

  *(stack->values + byte) &= and_mask;
  *(stack->values + byte) |= or_mask;
  return BOOL_STACK_OK;
}

bool_stack_result_t bool_stack_pop(bool_stack_t *stack) {
  bool_stack_result_t result;
  if (stack->size == 0) {
    result.status = BOOL_STACK_EMPTY;
    result.value = 0;
    return result;
  }

  result.status = BOOL_STACK_OK;
  int byte = (stack->size - 1) >> 3;
  int bit = (stack->size - 1) & 7;
  result.value = (((unsigned char)1 << bit) & *(stack->values + byte)) != 0;

  stack->size--;
  return result;
}

void bool_stack_destroy(bool_stack_t *stack) {
  free(stack->values);
  free(stack);
}
