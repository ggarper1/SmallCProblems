#include "../include/boolStack.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack {
  int capacity;
  int length;
  unsigned char *values;
} BoolStack_t;

// --- Private Function ---
BS_STATUS reallocateBoolStack(BoolStack_t *stack) {
  stack->capacity *= 2;
  unsigned char *temp = malloc(stack->capacity / 8);
  if (temp == NULL) {
    return BS_ERROR;
  }
  memcpy(temp, stack->values, stack->length / 8);
  free(stack->values);
  stack->values = temp;
  return BS_OK;
}

// --- Public Function Prototypes ---
BoolStack_t *newBoolStack(size_t capacity) {
  BoolStack_t *stack = malloc(sizeof(BoolStack_t));
  if (stack == NULL) {
    return stack;
  }

  stack->length = 0;
  stack->capacity = capacity + CHAR_BIT - (capacity % CHAR_BIT);

  stack->values = malloc(stack->capacity / 8);
  if (stack->values == NULL) {
    free(stack);
    return NULL;
  }

  return stack;
}

void printBits(unsigned char n, int j) {
  // Calculate total bits (usually 32 or 64 depending on architecture)
  int numBits = 8;

  // Iterate backwards from the highest bit index
  for (int i = numBits - 1; i >= 0; i--) {

    // Shift 'n' to the right by 'i' positions and check the last bit
    int bit = (n >> i) & 1;

    printf("%d", bit);
  }
  if (j) {
    printf("\n");
  } else {
    printf(" ");
  }
}

int bsLength(BoolStack_t *stack) { return stack->length; }

int bsPeek(BoolStack_t *stack) {
  if (stack->length == 0) {
    return 0;
  }

  int byteIdx = (stack->length - 1) / 8;
  int bitIdx = (stack->length - 1) % 8;

  return (((unsigned char)1 << bitIdx) & *(stack->values + byteIdx)) != 0;
}

BS_STATUS bsPush(BoolStack_t *stack, int value) {
  if (stack->length == stack->capacity) {
    if (reallocateBoolStack(stack) == BS_ERROR) {
      return BS_ERROR;
    }
  }

  stack->length++;
  int byteIdx = (stack->length - 1) / 8;
  int bitIdx = (stack->length - 1) % 8;

  unsigned char andMask = ~((unsigned char)(1) << bitIdx);
  unsigned char orMask = (unsigned char)(value != 0) << bitIdx;

  *(stack->values + byteIdx) &= andMask;
  *(stack->values + byteIdx) |= orMask;
  return BS_OK;
}

int bsPop(BoolStack_t *stack) {
  if (stack->length == 0) {
    return 0;
  }

  int byteIdx = (stack->length - 1) / 8;
  int bitIdx = (stack->length - 1) % 8;
  stack->length--;

  return (((unsigned char)1 << bitIdx) & *(stack->values + byteIdx)) != 0;
}

void bsDestroy(BoolStack_t *stack) {
  free(stack->values);
  free(stack);
}

void printBoolStack(BoolStack_t *stack) {
  printf("<- [");
  if (stack->length == 0) {
    printf("]\n");
    return;
  }
  unsigned char mask = 1;
  printf("%d", mask & *(stack->values));
  mask <<= 1;

  int byteIdx = 0;
  for (int i = 1; i < stack->length; i++) {
    printf("%d", (mask & *(stack->values + byteIdx)) != 0);
    if (i % 8 == 7) {
      mask = (unsigned char)1;
      byteIdx++;
    } else {
      mask <<= 1;
    }
  }
  printf("]\n");
}
