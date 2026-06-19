#include "../include/queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const int numTests = 1000;
const int MIN = -10000000;
const int MAX = 10000000;

int rand_int(int min, int max) {
  int range = max - min + 1;
  return (rand() % range) + min;
}

void destroy_value(void *value) { free(value); }

void test_nulls() {
  queue_t *q = new_queue(1);
  assert(queue_add(q, NULL) == QUEUE_OK);
  assert(queue_size(q) == 1);

  queue_result_t result = queue_peek(q);
  assert(result.status == QUEUE_OK && result.value == NULL);
  result = queue_remove(q);
  assert(result.status == QUEUE_OK && result.value == NULL);

  assert(queue_size(q) == 0);
  queue_destroy(q, NULL);
}

void test_destroy_with_function() {
  queue_t *q = new_queue(1);
  assert(queue_add(q, malloc(sizeof(int))) == QUEUE_OK);
  assert(queue_size(q) == 1);

  queue_destroy(q, destroy_value);
}

void check_empty_queue(queue_t *queue) {
  assert(queue_size(queue) == 0);

  queue_result_t result = queue_peek(queue);
  assert(result.status == QUEUE_EMPTY && result.value == NULL);
  result = queue_remove(queue);
  assert(result.status == QUEUE_EMPTY && result.value == NULL);
}

void test_basic(int item_count) {
  queue_t *queue = new_queue(1);

  check_empty_queue(queue);

  int *values = malloc(sizeof(int) * item_count);
  for (int i = 0; i < item_count; i++) {
    values[i] = rand_int(MIN, MAX);

    assert(queue_add(queue, &values[i]) == QUEUE_OK);

    queue_result_t result = queue_peek(queue);
    assert(result.status != QUEUE_ERROR);
    assert(result.status == QUEUE_OK && *(int *)result.value == values[0]);

    assert(queue_size(queue) == i + 1);
  }

  for (int i = 0; i < item_count; i++) {
    queue_result_t result = queue_peek(queue);
    assert(result.status != QUEUE_ERROR);
    assert(result.status == QUEUE_OK && *(int *)result.value == values[i]);

    result = queue_remove(queue);
    assert(result.status != QUEUE_ERROR);
    assert(result.status == QUEUE_OK && *(int *)result.value == values[i]);

    assert(queue_size(queue) == item_count - i - 1);
  }

  check_empty_queue(queue);

  queue_destroy(queue, NULL);

  free(values);
}

void test_random_adds_removes() {}

int main(int argc, char *argv[]) {
  test_nulls();
  test_destroy_with_function();
  test_random_adds_removes();

  int baseItemCount = 100;
  for (int factor = 1; factor < 1000; factor++) {
    test_basic(baseItemCount * factor);
  }

  printf("✅ All tests passed!\n");
  return 0;
}
