#include "../include/linkedList.h"
#include "../include/random.h"
#include <stdio.h>
#include <stdlib.h>

const int numTests = 1000;
const int max = 1000;
const int min = -1000;

void repr(void *value, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *((int *)value));
}

int testLinkedList(int size) {
  int **items = malloc(sizeof(int *) * size);
  LinkedList_t *list = newLinkedList();
  for (int i = 0; i < size; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(min, max);
    items[i] = item;
    if (i % 2 == 0) {
      LL_STATUS status = llAddFirst(list, item);
      if (status == LL_ERROR) {
        printf("🚨 Error during adding\n");
        return 0;
      }
      if (llPeekFirst(list) != item) {
        printf("🚨 Either peekFirst or addFirst is not working!\n");
        return 0;
      }
    } else {
      LL_STATUS status = llAddLast(list, item);
      if (status == LL_ERROR) {
        printf("🚨 Error during adding\n");
        return 0;
      }
      if (llPeekLast(list) != item) {
        printf("🚨 Either peekLast or addLast is not working!\n");
        return 0;
      }
    }
  }

  if (llLength(list) != size) {
    printf("🚨 Wrong length\n");
    return 0;
  }
  if (llLength(list) < 20) {
    printLinkedList(list, repr, 6);
  }

  if (size % 2 == 0) {
    for (int i = size - 1; i > -1; i--) {
      if (i % 2 == 0) {
        int *item = llPopFirst(list);
        if (*item != *(items[i])) {
          printf("🚨 popFirst is not working! Poped %d when should have popped "
                 "%d\n",
                 *item, *(items[i]));
          return 0;
        }
        free(item);
      } else {
        int *item = llPopLast(list);
        if (*item != *(items[i])) {
          printf("🚨 popLAst is not working!\n");
          return 0;
        }
        free(item);
      }
    }

    if (llPeekFirst(list) != NULL) {
      printf("🚨 Peek first in empty list returned a value different from "
             "null.\n");
      return 0;
    }
    if (llPeekLast(list) != NULL) {
      printf(
          "🚨 Peek last in empty list returned a value different from null.\n");
      return 0;
    }
    if (llPopFirst(list) != NULL) {
      printf(
          "🚨 Pop first in empty list returned a value different from null.\n");
      return 0;
    }
    if (llPopLast(list) != NULL) {
      printf(
          "🚨 Pop last in empty list returned a value different from null.\n");
      return 0;
    }
  } else {
    for (int i = size - 1; i > 5; i--) {
      if (i % 2 == 0) {
        int *item = llPopFirst(list);
        if (*item != *(items[i])) {
          printf("🚨 popFirst is not working! Poped %d when should have popped "
                 "%d\n",
                 *item, *(items[i]));
          return 0;
        }
        free(item);
      } else {
        int *item = llPopLast(list);
        if (*item != *(items[i])) {
          printf("🚨 popLAst is not working!\n");
          return 0;
        }
        free(item);
      }
    }
  }

  if (size % 2 == 0) {
    llDestroy(list);
  } else {
    llDestroyAll(list);
  }
  free(items);
  return 1;
}

int main(int argc, char *argv[]) {
  for (int i = 1; i <= numTests; i++) {
    if (!testLinkedList(i * 10 + ((i % 2) == 1))) {
      printf("🚨 Test failed!\n");
      return 1;
    }
  }
  printf("✅ All tests passed!\n");
  return 1;
}
