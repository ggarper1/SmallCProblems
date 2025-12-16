#include <stddef.h>

#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef enum { HT_OK, HT_ERROR, HT_NOT_FOUND, HT_DUPLICATE } HT_STATUS;

typedef struct HashTable HashTable_t;

// --- Public Function Prototypes ---

/**
 * Creates a hash table.
 * @param capacity Initial number of items.
 * @return A pointer to the new hash table, or NULL on failure.
 */
HashTable_t *newHashTable(size_t capacity,
                          int (*compare)(const void *key1, const void *key2),
                          unsigned long (*hash)(void *key));

/**
 * Returns a hash table's length (number of values in it).
 * @param hashTable The hash table.
 * @return the number of items in it.
 */
int htLength(HashTable_t *hashTable);

/**
 * Gets a the valu associated to the key in a hash table.
 * @param hashTable The hash table.
 * @param key The key.
 * @param where the value will be returned
 * @return the status of the operation (HT_OK or HT_NOT_FOUND).
 */
HT_STATUS htGet(HashTable_t *hashTable, void *key, void **value);

/**
 * Puts a key valeu pair to a hash table, if the value is already,
 * it doesn't modify it.
 * @param hashTable The hash table.
 * @param key The key.
 * @param value The value.
 * @return the operation's status (HT_OK, HT_DUPLICATE)
 */
HT_STATUS htPut(HashTable_t *hashTable, void *key, void *value);

/**
 * Removes a key value pair from a hash table.
 * @param hashTable The hash table.
 * @param key The key.
 * @return the operation's status.
 */
HT_STATUS htRemove(HashTable_t *hashTable, void *key);

/**
 * Destroys the hash table and along with all the keys and values in it.
 * After calling this method, accessing a element that had
 * been stored in the list can lead to erros due to it being
 * freed.
 * @param hashTable The hash table to destroy.
 */
void htDestroyAll(HashTable_t *hashTable);

/**
 * Prints the hash table.
 * @param hashTable The hash table to print.
 */
void printHashTable(HashTable_t *hashTable,
                    void (*reprKey)(void *value, char *buffer, int bufferSize),
                    int keyBufferSize,
                    void (*reprValue)(void *value, char *buffer,
                                      int bufferSize),
                    int valueBufferSize);

#endif
