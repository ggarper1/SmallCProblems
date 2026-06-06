#include "../include/queue.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Queue {
  size_t capacity;
  void **values;
  int start;
  int end;
} Queue_t;

// --- Private Functions ---
Q_STATUS reallocateQueue(Queue_t *queue) {
  int length = qLength(queue);
  if (length > queue->capacity / 2) {
    queue->capacity *= 2;
  }
  void **temp = malloc(queue->capacity * sizeof(void *));
  if (temp == NULL) {
    return Q_ERROR;
  }
  memcpy(temp, &queue->values[queue->start], length * sizeof(void *));
  free(queue->values);
  queue->values = temp;
  queue->start = 0;
  queue->end = queue->start + length - 1;
  return Q_OK;
}

// --- Public Function ---
Queue_t *newQueue(size_t capacity) {
  if (capacity < 1) {
    return NULL;
  }
  Queue_t *queue = malloc(sizeof(Queue_t));
  if (queue == NULL) {
    return NULL;
  }
  queue->capacity = capacity;
  queue->start = 0;
  queue->end = -1;
  queue->values = malloc(sizeof(void *) * capacity);
  if (queue->values == NULL) {
    free(queue);
    return NULL;
  }

  return queue;
}

int qLength(Queue_t *queue) { return queue->end - queue->start + 1; }

void *qPeek(Queue_t *queue) {
  if (queue->start <= queue->end) {
    return queue->values[queue->start];
  }
  return NULL;
}

Q_STATUS qAdd(Queue_t *queue, void *value) {
  if (queue->end + 1 == queue->capacity) {
    if (reallocateQueue(queue) == Q_ERROR) {
      return Q_ERROR;
    }
  }
  queue->end++;
  queue->values[queue->end] = value;
  return Q_OK;
}

void *qRemove(Queue_t *queue) {
  if (queue->start <= queue->end) {
    void *ptr = queue->values[queue->start];
    queue->start++;
    return ptr;
  }
  return NULL;
}

void qDestroy(Queue_t *queue) {
  free(queue->values);
  free(queue);
}

void qDestroyAll(Queue_t *queue) {
  for (int i = queue->start; i <= queue->end; i++) {
    free(queue->values[i]);
  }
  free(queue->values);
  free(queue);
}

void printQueue(Queue_t *queue,
                void (*repr)(void *value, char *buffer, int bufferSize),
                int bufferSize) {
  if (queue->start > queue->end) {
    printf("<- []\n");
    return;
  }

  char *buffer = malloc(sizeof(char) * bufferSize);
  repr(queue->values[queue->start], buffer, bufferSize);
  printf("<- [%s", buffer);
  for (int i = queue->start + 1; i <= queue->end; i++) {
    repr(queue->values[i], buffer, bufferSize);
    printf(", %s", buffer);
  }
  printf("]\n");
  free(buffer);
}
