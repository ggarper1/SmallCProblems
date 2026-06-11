#include "../include/linked_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX = 1000000;
const int MIN = -1000000;

int rand_int(int min, int max) {
  int range = max - min + 1;
  return (rand() % range) + min;
}

void check_empty_linked_list(linked_list_t *list) {
  assert(linked_list_size(list) == 0);

  linked_list_result_t result = linked_list_peek_first(list);
  assert(result.status == LINKED_LIST_EMPTY && result.value == NULL);

  result = linked_list_peek_last(list);
  assert(result.status == LINKED_LIST_EMPTY && result.value == NULL);

  result = linked_list_pop_first(list);
  assert(result.status == LINKED_LIST_EMPTY && result.value == NULL);

  result = linked_list_pop_last(list);
  assert(result.status == LINKED_LIST_EMPTY && result.value == NULL);
}

void test_basic(int item_count) {
  linked_list_t *list = new_linked_list();

  check_empty_linked_list(list);

  int *values = malloc(sizeof(int) * (item_count * 2 + 1));
  int first_idx = 1, last_idx = 0;

  linked_list_result_t result;
  for (int i = 0; i < item_count; i++) {
    if (rand_int(0, 1)) {
      values[item_count - first_idx] = rand_int(MIN, MAX);
      assert(linked_list_add_first(list, &values[item_count - first_idx]) ==
             LINKED_LIST_OK);

      result = linked_list_peek_first(list);
      assert(result.status == LINKED_LIST_OK &&
             *(int *)result.value == values[item_count - first_idx]);

      first_idx++;

    } else {
      values[item_count + last_idx] = rand_int(MIN, MAX);
      assert(linked_list_add_last(list, &values[item_count + last_idx]) ==
             LINKED_LIST_OK);

      result = linked_list_peek_last(list);
      assert(result.status == LINKED_LIST_OK &&
             *(int *)result.value == values[item_count + last_idx]);

      last_idx++;
    }

    assert(linked_list_size(list) == i + 1);
  }

  for (int i = 0; i < item_count - 1; i++) {
    if (rand_int(0, 1)) {
      first_idx--;
      result = linked_list_pop_first(list);
      assert(result.status == LINKED_LIST_OK &&
             *(int *)result.value == values[item_count - first_idx]);

      result = linked_list_peek_first(list);
      assert(result.status == LINKED_LIST_OK &&
             *(int *)result.value == values[item_count - first_idx + 1]);

    } else {
      last_idx--;
      result = linked_list_pop_last(list);
      assert(result.status == LINKED_LIST_OK &&
             *(int *)result.value == values[item_count + last_idx]);

      result = linked_list_peek_last(list);
      assert(result.status == LINKED_LIST_OK &&
             *(int *)result.value == values[item_count + last_idx - 1]);
    }

    assert(linked_list_size(list) == item_count - i - 1);
  }

  result = linked_list_pop_last(list);
  assert(result.status == LINKED_LIST_OK &&
         *(int *)result.value == values[item_count + last_idx - 1]);

  check_empty_linked_list(list);

  linked_list_destroy(list, NULL);

  free(values);
}

void test_nulls() {
  linked_list_t *list = new_linked_list();

  assert(linked_list_add_first(list, NULL) == LINKED_LIST_OK);
  assert(linked_list_size(list) == 1);
  linked_list_result_t result = linked_list_peek_first(list);
  assert(result.status == LINKED_LIST_OK && result.value == NULL);

  assert(linked_list_add_last(list, NULL) == LINKED_LIST_OK);
  result = linked_list_peek_last(list);
  assert(result.status == LINKED_LIST_OK && result.value == NULL);
  assert(linked_list_size(list) == 2);

  linked_list_destroy(list, NULL);
}

void test_random_adds_pops() {
  linked_list_t *list = new_linked_list();

  check_empty_linked_list(list);

  int item_count = 500;

  int *values = malloc(sizeof(int) * (item_count * 2 + 1));
  int first_idx = 1, last_idx = 0;

  linked_list_result_t result;
  for (int i = 0; i < item_count; i++) {
    if (linked_list_size(list) == 0 || rand_int(0, 1)) {
      if (rand_int(0, 1)) {
        values[item_count - first_idx] = rand_int(MIN, MAX);
        assert(linked_list_add_first(list, &values[item_count - first_idx]) ==
               LINKED_LIST_OK);

        result = linked_list_peek_first(list);
        assert(result.status == LINKED_LIST_OK &&
               *(int *)result.value == values[item_count - first_idx]);

        first_idx++;

      } else {
        values[item_count + last_idx] = rand_int(MIN, MAX);
        assert(linked_list_add_last(list, &values[item_count + last_idx]) ==
               LINKED_LIST_OK);

        result = linked_list_peek_last(list);
        assert(result.status == LINKED_LIST_OK &&
               *(int *)result.value == values[item_count + last_idx]);

        last_idx++;
      }

      assert(linked_list_size(list) == first_idx + last_idx - 1);
    } else {
      if (rand_int(0, 1)) {
        first_idx--;

        result = linked_list_peek_first(list);
        assert(result.status == LINKED_LIST_OK &&
               *(int *)result.value == values[item_count - first_idx]);

        result = linked_list_pop_first(list);
        assert(result.status == LINKED_LIST_OK &&
               *(int *)result.value == values[item_count - first_idx]);

      } else {
        last_idx--;

        result = linked_list_peek_last(list);
        assert(result.status == LINKED_LIST_OK &&
               *(int *)result.value == values[item_count + last_idx]);

        result = linked_list_pop_last(list);
        assert(result.status == LINKED_LIST_OK &&
               *(int *)result.value == values[item_count + last_idx]);
      }

      assert(linked_list_size(list) == first_idx + last_idx - 1);
    }
  }

  linked_list_destroy(list, NULL);

  free(values);
}

void destroy_value(void *value) { free(value); }

void test_destroy_with_function() {
  linked_list_t *list = new_linked_list();

  assert(linked_list_add_first(list, malloc(sizeof(void *))) == LINKED_LIST_OK);

  linked_list_destroy(list, destroy_value);
}

int main(int argc, char *argv[]) {
  test_nulls();
  test_destroy_with_function();
  test_random_adds_pops();

  int base_item_count = 100;
  for (int factor = 1; factor < 1000; factor++) {
    test_basic(base_item_count * factor);
  }

  printf("✅ All tests passed!\n");
  return 1;
}
