#include <stddef.h>

#ifndef QUEUE_H
#define QUEUE_H

typedef enum { Q_OK, Q_ERROR } Q_STATUS;

typedef struct Queue Queue_t;

// --- Public Function Prototypes ---

/**
 * Creates a new Queue.
 * @param capacity Initial number of items.
 * @return A pointer to the new Queue, or NULL on failure.
 */
Queue_t *newQueue(size_t capacity);

/**
 * Returns a Queue's length (number of values in it).
 * @param queue The Queue.
 * @return the number of items in it.
 */

int qLength(Queue_t *queue);
/**
 * Peeks the front element of a Queue.
 * @param queue The queue.
 * @return A pointer to the element, NULL if the queue is empty.
 */
void *qPeek(Queue_t *queue);

/**
 * Adds a value in a Queue.
 * Pointers to the item are stored (caller manages memory).
 * @param queue The Queue.
 * @param value The value to add.
 */
Q_STATUS qAdd(Queue_t *queue, void *value);

/**
 * Removes the front element of a Queue.
 * @param queue The Queue.
 * @return NULL if the Queue is empty, the pointer to the value if it
 */
void *qRemove(Queue_t *queue);

/**
 * Destroys the Queue and frees all allocated memory.
 * @param queue The Queue to destroy.
 */
void qDestroy(Queue_t *queue);

/**
 * Destroys the Queue and along with all the values in it.
 * After calling this method, accessing a element that had
 * been stored in the list can lead to erros due to it being
 * freed.
 * @param queue The Queue to destroy.
 */
void qDestroyAll(Queue_t *queue);

/**
 * Prints the Queue.
 * @param queue The Queue to print.
 */
void printQueue(Queue_t *queue,
                void (*repr)(void *value, char *buffer, int bufferSize),
                int bufferSize);

#endif
