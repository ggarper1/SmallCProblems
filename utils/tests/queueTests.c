#include "../include/queue.h"
#include "../include/random.h"
#include <stdio.h>
#include <stdlib.h>

const int numTests = 1000;

void repr(void *value, char *buffer, int bufferSize) {
  snprintf(buffer, bufferSize, "%d", *((int *)value));
}

int testPeekAddRemoveDestroyAll(Queue_t *queue, int size) {
  int **items = malloc(size * sizeof(int *));

  for (int i = 0; i < size; i++) {
    if (qLength(queue) != i) {
      printf("🚨 Incorrect length\n");
      return 0;
    }
    int *item = malloc(sizeof(int));
    *item = randInt(0, 99);
    items[i] = item;
    if (qAdd(queue, item) == Q_ERROR) {
      printf("🚨 Error when adding.\n");
      return 0;
    }
  }

  for (int i = 0; i < size; i++) {
    if (qLength(queue) != size - i) {
      printf("🚨 Incorrect length\n");
      return 0;
    }
    int *peeked = qPeek(queue);
    int *removed = qRemove(queue);
    if (*peeked != *items[i]) {
      printf("🚨 Peeked is incorrect");
      return 0;
    }
    if (*removed != *items[i]) {
      printf("🚨 Peeked is incorrect");
      return 0;
    }
    free(removed);
  }

  if (qLength(queue) != 0) {
    printf("🚨 Length should be 0\n");
    return 0;
  }

  int *peeked = qPeek(queue);
  if (peeked != NULL) {
    printf("🚨 Peeked should be null");
    return 0;
  }
  int *removed = qRemove(queue);
  if (removed != NULL) {
    printf("🚨 Peeked should be null");
    return 0;
  }

  for (int i = 0; i < size / 10; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(0, 99);
    items[i] = item;
    if (qAdd(queue, item) == Q_ERROR) {
      printf("🚨 Error when adding.\n");
      return 0;
    }
  }

  qDestroyAll(queue);

  free(items);
  return 1;
}

int testDestroy(Queue_t *queue) {
  int **items = malloc(sizeof(int *) * 50);
  for (int i = 0; i < 50; i++) {
    int *item = malloc(sizeof(int));
    *item = randInt(0, 99);
    if (qAdd(queue, item) == Q_ERROR) {
      printf("🚨 Error when adding.\n");
      return 0;
    }

    items[i] = item;
  }

  qDestroy(queue);
  for (int i = 0; i < 50; i++) {
    free(items[i]);
  }
  free(items);
  return 1;
}

void testQueue() {
  for (int i = 1; i <= numTests; i++) {
    int size = i * 10;
    Queue_t *queue = newQueue(2);
    if (queue == NULL) {
      printf("🚨 Error creating tree\n");
      return;
    }

    if (i % 10 != 0) {
      if (!testPeekAddRemoveDestroyAll(queue, size * 2)) {
        printf("🚨 Failed Peek Add Remove Test!\n");
        return;
      }
    } else {
      testDestroy(queue);
    }
  }
  printf("✅ All tests passed!\n");
}

int main(int argc, char *argv[]) {
  testQueue();
  return 0;
}
