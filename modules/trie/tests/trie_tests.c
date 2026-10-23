#include "../include/trie.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TESTS 100
#define NUM_PROBES 100

// Keys are built only from VALID_CHARS, so any key containing one of
// INVALID_CHARS is guaranteed not to be in the trie
static const char VALID_CHARS[] = "bcdefghijklnopqrstuvwxy";
static const char INVALID_CHARS[] = "amz";
#define NUM_VALID_CHARS ((int)sizeof(VALID_CHARS) - 1)
#define NUM_INVALID_CHARS ((int)sizeof(INVALID_CHARS) - 1)

// ---------------------------------------------------------------------------
// Key helpers
// ---------------------------------------------------------------------------

static int rand_between(int min, int max) {
  return min + rand() % (max - min + 1);
}

static char random_valid_char(void) {
  return VALID_CHARS[rand() % NUM_VALID_CHARS];
}

static char *random_key(int size) {
  char *key = malloc(size + 1);
  for (int i = 0; i < size; i++) {
    key[i] = random_valid_char();
  }
  key[size] = '\0';
  return key;
}

// A random key with one char that is never inserted
static char *random_invalid_key(int size) {
  char *key = random_key(size);
  key[rand() % size] = INVALID_CHARS[rand() % NUM_INVALID_CHARS];
  return key;
}

static char *concat(const char *a, const char *b) {
  char *s = malloc(strlen(a) + strlen(b) + 1);
  strcpy(s, a);
  strcat(s, b);
  return s;
}

static bool contains_key(char **keys, int num_keys, const char *key) {
  for (int i = 0; i < num_keys; i++) {
    if (strcmp(keys[i], key) == 0) {
      return true;
    }
  }
  return false;
}

// ---------------------------------------------------------------------------
// Assertions
// ---------------------------------------------------------------------------

static void assert_not_found(trie_t *trie, const char *key) {
  trie_result_t result = trie_get(trie, key);
  assert(result.status == TRIE_NOT_FOUND && result.value.node == NULL);
}

// Takes ownership of `key`, so generated keys can be passed inline
static void assert_not_found_and_free(trie_t *trie, char *key) {
  assert_not_found(trie, key);
  free(key);
}

static void assert_value(trie_t *trie, const char *key, void *value) {
  trie_result_t result = trie_get(trie, key);
  assert(result.status == TRIE_OK && result.value.node->value == value);
}

static void assert_removed(trie_t *trie, const char *key, void *value) {
  trie_result_t result = trie_remove(trie, key);
  assert(result.status == TRIE_OK && result.value.value == value);
  assert_not_found(trie, key);
  assert(trie_remove(trie, key).status == TRIE_NOT_FOUND); // Only once
}

static void assert_empty(trie_t *trie) {
  assert(trie_size(trie) == 0 && trie->roots == NULL);
}

// ---------------------------------------------------------------------------
// Printing
// ---------------------------------------------------------------------------

// The '\0' child marking that a key ends at `node`, if any (always first)
static trie_node_t *key_end(const trie_node_t *node) {
  linked_list_node_t *first = node->children;
  return (first && first->node->character == '\0') ? first->node : NULL;
}

// Prints one node per line as a tree:
//   c
//   `-- a = 0
//       |-- r = 1
//       `-- t = 2
static void print_node(FILE *out, const trie_node_t *node, const char *indent,
                       const char *connector) {
  fprintf(out, "%s%s%c", indent, connector, node->character);
  trie_node_t *end = key_end(node);
  if (end != NULL) {
    fprintf(out, " = %d", *(int *)end->value);
  }
  fputc('\n', out);

  // Below this node, its connector turns into a column (or blank if last)
  const char *column = connector[0] == '\0'  ? ""
                       : connector[0] == '`' ? "    "
                                             : "|   ";
  char *child_indent = concat(indent, column);
  for (linked_list_node_t *c = node->children; c != NULL; c = c->next) {
    if (c->node->character != '\0') {
      print_node(out, c->node, child_indent, c->next ? "|-- " : "`-- ");
    }
  }
  free(child_indent);
}

// Returns the subtree under the root `c` as a string the caller must free
static char *print_trie(trie_t *trie, char c) {
  char *string;
  size_t length;
  FILE *out = open_memstream(&string, &length);

  for (linked_list_node_t *r = trie->roots; r != NULL; r = r->next) {
    if (r->node->character == c) {
      print_node(out, r->node, "", "");
    }
  }

  fclose(out);
  return string;
}

