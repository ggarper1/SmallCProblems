#include <stddef.h>

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef enum {
  HASH_TABLE_NOT_FOUND,
  HASH_TABLE_DUPLICATE,
  HASH_TABLE_ERROR,
  HASH_TABLE_OK
} HASH_TABLE_STATUS;

typedef struct hash_table hash_table_t;

typedef struct hash_table_result {
  HASH_TABLE_STATUS status;
  void *value;
} hash_table_result_t;

typedef struct hash_table_remove_result {
  HASH_TABLE_STATUS status;
  void *key;
  void *value;
} hash_table_remove_result_t;

// --- Public Function Prototypes ---

/**
 * Creates a new Hash Table.
 * @param capacity Initial number of buckets (must be at least 1).
 * @param compare_key Function to compare keys (0 if equal, negative if
 * key1 < key2, positive if key1 > key2).
 * @param hash Function to hash keys.
 * @return A pointer to the new Hash Table, or NULL on failure.
 */
hash_table_t *new_hash_table(size_t capacity,
                             int (*compare_key)(const void *key1,
                                                const void *key2),
                             unsigned long (*hash)(const void *key));

/**
 * Returns a Hash Table's size (number of key-value pairs in it).
 * @param table The Hash Table.
 * @return the number of pairs in it (0 if table is NULL).
 */
size_t hash_table_size(hash_table_t *table);

/**
 * Gets the value associated to a key in a Hash Table.
 * @param table The Hash Table.
 * @param key The key.
 * @return HASH_TABLE_OK and the value if the key is in the table,
 * HASH_TABLE_NOT_FOUND and NULL otherwise (HASH_TABLE_ERROR if table is NULL).
 */
hash_table_result_t hash_table_get(hash_table_t *table, const void *key);

/**
 * Puts a key-value pair in a Hash Table. If the key is already in it, the
 * stored value is not modified.
 * Pointers to the key and value are stored (caller manages memory).
 * @param table The Hash Table.
 * @param key The key.
 * @param value The value.
 * @return the status of the operation (HASH_TABLE_OK, HASH_TABLE_DUPLICATE or
 * HASH_TABLE_ERROR).
 */
HASH_TABLE_STATUS hash_table_put(hash_table_t *table, void *key, void *value);

/**
 * Removes a key-value pair from a Hash Table.
 * @param table The Hash Table.
 * @param key The key.
 * @return HASH_TABLE_OK and the stored key and value (caller frees them if
 * needed), or HASH_TABLE_NOT_FOUND / HASH_TABLE_ERROR and NULLs.
 */
hash_table_remove_result_t hash_table_remove(hash_table_t *table,
                                             const void *key);

/**
 * Destroys the Hash Table and frees all allocated memory.
 * @param table The Hash Table to destroy.
 * @param destroy_key The function used to destroy the keys, or NULL if keys
 * should not be destroyed.
 * @param destroy_value The function used to destroy the values, or NULL if
 * values should not be destroyed.
 */
void hash_table_destroy(hash_table_t *table, void (*destroy_key)(void *),
                        void (*destroy_value)(void *));

/**
 * Prints the Hash Table, one bucket per line.
 * @param table The Hash Table to print.
 * @param repr_key Function that writes a key's representation into buffer.
 * @param key_buffer_size Size of the buffer passed to repr_key.
 * @param repr_value Function that writes a value's representation into buffer.
 * @param value_buffer_size Size of the buffer passed to repr_value.
 */
void hash_table_print(hash_table_t *table,
                      void (*repr_key)(const void *key, char *buffer,
                                       size_t buffer_size),
                      size_t key_buffer_size,
                      void (*repr_value)(const void *value, char *buffer,
                                         size_t buffer_size),
                      size_t value_buffer_size);

#endif
