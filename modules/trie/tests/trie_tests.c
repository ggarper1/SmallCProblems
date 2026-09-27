#include "../include/trie.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int ASCII_A = 97;
const int ASCII_B = 98;
const int ASCII_M = 109;
const int ASCII_Y = 121;
const int ASCII_Z = 122;

const int NUM_TESTS = 10;

int get_trie_string_length(trie_node_t *node, size_t *result,
                           size_t max_value_size, int indentation) {
  if (node == NULL) {
    return 0;
  }

  int num_children = 0, num_new_lines = 0;
  linked_list_node_t *next = node->children;
  while (next != NULL) {
    num_children += 1;
    num_new_lines += get_trie_string_length(next->node, result, max_value_size,
                                            indentation + 1) +
                     2;
    next = next->next;
  }

  *result = *result // Previous value
            + 2     // Accounts for tthe {key}:{value if it exists}\n
            + max_value_size * (node->value != NULL) // Value size if present
            + num_children * 3                       // Accounts for |\n-
            + num_new_lines * (indentation > 0);     // Indentation

  return num_new_lines;
}

void print_trie_recursive(trie_node_t *node, char *string, int str_idx,
                          int indentation) {
  string[++str_idx] = node->character;
  string[++str_idx] = '\n';
  if (node->children == NULL) {
    return;
  }
  linked_list_node_t *next = node->children;
  while (next != NULL) {
    // Indent
    for (int i = 0; i < indentation; i++) {
      string[++str_idx] = ' ';
    }
    string[++str_idx] = '|';
    string[++str_idx] = '\n';
    // Indent
    for (int i = 0; i < indentation; i++) {
      string[++str_idx] = ' ';
    }
    string[++str_idx] = '-';
    print_trie_recursive(next->node, string, str_idx, indentation + 1);
    next = next->next;
  }
}

char *print_trie(trie_t *trie, char c, size_t max_value_size) {
  if (trie->roots == NULL) {
    char *string = malloc(sizeof(char));
    string = "\0";
    return string;
  }

  linked_list_node_t *node = trie->roots;
  while (node != NULL && node->node->character != c) {
    node = node->next;
  }

  if (node == NULL) {
    char *string = malloc(sizeof(char));
    string = "\0";
    return string;
  }

  size_t str_size = 1;
  get_trie_string_length(node->node, &str_size, max_value_size, 0);
  printf("str size: %lu\n", str_size);
  char *string = malloc(sizeof(char) * str_size);

  print_trie_recursive(node->node, string, 0, 0);
  string[str_size] = '\0';
  return string;
}

char *generate_key(int size) {
  char *key = malloc(size + 1);
  for (int i = 0; i < size; i++) {

    char c = (rand() % (ASCII_Y - ASCII_B + 1)) + ASCII_B;
    while (c == ASCII_M) {
      c = (rand() % (ASCII_Y - ASCII_B + 1)) + ASCII_B;
    }
    key[i] = c;
  }
  key[size] = '\0';

  return key;
}

char *generate_bad_key(int size) {
  char *key = malloc(size + 1);
  for (int i = 0; i < size; i++) {
    char c = (rand() % (ASCII_Z - ASCII_A + 1)) + ASCII_A;
    key[i] = c;
  }

  char to_add;
  int which_char = rand() % 3;
  if (which_char == 0) {
    to_add = ASCII_A;
  } else if (which_char == 1) {
    to_add = ASCII_M;
  } else {
    to_add = ASCII_Z;
  }

  key[rand() % size] = to_add;
  key[size] = '\0';

  return key;
}

