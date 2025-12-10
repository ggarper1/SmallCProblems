#include "../include/linkedListStack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  struct Node *prev;
  void *value;
} Node_t;

typedef struct LLStack {
  Node_t *top;
  size_t length;

} LLStack_t;

// --- Public Function Prototypes ---
LLStack_t *newLLStack() {
  LLStack_t *stack = malloc(sizeof(LLStack_t));
  if (stack == NULL) {
    return NULL;
  }

  stack->length = 0;
  stack->top = NULL;
  return stack;
}

int llsLength(LLStack_t *stack) { return stack->length; }

void *llsPeek(LLStack_t *stack) {
  if (stack->length == 0) {
    return NULL;
  }
  return stack->top->value;
}

LLS_STATUS llsPush(LLStack_t *stack, void *value) {
  Node_t *node = malloc(sizeof(Node_t));
  if (node == NULL) {
    return LLS_ERROR;
  }
  node->value = value;

  stack->length++;
  if (stack->length == 1) {
    stack->top = node;
    node->prev = NULL;
    return LLS_OK;
  }

  node->prev = stack->top;
  stack->top = node;
  return LLS_OK;
}

void *llsPop(LLStack_t *stack) {
  if (stack->length == 0) {
    return NULL;
  }
  stack->length--;

  void *value = stack->top->value;
  Node_t *old = stack->top;
  stack->top = old->prev;
  free(old);

  return value;
}

void llsDestroy(LLStack_t *stack) {
  Node_t *node = stack->top;
  Node_t *next;
  for (int i = 0; i < stack->length; i++) {
    next = node->prev;
    free(node);
    node = next;
  }
  free(stack);
}

void llsDestroyAll(LLStack_t *stack) {
  Node_t *node = stack->top;
  Node_t *next;
  for (int i = 0; i < stack->length; i++) {
    next = node->prev;
    free(node->value);
    free(node);
    node = next;
  }
  free(stack);
}

void printLLStack(LLStack_t *stack,
                  void (*repr)(void *value, char *buffer, int bufferSize),
                  int bufferSize) {

  if (stack->length == 0) {
    printf("<- []\n");
    return;
  }

  char *buffer = malloc(sizeof(char) * bufferSize);
  repr(stack->top->value, buffer, bufferSize);
  printf("<- [%s", buffer);

  Node_t *node = stack->top->prev;
  for (int i = 1; i < stack->length; i++) {
    repr(node->value, buffer, bufferSize);
    printf(", %s", buffer);
    node = node->prev;
  }
  printf("]\n");
  free(buffer);
}
