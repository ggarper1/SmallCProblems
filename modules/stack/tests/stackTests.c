#include "../include/stack.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX = -10000000;
const int MIN = 10000000;

int randInt(int min, int max) {
  int range = max - min + 1;
  return (rand() % range) + min;
}

void checkEmptyStack(Stack_t *stack) {
  StackResult result = stackPeek(stack);
  assert(result.status == STACK_EMPTY && result.value == NULL);

  result = stackPop(stack);
  assert(result.status == STACK_EMPTY && result.value == NULL);

  assert(stackSize(stack) == 0);
}

void testBasicStack(int itemCount) {

  Stack_t *stack = newStack(1);

  checkEmptyStack(stack);

  int *values = malloc(sizeof(int) * itemCount);
  for (int i = 0; i < itemCount; i++) {
    values[i] = randInt(MIN, MAX);

    STACK_STATUS status = stackPush(stack, &values[i]);
    assert(status == STACK_OK);

    StackResult result = stackPeek(stack);
    assert(result.status != STACK_ERROR);
    assert((result.status == STACK_EMPTY && result.value == NULL && i == 0) ||
           (result.status == STACK_OK && *(int *)result.value == values[i]));

    assert(stackSize(stack) == i + 1);
  }

  for (int i = itemCount - 1; i >= 0; i--) {
    StackResult result = stackPeek(stack);
    assert(result.status != STACK_ERROR);
    assert((result.status == STACK_EMPTY && result.value == NULL && i == 0) ||
           (result.status == STACK_OK && *(int *)result.value == values[i]));

    result = stackPop(stack);
    assert(result.status != STACK_ERROR);
    assert((result.status == STACK_EMPTY && result.value == NULL && i == 0) ||
           (result.status == STACK_OK && *(int *)result.value == values[i]));

    assert(stackSize(stack) == i);
  }

  checkEmptyStack(stack);

  stackDestroy(stack, NULL);

  free(values);
}

void testNulls() {
  Stack_t *stack = newStack(1);
  assert(stackPush(stack, NULL) == STACK_OK);

  assert(stackSize(stack) == 1);

  StackResult result = stackPeek(stack);
  assert(result.status == STACK_OK && result.value == NULL);

  result = stackPop(stack);
  assert(result.status == STACK_OK && result.value == NULL);

  result = stackPeek(stack);
  assert(result.status == STACK_EMPTY && result.value == NULL);

  assert(stackSize(stack) == 0);

  stackDestroy(stack, NULL);
}

void testRandomPushPops() {
  Stack_t *stack = newStack(5);

  int *values = malloc(sizeof(int) * 100000);

  int size = 0;
  while (size < 5) {
    values[size] = randInt(MIN, MAX);
    assert(stackPush(stack, &values[size]) == STACK_OK);
    size++;
  }

  for (int i = 0; i < 100000; i++) {
    assert(stackSize(stack) == size);

    StackResult result = stackPeek(stack);

    assert((size > 0 && result.status == STACK_OK &&
            (int *)result.value == &values[size - 1]) ||
           (result.status == STACK_EMPTY && result.value == NULL));

    if (size == 0 || randInt(0, 1)) {
      values[size] = randInt(MIN, MAX);
      assert(stackPush(stack, &values[size]) == STACK_OK);
      size++;
    } else {
      result = stackPop(stack);
      assert((result.status == STACK_OK &&
              *(int *)result.value == values[size - 1] && size > 0) ||
             (result.status == STACK_EMPTY && result.value == NULL));
      size--;
    }
  }

  stackDestroy(stack, NULL);
  free(values);
}

void freeValue(void *value) { free(value); }

void testDestroyWithFunction() {
  Stack_t *stack = newStack(5);
  int *v = malloc(sizeof(int));
  stackPush(stack, v);

  stackDestroy(stack, freeValue);
}

int main(int argc, char *argv[]) {
  testNulls();
  testDestroyWithFunction();
  testRandomPushPops();

  int baseItemCount = 100;
  for (int factor = 0; factor < 1000; factor++) {
    testBasicStack(baseItemCount * factor);
  }

  printf("✅ All tests passed!\n");
  return 0;
}
