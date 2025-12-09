#include "../include/stack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack {
  int capacity;
  int idx;
  void **values;
} Stack_t;

// --- Private Function ---
S_STATUS reallocateStack(Stack_t *stack) {
  stack->capacity *= 2;
  void **temp = malloc(sizeof(void *) * stack->capacity);
  if (temp == NULL) {
    return S_ERROR;
  }
  memcpy(temp, stack->values, sizeof(void *) * (stack->idx + 1));
  free(stack->values);
  stack->values = temp;
  return S_OK;
}

// --- Public Function  ---
Stack_t *newStack(size_t capacity) {
  if (capacity < 1) {
    return NULL;
  }
  Stack_t *stack = malloc(sizeof(Stack_t));
  if (stack == NULL) {
    return NULL;
  }
  stack->capacity = capacity;
  stack->idx = -1;
  stack->values = malloc(sizeof(void *) * capacity);
  if (stack->values == NULL) {
    free(stack);
    return NULL;
  }
  return stack;
}

int sLength(Stack_t *stack) { return stack->idx + 1; }

void *sPeek(Stack_t *stack) {
  if (stack->idx < 0) {
    return NULL;
  }
  return stack->values[stack->idx];
}

S_STATUS sPush(Stack_t *stack, void *value) {
  if (stack->idx + 1 == stack->capacity) {
    if (reallocateStack(stack) == S_ERROR) {
      return S_ERROR;
    }
  }
  stack->idx++;
  stack->values[stack->idx] = value;
  return S_OK;
}

void *sPop(Stack_t *stack) {
  if (stack->idx == -1) {
    return NULL;
  }
  void *ptr = stack->values[stack->idx];
  stack->idx--;
  return ptr;
}

void sDestroy(Stack_t *stack) {
  free(stack->values);
  free(stack);
}

void sDestroyAll(Stack_t *stack) {
  for (int i = 0; i <= stack->idx; i++) {
    free(stack->values[i]);
  }
  free(stack->values);
  free(stack);
}

void printStack(Stack_t *stack,
                void (*repr)(void *value, char *buffer, int bufferSize),
                int bufferSize) {
  if (sLength(stack) == 0) {
    printf("<- []\n");
    return;
  }

  char *buffer = malloc(sizeof(char) * bufferSize);
  repr(stack->values[stack->idx], buffer, bufferSize);
  printf("<- [%s", buffer);

  for (int i = stack->idx - 1; i > -1; i--) {
    repr(stack->values[i], buffer, bufferSize);
    printf(", %s", buffer);
  }
  printf("]\n");
  free(buffer);
}
