#include "../include/boolStack.h"
#include "../include/random.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int numTests = 10000;

bool testPushPeek(BoolStack_t *stack, int size, int **items) {
  *items = malloc(sizeof(int) * size);
  for (int i = 0; i < size; i++) {
    int item = randInt(0, 1);
    bsPush(stack, item);
    int peeked = bsPeek(stack);
    (*items)[i] = item;
    if (peeked != (*items)[i]) {
      printf("🚨 Peek should be %d, instead is %d.\n", (*items)[i], peeked);
      return false;
    }
  }
  return true;
}

bool testPopPeek(BoolStack_t *stack, int *items, int size) {
  for (int i = 1; i <= size; i++) {
    int peeked = bsPeek(stack);
    int removed = bsPop(stack);

    if (peeked != items[size - i]) {
      printf("🚨 Peek should be %d, instead is %d.\n", items[size - i], peeked);
      return false;
    }
    if (removed != items[size - i]) {
      printf("🚨 Removed should be %d, instead is %d.\n", items[size - i],
             removed);
      return false;
    }
  }
  return true;
}

void testStack() {
  for (int i = 0; i < numTests; i++) {
    BoolStack_t *stack = newBoolStack(2);
    int *items;
    if (!testPushPeek(stack, i * 10, &items)) {
      printf("🚨 Push test failed\n");
      return;
    }

    if (!testPopPeek(stack, items, i * 10)) {
      printf("🚨 Pop test failed\n");
      return;
    }
    free(items);
    bsDestroy(stack);
  }
  printf("✅ All tests passed!\n");
}

int main(int argc, char *argv[]) {
  testStack();
  return 0;
}
