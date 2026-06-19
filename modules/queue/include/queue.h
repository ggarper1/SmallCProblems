#include <stddef.h>

#ifndef QUEUE_H
#define QUEUE_H

typedef enum { QUEUE_OK, QUEUE_ERROR, QUEUE_EMPTY } QUEUE_STATUS;

typedef struct queue queue_t;

typedef struct queue_result {
  QUEUE_STATUS status;
  void *value;
} queue_result_t;

// --- Public Function Prototypes ---

/**
 * Creates a new Queue.
 * @param capacity Initial number of items.
 * @return A pointer to the new Queue, or NULL on failure.
 */
queue_t *new_queue(size_t capacity);

/**
 * Returns a Queue's length (number of values in it).
 * @param queue The Queue.
 * @return the number of items in it.
 */

size_t queue_size(queue_t *queue);
/**
 * Peeks the front element of a Queue.
 * @param queue The queue.
 * @return A pointer to the element, NULL if the queue is empty.
 */
queue_result_t queue_peek(queue_t *queue);

/**
 * Adds a value in a Queue.
 * Pointers to the item are stored (caller manages memory).
 * @param queue The Queue.
 * @return queue result containing a status (to indicate if the queue is
 * empty) and the value or null if it's empty.
 */
QUEUE_STATUS queue_add(queue_t *queue, void *value);

/**
 * Removes the front element of a Queue.
 * @param queue The Queue.
 * @return queue result containing a status (to indicate if the queue is empty)
 * and the value or null if it's empty.
 */
queue_result_t queue_remove(queue_t *queue);

/**
 * Destroys the Queue and frees all allocated memory.
 * @param queue The Queue to destroy.
 * @param destroy_value the function with which to destroy values with or NULL.
 */
void queue_destroy(queue_t *queue, void (*destroy_value)(void *));

#endif
