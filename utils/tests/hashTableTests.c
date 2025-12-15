#include "../include/hashTable.h"
#include "../include/random.h"
#include <ranlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int numTests = 1000;
const int maxKeySize = 16;
const int minValue = 0;
const int maxValue = 10000;

unsigned long hash(void *key) {
  unsigned long sum = 0;
  sum ^= *(unsigned long *)key;
  sum ^= *(unsigned long *)(key + 8);
  return sum;
}

int compareKey(const void *key1, const void *key2) {
  return strcmp(key1, key2);
}

void generateRandomString(char **s) {
  *s = malloc(sizeof(char) * maxKeySize);
  if (*s == NULL) {
    printf("🚨 Error generating key\n");
    return;
  }
  for (int i = 0; i < maxKeySize - 1; i++) {
    (*s)[i] = (char)randInt(97, 122);
  }
  (*s)[maxKeySize - 1] = '\0';
}

void reprKey(void *key, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%s", (char *)key);
}

void reprValue(void *value, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *(int *)value);
}

HashTable_t *createHashTable() {
  HashTable_t *hashTable = newHashTable(5, compareKey, hash);
  return hashTable;
}

void testHashTable() {
  for (int j = 0; j < numTests; j++) {
    int insertions = j * 10;
    char **keys = malloc(sizeof(char *) * insertions);
    int **values = malloc(sizeof(int *) * insertions);
    HashTable_t *hashTable = createHashTable();
    for (int i = 0; i < insertions; i++) {
      char *key;
      generateRandomString(&key);
      int *value = malloc(sizeof(int));
      *value = randInt(minValue, maxValue);
      keys[i] = key;
      values[i] = value;
      HT_STATUS status = htPut(hashTable, key, value);
      if (status != HT_OK) {
        printf("🚨 Error\n");
        return;
      }
    }

    for (int i = 0; i < insertions; i++) {
      char *key = keys[i];
      int *value = values[i];

      HT_STATUS status = htPut(hashTable, key, value);
      if (status != HT_DUPLICATE) {
        printf("🚨 Error: status is: %d\n", status);
        return;
      }
    }

    for (int i = 0; i < insertions; i++) {
      char *key = keys[i];
      int *value = values[i];

      void *val;
      HT_STATUS status = htGet(hashTable, key, &val);
      if (status != HT_OK) {
        printf("🚨 Error: status is: %d\n", status);
        return;
      } else if (val != value) {
        printf("🚨 Error: val is not equal to value\n");
        return;
      }
    }

    for (int i = 0; i < insertions - 5; i++) {
      char *key = keys[i];
      int *value = values[i];

      HT_STATUS status = htRemove(hashTable, key);
      if (status != HT_OK) {
        printf("🚨 Error:(removals) status is: %d\n", status);
        return;
      }
    }

    for (int i = 0; i < insertions - 5; i++) {
      char *key = keys[i];
      int *value = values[i];

      void *val = NULL;
      HT_STATUS status = htGet(hashTable, key, &val);
      if (status != HT_NOT_FOUND) {
        printf("🚨 Error: status is: %d\n", status);
        return;
      } else if (val != NULL) {
        printf("🚨 Error: val is not equal to value\n");
        return;
      }
    }

    for (int i = 0; i < insertions - 5; i++) {
      char *key = keys[i];
      int *value = values[i];

      HT_STATUS status = htRemove(hashTable, key);
      if (status != HT_NOT_FOUND) {
        printf("🚨 Error:(removals) status is: %d\n", status);
        return;
      }
    }
    if (insertions < 40) {
      printHashTable(hashTable, reprKey, maxKeySize, reprValue, 6);
    }

    if (j % 2 == 0) {
      htDestroyAll(hashTable);
      free(keys);
      free(values);
    } else {
      htDestroy(hashTable);
      for (int i = 0; i < insertions; i++) {
        free(keys[i]);
        free(values[i]);
      }
      free(keys);
      free(values);
    }
  }
  printf("✅ All tests passed\n");
}

int main(int agrc, char *argv[]) {
  srand(time(NULL));
  testHashTable();
}
