#include "../include/stack.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX = -10000000;
const int MIN = 10000000;

int rand_int(int min, int max) {
  int range = max - min + 1;
  return (rand() % range) + min;
}

void check_empty_stack(my_stack_t *stack) {
  stack_result_t result = stackPeek(stack);
  assert(result.status == STACK_EMPTY && result.value == NULL);

  result = stackPop(stack);
  assert(result.status == STACK_EMPTY && result.value == NULL);

  assert(stackSize(stack) == 0);
}

void testBasicStack(int itemCount) {

  my_stack_t *stack = newStack(1);

  check_empty_stack(stack);

  int *values = malloc(sizeof(int) * itemCount);
  for (int i = 0; i < itemCount; i++) {
    values[i] = rand_int(MIN, MAX);

    STACK_STATUS status = stackPush(stack, &values[i]);
    assert(status == STACK_OK);

    stack_result_t result = stackPeek(stack);
    assert(result.status != STACK_ERROR);
    assert((result.status == STACK_EMPTY && result.value == NULL && i == 0) ||
           (result.status == STACK_OK && *(int *)result.value == values[i]));

    assert(stackSize(stack) == i + 1);
  }

  for (int i = itemCount - 1; i >= 0; i--) {
    stack_result_t result = stackPeek(stack);
    assert(result.status != STACK_ERROR);
    assert((result.status == STACK_EMPTY && result.value == NULL && i == 0) ||
           (result.status == STACK_OK && *(int *)result.value == values[i]));

    result = stackPop(stack);
    assert(result.status != STACK_ERROR);
    assert((result.status == STACK_EMPTY && result.value == NULL && i == 0) ||
           (result.status == STACK_OK && *(int *)result.value == values[i]));

    assert(stackSize(stack) == i);
  }

  check_empty_stack(stack);

  stackDestroy(stack, NULL);

  free(values);
}

void test_nulls() {
  my_stack_t *stack = newStack(1);
  assert(stackPush(stack, NULL) == STACK_OK);

  assert(stackSize(stack) == 1);

  stack_result_t result = stackPeek(stack);
  assert(result.status == STACK_OK && result.value == NULL);

  result = stackPop(stack);
  assert(result.status == STACK_OK && result.value == NULL);

  result = stackPeek(stack);
  assert(result.status == STACK_EMPTY && result.value == NULL);

  assert(stackSize(stack) == 0);

  stackDestroy(stack, NULL);
}

void test_random_push_pops() {
  my_stack_t *stack = newStack(5);

  int *values = malloc(sizeof(int) * 100000);

  int size = 0;
  while (size < 5) {
    values[size] = rand_int(MIN, MAX);
    assert(stackPush(stack, &values[size]) == STACK_OK);
    size++;
  }

  for (int i = 0; i < 100000; i++) {
    assert(stackSize(stack) == size);

    stack_result_t result = stackPeek(stack);

    assert((size > 0 && result.status == STACK_OK &&
            (int *)result.value == &values[size - 1]) ||
           (result.status == STACK_EMPTY && result.value == NULL));

    if (size == 0 || rand_int(0, 1)) {
      values[size] = rand_int(MIN, MAX);
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

void free_value(void *value) { free(value); }

void test_destroy_with_function() {
  my_stack_t *stack = newStack(5);
  int *v = malloc(sizeof(int));
  stackPush(stack, v);

  stackDestroy(stack, free_value);
}

int main(int argc, char *argv[]) {
  test_nulls();
  test_destroy_with_function();
  test_random_push_pops();

  int baseItemCount = 100;
  for (int factor = 0; factor < 1000; factor++) {
    testBasicStack(baseItemCount * factor);
  }

  printf("✅ All tests passed!\n");
  return 0;
}