// ---------------------------------------------------------------------------
// Fixed-key tests
// ---------------------------------------------------------------------------

static void test_empty_trie(void) {
  trie_t *trie = new_trie();
  assert_empty(trie);
  assert_not_found(trie, "a");
  assert(trie_remove(trie, "a").status == TRIE_NOT_FOUND);

  char *s = print_trie(trie, 'a');
  assert(strcmp(s, "") == 0);
  free(s);

  trie_destroy(trie, NULL);
}

static void test_add_duplicate(void) {
  int value = 1, other = 2;
  trie_t *trie = new_trie();

  assert(trie_add(trie, "key", &value).status == TRIE_OK);
  assert(trie_add(trie, "key", &other).status == TRIE_ALREADY_EXISTS);
  assert(trie_add(trie, "key", &value).status == TRIE_ALREADY_EXISTS);
  assert(trie_size(trie) == 1);
  assert_value(trie, "key", &value);

  trie_destroy(trie, NULL);
}

static void test_print(void) {
  char *keys[] = {"cat", "ca", "do", "car"};
  int values[] = {2, 0, 3, 1};
  trie_t *trie = new_trie();
  for (int i = 0; i < 4; i++) {
    assert(trie_add(trie, keys[i], &values[i]).status == TRIE_OK);
  }

  char *s = print_trie(trie, 'c');
  assert(strcmp(s, "c\n"
                   "`-- a = 0\n"
                   "    |-- r = 1\n"
                   "    `-- t = 2\n") == 0);
  free(s);

  trie_destroy(trie, NULL);
}

// Keys that share prefixes or are prefixes of each other
static void test_remove_shared_prefixes(void) {
  char *keys[] = {"ca", "car", "cart", "cat", "do"};
  int values[] = {0, 1, 2, 3, 4};
  trie_t *trie = new_trie();
  for (int i = 0; i < 5; i++) {
    assert(trie_add(trie, keys[i], &values[i]).status == TRIE_OK);
  }

  assert(trie_remove(trie, "c").status == TRIE_NOT_FOUND);     // Only a prefix
  assert(trie_remove(trie, "carts").status == TRIE_NOT_FOUND); // Too long
  assert(trie_remove(trie, "dog").status == TRIE_NOT_FOUND);

  assert_removed(trie, "car", &values[1]); // Middle of a path: "cart" stays
  assert_value(trie, "cart", &values[2]);
  assert_value(trie, "ca", &values[0]);

  assert_removed(trie, "cart", &values[2]); // Leaf: "car" branch is pruned
  assert_not_found(trie, "car");
  assert_value(trie, "ca", &values[0]);
  assert_value(trie, "cat", &values[3]);

  assert_removed(trie, "ca", &values[0]); // Prefix of "cat"
  assert_value(trie, "cat", &values[3]);

  assert_removed(trie, "do", &values[4]); // Whole root branch
  assert(trie->roots != NULL && trie->roots->node->character == 'c' &&
         trie->roots->next == NULL);

  assert_removed(trie, "cat", &values[3]);
  assert_empty(trie);

  trie_destroy(trie, NULL);
}

// ---------------------------------------------------------------------------
// Random-key tests
// ---------------------------------------------------------------------------

typedef struct {
  trie_t *trie;
  char **keys;  // Only the keys that were actually inserted
  int **values; // values[i] belongs to keys[i]
  int num_keys;
  int key_size; // All keys have this length
} fixture_t;

// Smallest size whose key space is ~NUM_VALID_CHARS times bigger than
// `num_items`, so there are some duplicates but not many
static int key_size_for(int num_items) {
  int key_size = 1;
  for (long space = NUM_VALID_CHARS; space < (long)num_items * NUM_VALID_CHARS;
       space *= NUM_VALID_CHARS) {
    key_size++;
  }
  return key_size;
}

