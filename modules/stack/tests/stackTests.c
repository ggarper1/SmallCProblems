#include "../include/random.h"
#include "../include/stack.h"
#include <stdio.h>
#include <stdlib.h>

const int numTests = 10000;

int testPushPeek(Stack_t *stack, int size, int ***items) {
  *items = malloc(sizeof(int *) * size);
  if (sPeek(stack) != NULL) {
    printf("🚨 Peek should be null when stack is empty.\n");
    return 0;
  }
  for (int i = 0; i < size; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(0, 99);
    (*items)[i] = item;
    int status = sPush(stack, item);
    if (stack == S_ERROR) {
      printf("🚨 Error when pushing\n");
      return 0;
    }
    int *peeked = sPeek(stack);
    if (*(int *)peeked != *(*items)[i]) {
      printf("🚨 Peek should be %d, instead is %d.\n", *peeked, **items[i]);
      return 0;
    }
  }
  return 1;
}

int testPopPeek(Stack_t *stack, int **items, int size) {
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
  return 1;
}

int testDelete(Stack_t *stack) {
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

  return 1;
}

int testDeleteAll(Stack_t *stack) {
  sDestroyAll(stack);
  return 1;
}

void testStack() {
  for (int i = 0; i < numTests; i++) {
    Stack_t *stack = newStack(2);
    if (1) {
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
