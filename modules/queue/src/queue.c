#include "../include/queue.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct queue {
  size_t capacity;
  void **values;
  size_t start;
  size_t end;
  size_t size;
} queue_t;

// --- Private Functions ---
static QUEUE_STATUS reallocate_queue(queue_t *queue) {
  size_t new_capacity =
      queue->size > queue->capacity / 2 ? queue->capacity * 2 : queue->capacity;

  void **temp = malloc(new_capacity * sizeof(void *));
  if (temp == NULL) {
    return QUEUE_ERROR;
  }

  memcpy(temp, &queue->values[queue->start], queue->size * sizeof(void *));
  free(queue->values);

  queue->values = temp;
  queue->start = 0;
  queue->end = queue->size - 1;
  queue->capacity = new_capacity;
  return QUEUE_OK;
}

// --- Public Function ---
queue_t *new_queue(size_t capacity) {
  if (capacity < 1) {
    return NULL;
  }
  queue_t *queue = malloc(sizeof(queue_t));
  if (queue == NULL) {
    return NULL;
  }

  queue->values = malloc(sizeof(void *) * capacity);
  if (queue->values == NULL) {
    free(queue);
    return NULL;
  }

  queue->capacity = capacity;
  queue->start = 0;
  queue->end = 0;
  queue->size = 0;
  return queue;
}

size_t queue_size(queue_t *queue) { return queue->size; }

queue_result_t queue_peek(queue_t *queue) {
  queue_result_t result;
  if (queue == NULL) {
    result.status = QUEUE_ERROR;
    result.value = NULL;
    return result;
  }

  if (queue->size > 0) {
    result.status = QUEUE_OK;
    result.value = queue->values[queue->start];
    return result;
  }

  result.status = QUEUE_EMPTY;
  result.value = NULL;
  return result;
}

QUEUE_STATUS queue_add(queue_t *queue, void *value) {
  if (queue == NULL) {
    return QUEUE_ERROR;
  }
  if (queue->end + 1 == queue->capacity &&
      reallocate_queue(queue) == QUEUE_ERROR) {
    return QUEUE_ERROR;
  }
  queue->end++;
  queue->values[queue->end] = value;
  queue->size++;
  return QUEUE_OK;
}

queue_result_t queue_remove(queue_t *queue) {
  queue_result_t result;
  if (queue == NULL) {
    result.status = QUEUE_ERROR;
    result.value = NULL;
    return result;
  }
  if (queue->size > 0) {
    result.status = QUEUE_OK;
    result.value = queue->values[queue->start];
    queue->start++;
    queue->size--;
    return result;
  }
  result.status = QUEUE_EMPTY;
  result.value = NULL;
  return result;
}

void queue_destroy(queue_t *queue, void (*destroy_value)(void *)) {
  if (queue == NULL) {
    return;
  }
  if (destroy_value != NULL) {
    for (int i = queue->start; i <= queue->end; i++) {
      destroy_value(queue->values[i]);
    }
  }

  free(queue->values);
  free(queue);
}