void test_trie(int num_items) {
  int key_size = (int)floor(log((float)num_items / 2.0) / log(23)) -
                 1; // FIX: for 100 items thos gives 0.2
  if (key_size < 1) {
    key_size = 1;
  }

  char **keys = malloc(sizeof(void *) * num_items);
  int *values = malloc(sizeof(int) * num_items);

  trie_t *trie = new_trie();

  int kv_idx = 0;
  for (int i = 0; i < num_items; i++) {
    char *key = generate_key(key_size);
    values[kv_idx] = rand();

    trie_result_t result = trie_add(trie, key, (void *)&values[kv_idx]);
    assert(result.status != TRIE_ERROR);

    if (result.status == TRIE_OK) {
      keys[kv_idx] = key;
      kv_idx++;
    } else {
      bool is_in = false;
      for (int j = 0; j < kv_idx; j++) {
        int k_idx = 0;
        while (key[k_idx] != '\0' && key[k_idx] != keys[j][k_idx]) {
          k_idx++;
        }
        if (key[k_idx] == '\0' && keys[j][k_idx] == '\0') {
          is_in = true;
          break;
        }
      }
      assert(is_in);
    }
  }
  // DEBUG
  linked_list_node_t *node = trie->roots;
  while (node != NULL) {
    // FIX: key and value size
    char *s = print_trie(trie, node->node->character, 9);
    printf("%s\n", s);
    free(s);
    node = node->next;
  }
  // END DEBUG

  for (int i = 0; i < kv_idx; i++) {
    trie_result_t result = trie_get(trie, keys[i]);
    printf("%d, %d\n", result.status, TRIE_OK);
    assert(result.status == TRIE_OK &&
           *(int *)(result.value.node->value) == values[i]);
  }

  printf("HERE6\n");
  for (int i = 0; i < 100; i++) {
    printf("HERE6.1\n");
    char *small_key = generate_key(rand() % key_size);
    printf("HERE6.2\n");
    trie_result_t result = trie_get(trie, small_key);
    printf("HERE6.3\n");
    assert(result.status == TRIE_NOT_FOUND && result.value.node == NULL);
    free(small_key);
  }
  // TODO: Add test reusing existing keys to make smaller ones

  printf("HERE7\n");
  for (int i = 0; i < 100; i++) {
    char *big_key = generate_key((rand() % 10) + 1 + key_size);
    trie_result_t result = trie_get(trie, big_key);
    assert(result.status == TRIE_NOT_FOUND && result.value.node == NULL);
    free(big_key);
  }
  // TODO: Add test reusing existing keys to make bigger ones

  printf("HERE8\n");
  for (int i = 0; i < 100; i++) {
    char *bad_key = generate_bad_key(key_size);
    trie_result_t result = trie_get(trie, bad_key);
    assert(result.status == TRIE_NOT_FOUND && result.value.node == NULL);
    free(bad_key);
  }
  // TODO: get existing keys and switch (first , last or middke)
}

void test_print() {
  int num_items = 10;
  int key_size = 4;

  char **keys = malloc(sizeof(void *) * num_items);
  int *values = malloc(sizeof(int) * num_items);

  trie_t *trie = new_trie();

  int kv_idx = 0;
  for (int i = 0; i < num_items; i++) {
    char *key = generate_key(key_size);
    values[kv_idx] = rand();

    trie_result_t result = trie_add(trie, key, (void *)&values[kv_idx]);
    assert(result.status != TRIE_ERROR);

    if (result.status == TRIE_OK) {
      keys[kv_idx] = key;
    } else {
      bool is_in = false;
      for (int j = 0; j < kv_idx; j++) {
        int k_idx = 0;
        while (key[k_idx] != '\0' && key[k_idx] != keys[j][k_idx]) {
          k_idx++;
        }
        if (key[k_idx] == '\0' && keys[j][k_idx] == '\0') {
          is_in = true;
          break;
        }
      }
      assert(is_in);
    }
    kv_idx++;
  }
  // DEBUG
  printf("Printing:\n");
  linked_list_node_t *node = trie->roots;
  while (node != NULL) {
    char *s = print_trie(trie, node->node->character, 9);
    printf("%s\n", s);
    free(s);
    node = node->next;
  }
}

int main(int agrc, char *argv[]) {
  srand(42);
  // Test empy trie
  test_print();
  // for (int i = 1; i <= NUM_TESTS; i++) {
  //   test_trie(i * 100);
  // }

  printf("✅ All tests passed!\n");
}
