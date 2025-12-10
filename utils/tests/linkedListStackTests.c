#include "../include/linkedListStack.h"
#include "../include/random.h"
#include <stdio.h>
#include <stdlib.h>

const int numTests = 1000;

int testPushPeek(LLStack_t *stack, int size, int ***items) {
  *items = malloc(sizeof(int *) * size);
  if (llsPeek(stack) != NULL) {
    printf("🚨 Peek should be null when stack is empty.\n");
    return 0;
  }
  for (int i = 0; i < size; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(0, 99);
    (*items)[i] = item;
    int status = llsPush(stack, item);
    if (stack == LLS_ERROR) {
      printf("🚨 Error when pushing\n");
      return 0;
    }
    int *peeked = llsPeek(stack);
    if (*(int *)peeked != *(*items)[i]) {
      printf("🚨 Peek should be %d, instead is %d.\n", *peeked, **items[i]);
      return 0;
    }
  }
  return 1;
}

int testPopPeek(LLStack_t *stack, int **items, int size) {
  for (int i = 1; i <= size; i++) {
    int *peeked = llsPeek(stack);
    int *removed = llsPop(stack);

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

int testDelete(LLStack_t *stack) {
  int **items = malloc(sizeof(int *) * 20);
  for (int i = 0; i < 20; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(0, 99);
    items[i] = item;
    llsPush(stack, item);
  }

  llsDestroy(stack);
  for (int i = 0; i < 20; i++) {
    free(items[i]);
  }
  free(items);

  return 1;
}

int testDeleteAll(LLStack_t *stack) {
  llsDestroyAll(stack);
  return 1;
}

void testStack() {
  for (int i = 0; i < numTests; i++) {
    LLStack_t *stack = newLLStack();
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
