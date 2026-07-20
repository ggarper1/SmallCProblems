#include "../include/bool_stack.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int num_tests = 10000;
const int MIN = -10000000;
const int MAX = 10000000;

int rand_int(int min, int max) {
  int range = max - min + 1;
  return (rand() % range) + min;
}

void test_push_peek(bool_stack_t *stack, int size, int **items) {
  *items = malloc(sizeof(int) * size);
  for (int i = 0; i < size; i++) {
    (*items)[i] = rand_int(0, 1);
    assert(bool_stack_push(stack, (*items)[i]) == BOOL_STACK_OK);
    bool_stack_result_t result = bool_stack_peek(stack);
    assert(result.status == BOOL_STACK_OK && result.value == (*items)[i]);
  }
}

void test_pop_peek(bool_stack_t *stack, int *items, int size) {
  for (int i = 1; i <= size; i++) {
    bool_stack_result_t result = bool_stack_peek(stack);
    assert(result.status == BOOL_STACK_OK && result.value == items[size - i]);

    result = bool_stack_pop(stack);
    assert(result.status == BOOL_STACK_OK && result.value == items[size - i]);
  }

  bool_stack_result_t result = bool_stack_peek(stack);
  assert(result.status == BOOL_STACK_EMPTY);

  result = bool_stack_pop(stack);
  assert(result.status == BOOL_STACK_EMPTY);
}

int main(int argc, char *argv[]) {
  for (int i = 0; i < num_tests; i++) {
    bool_stack_t *stack = new_bool_stack(2);

    int *items;
    test_push_peek(stack, i * 10, &items);

    test_pop_peek(stack, items, i * 10);

    free(items);
    bool_stack_destroy(stack);
  }
  printf("✅ All tests passed!\n");
  return 0;
}