static fixture_t build_random_trie(int num_items) {
  fixture_t f = {
      .trie = new_trie(),
      .keys = malloc(sizeof(char *) * num_items),
      .values = malloc(sizeof(int *) * num_items),
      .num_keys = 0,
      .key_size = key_size_for(num_items),
  };

  for (int i = 0; i < num_items; i++) {
    char *key = random_key(f.key_size);
    int *value = malloc(sizeof(int));
    *value = rand();

    trie_result_t result = trie_add(f.trie, key, value);
    if (result.status == TRIE_OK) {
      f.keys[f.num_keys] = key;
      f.values[f.num_keys] = value;
      f.num_keys++;
    } else {
      assert(result.status == TRIE_ALREADY_EXISTS &&
             contains_key(f.keys, f.num_keys, key));
      free(key);
      free(value);
    }
  }

  assert(trie_size(f.trie) == (size_t)f.num_keys);
  return f;
}

static void free_fixture(fixture_t *f) {
  trie_destroy(f->trie, free); // The trie owns the values
  for (int i = 0; i < f->num_keys; i++) {
    free(f->keys[i]);
  }
  free(f->keys);
  free(f->values);
}

static const char *random_stored_key(const fixture_t *f) {
  return f->keys[rand() % f->num_keys];
}

static void assert_all_present(const fixture_t *f) {
  for (int i = 0; i < f->num_keys; i++) {
    assert_value(f->trie, f->keys[i], f->values[i]);
  }
}

static void test_get_missing(const fixture_t *f) {
  trie_t *trie = f->trie;
  int size = f->key_size;

  for (int i = 0; i < NUM_PROBES; i++) {
    // Shorter than any stored key
    assert_not_found_and_free(trie, random_key(rand_between(1, size - 1)));

    // Proper prefix of a stored key: the path exists but no key ends there
    assert_not_found_and_free(
        trie, strndup(random_stored_key(f), rand_between(1, size - 1)));

    // Longer than any stored key
    assert_not_found_and_free(trie, random_key(size + rand_between(1, 10)));

    // Stored key plus extra chars: the whole key matches, then runs past
    char *suffix = random_key(rand_between(1, 10));
    assert_not_found_and_free(trie, concat(random_stored_key(f), suffix));
    free(suffix);

    // Contains a char that is never inserted
    assert_not_found_and_free(trie, random_invalid_key(size));
  }
}

// Stored keys with the first, last or middle char changed. The result may be
// another stored key, so the expected outcome is checked against the list.
static void test_get_mutated(const fixture_t *f) {
  int positions[] = {0, f->key_size - 1, f->key_size / 2};

  for (int i = 0; i < NUM_PROBES; i++) {
    char *key = strdup(random_stored_key(f));
    int pos = positions[i % 3];
    char original = key[pos];
    while (key[pos] == original) {
      key[pos] = random_valid_char();
    }

    TRIE_STATUS expected =
        contains_key(f->keys, f->num_keys, key) ? TRIE_OK : TRIE_NOT_FOUND;
    assert(trie_get(f->trie, key).status == expected);
    free(key);
  }
}

static void test_remove(const fixture_t *f) {
  trie_t *trie = f->trie;

  // Remove every other key
  for (int i = 0; i < f->num_keys; i += 2) {
    assert_removed(trie, f->keys[i], f->values[i]);
  }
  assert(trie_size(trie) == (size_t)(f->num_keys / 2));

  // The rest are still there, and removing their prefixes (never keys, since
  // all keys have the same length) fails without touching them
  for (int i = 1; i < f->num_keys; i += 2) {
    char *prefix = strndup(f->keys[i], f->key_size - 1);
    assert(trie_remove(trie, prefix).status == TRIE_NOT_FOUND);
    free(prefix);
    assert_value(trie, f->keys[i], f->values[i]);
  }

  // Remove the rest: every branch must be pruned
  for (int i = 1; i < f->num_keys; i += 2) {
    assert_removed(trie, f->keys[i], f->values[i]);
  }
  assert_empty(trie);

  // The emptied trie is still usable
  for (int i = 0; i < f->num_keys; i++) {
    assert(trie_add(trie, f->keys[i], f->values[i]).status == TRIE_OK);
  }
  assert_all_present(f);
}

static void test_random_trie(int num_items) {
  fixture_t f = build_random_trie(num_items);

  assert_all_present(&f);
  test_get_missing(&f);
  test_get_mutated(&f);
  test_remove(&f);

  free_fixture(&f);
}

int main(void) {
  srand(42);

  test_empty_trie();
  test_add_duplicate();
  test_print();
  test_remove_shared_prefixes();
  for (int i = 1; i <= NUM_TESTS; i++) {
    test_random_trie(i * 100);
  }

  printf("✅ All tests passed!\n");
  return 0;
}
