#include "../include/random.h"
#include "../include/stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int numTests = 10000;

bool testPushPeek(Stack_t *stack, int size, int ***items) {
  *items = malloc(sizeof(int *) * size);
  if (sPeek(stack) != NULL) {
    printf("🚨 Peek should be null when stack is empty.\n");
    return false;
  }
  for (int i = 0; i < size; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(0, 99);
    (*items)[i] = item;
    sPush(stack, item);
    int *peeked = sPeek(stack);
    if (*(int *)peeked != *(*items)[i]) {
      printf("🚨 Peek should be %d, instead is %d.\n", *peeked, **items[i]);
      return false;
    }
  }
  return true;
}

bool testPopPeek(Stack_t *stack, int **items, int size) {
  for (int i = 1; i <= size; i++) {
    int *peeked = sPeek(stack);
    int *removed = sPop(stack);

    if (*peeked != *items[size - i]) {
      printf("🚨 Peek should be %d, instead is %d.\n", *items[i], *peeked);
    }
    if (*removed != *items[size - i]) {
      printf("🚨 Peek should be %d, instead is %d.\n", *items[i], *removed);
    }
    free(removed);
  }
  return true;
}

bool testDelete(Stack_t *stack) {
  int **items = malloc(sizeof(int *) * 20);
  for (int i = 0; i < 20; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(0, 99);
    items[i] = item;
    sPush(stack, item);
  }

  sDestroy(stack);
  for (int i = 0; i < 20; i++) {
    free(items[i]);
  }
  free(items);

  return true;
}

bool testDeleteAll(Stack_t *stack) {
  sDestroyAll(stack);
  return true;
}

void testStack() {
  for (int i = 0; i < numTests; i++) {
    Stack_t *stack = newStack(2);
    if (true) {
      int **items;
      if (!testPushPeek(stack, i * 10, &items)) {
        printf("🚨 Push test failed\n");
        return;
      }
      if (!testPopPeek(stack, items, i * 10)) {
        printf("🚨 Pop test failed\n");
        return;
      }
      testDeleteAll(stack);
      free(items);
    } else {
      testDelete(stack);
    }
  }
  printf("✅ All tests passed!\n");
}

int main(int argc, char *argv[]) {
  testStack();
  return 0;
}
