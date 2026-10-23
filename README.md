# 🎉 SmallCProblems 🎉

This repo contains a series of data structures written from scratch for the purpose of becoming a better programmer and understanding things. Additionally, there are some simple LeetCode-type problems solved in this repo.

AI has been used in this project to automate less exciting tasks such as writing documentation or refactoring. No agents have been used to implement features since the purpose of this project is to gain insights about programming, data structures and solving problems. The purpose is not productivity.

## 📦 Data Structures

Updated documentation with status codes and return values.

### Stack (`modules/stack/include/stack.h`)

A Last-In-First-Out (LIFO) data structure with dynamic capacity.

#### Types

**`STACK_STATUS`**
- `STACK_OK` when an operation succeeds
- `STACK_EMPTY` when peeking or popping from an empty stack
- `STACK_ERROR` when an operation fails, such as memory allocation failure or a null stack

**`stack_result_t`**
- Result type returned by peek and pop operations
- Contains:
  - `status` — one of the `STACK_STATUS` values above
  - `value` — the stored pointer when the operation succeeds, or `NULL` otherwise

#### Functions

**`my_stack_t *new_stack(size_t capacity)`**
- Creates a new stack with an initial capacity
- **Returns**: Pointer to the new stack, or `NULL` on failure

**`size_t stack_size(my_stack_t *stack)`**
- Returns the number of elements currently in the stack
- **Parameters**: `stack` - The stack
- **Returns**: Number of items in the stack

**`stack_result_t stack_peek(my_stack_t *stack)`**
- Retrieves the top element without removing it
- **Parameters**: `stack` - The stack
- **Returns**: `STACK_OK` with the top value, `STACK_EMPTY` if the stack is empty, `STACK_ERROR` if `stack` is `NULL`

**`STACK_STATUS stack_push(my_stack_t *stack, void *value)`**
- Adds a value to the top of the stack
- **Parameters**:
  - `stack` - The stack
  - `value` - Pointer to the value to add
- **Returns**: `STACK_OK` on success, `STACK_ERROR` on failure
- **Note**: The stack stores the pointer as-is; the caller manages the pointed-to memory

**`stack_result_t stack_pop(my_stack_t *stack)`**
- Removes and returns the top element
- **Parameters**: `stack` - The stack
- **Returns**: `STACK_OK` with the removed value, `STACK_EMPTY` if the stack is empty, `STACK_ERROR` if `stack` is `NULL`
- **Note**: The caller is responsible for freeing the returned value when needed

**`void stack_destroy(my_stack_t *stack, void (*destroy_value)(void *))`**
- Frees the stack structure and its internal storage
- **Parameters**:
  - `stack` - The stack to destroy
  - `destroy_value` - Optional callback used to free each stored value; pass `NULL` to leave stored values untouched
- **Warning**: After destroying a stack, accessing it or any values freed by `destroy_value` causes undefined behavior

---

### Bool Stack (`modules/bool_stack/include/bool_stack.h`)

A specialized Last-In-First-Out (LIFO) stack for boolean values with dynamic capacity. Values are stored as bits.

#### Types

**`BOOL_STACK_STATUS`**
- `BOOL_STACK_OK` when an operation succeeds
- `BOOL_STACK_EMPTY` when peeking or popping from an empty stack
- `BOOL_STACK_ERROR` when an operation fails, such as memory allocation failure

**`bool_stack_result_t`**
- Result type returned by peek and pop operations
- Contains:
  - `status` — one of the `BOOL_STACK_STATUS` values above
  - `value` — the stored bit (`0` or `1`) when the operation succeeds, or `0` otherwise

#### Functions

**`bool_stack_t *new_bool_stack(size_t capacity)`**
- Creates a new boolean stack with an initial capacity (rounded up to a multiple of 8)
- **Parameters**: `capacity` - Initial number of values the stack can hold
- **Returns**: Pointer to the new stack, or `NULL` on failure

**`int bool_stack_size(bool_stack_t *stack)`**
- Returns the number of values currently in the stack
- **Parameters**: `stack` - The bool stack
- **Returns**: Number of values in the stack

**`bool_stack_result_t bool_stack_peek(bool_stack_t *stack)`**
- Retrieves the top value without removing it
- **Parameters**: `stack` - The bool stack
- **Returns**: `BOOL_STACK_OK` with the top value, `BOOL_STACK_EMPTY` if the stack is empty

**`BOOL_STACK_STATUS bool_stack_push(bool_stack_t *stack, int value)`**
- Adds a value to the top of the stack; any non-zero value is stored as `1`
- **Parameters**:
  - `stack` - The bool stack
  - `value` - The value to add
- **Returns**: `BOOL_STACK_OK` on success, `BOOL_STACK_ERROR` on failure

**`bool_stack_result_t bool_stack_pop(bool_stack_t *stack)`**
- Removes and returns the top value
- **Parameters**: `stack` - The bool stack
- **Returns**: `BOOL_STACK_OK` with the removed value, `BOOL_STACK_EMPTY` if the stack is empty

**`void bool_stack_destroy(bool_stack_t *stack)`**
- Frees the stack structure and its internal storage
- **Parameters**: `stack` - The bool stack to destroy

---

### Queue (`modules/queue/include/queue.h`)

A First-In-First-Out (FIFO) data structure with dynamic capacity.

#### Types

**`QUEUE_STATUS`**
- `QUEUE_OK` when an operation succeeds
- `QUEUE_EMPTY` when peeking or removing from an empty queue
- `QUEUE_ERROR` when an operation fails, such as memory allocation failure or a null queue

**`queue_result_t`**
- Result type returned by peek and remove operations
- Contains:
  - `status` — one of the `QUEUE_STATUS` values above
  - `value` — the stored pointer when the operation succeeds, or `NULL` otherwise

#### Functions

**`queue_t *new_queue(size_t capacity)`**
- Creates a new queue with an initial capacity
- **Parameters**: `capacity` - Initial number of items the queue can hold (at least 1)
- **Returns**: Pointer to the new queue, or `NULL` on failure

**`size_t queue_size(queue_t *queue)`**
- Returns the number of elements currently in the queue
- **Parameters**: `queue` - The queue
- **Returns**: Number of items in the queue

**`queue_result_t queue_peek(queue_t *queue)`**
- Retrieves the front element without removing it
- **Parameters**: `queue` - The queue
- **Returns**: `QUEUE_OK` with the front value, `QUEUE_EMPTY` if the queue is empty, `QUEUE_ERROR` if `queue` is `NULL`

**`QUEUE_STATUS queue_add(queue_t *queue, void *value)`**
- Adds a value to the back of the queue
- **Parameters**:
  - `queue` - The queue
  - `value` - Pointer to the value to add
- **Returns**: `QUEUE_OK` on success, `QUEUE_ERROR` on failure
- **Note**: The queue stores the pointer as-is; the caller manages the pointed-to memory

**`queue_result_t queue_remove(queue_t *queue)`**
- Removes and returns the front element
- **Parameters**: `queue` - The queue
- **Returns**: `QUEUE_OK` with the removed value, `QUEUE_EMPTY` if the queue is empty, `QUEUE_ERROR` if `queue` is `NULL`
- **Note**: The caller is responsible for freeing the returned value when needed

**`void queue_destroy(queue_t *queue, void (*destroy_value)(void *))`**
- Frees the queue structure and its internal storage
- **Parameters**:
  - `queue` - The queue to destroy
  - `destroy_value` - Optional callback used to free each stored value; pass `NULL` to leave stored values untouched
- **Warning**: After destroying a queue, accessing it or any values freed by `destroy_value` causes undefined behavior

---

### Linked List (`modules/linked_list/include/linked_list.h`)

A doubly-linked list data structure with efficient insertion and removal at both ends.

#### Types

**`LINKED_LIST_STATUS`**
- `LINKED_LIST_OK` when an operation succeeds
- `LINKED_LIST_EMPTY` when peeking or popping from an empty list
- `LINKED_LIST_ERROR` when an operation fails, such as memory allocation failure

**`linked_list_result_t`**
- Result type returned by peek and pop operations
- Contains:
  - `status` — one of the `LINKED_LIST_STATUS` values above
  - `value` — the stored pointer when the operation succeeds, or `NULL` otherwise

#### Functions

**`linked_list_t *new_linked_list()`**
- Creates a new linked list
- **Returns**: Pointer to the new list, or `NULL` on failure

**`size_t linked_list_size(linked_list_t *list)`**
- Returns the number of elements currently in the list
- **Parameters**: `list` - The linked list
- **Returns**: Number of items in the list

**`linked_list_result_t linked_list_peek_first(linked_list_t *list)`**
- Retrieves the first element without removing it
- **Parameters**: `list` - The linked list
- **Returns**: `LINKED_LIST_OK` with the first value, `LINKED_LIST_EMPTY` if the list is empty

**`linked_list_result_t linked_list_peek_last(linked_list_t *list)`**
- Retrieves the last element without removing it
- **Parameters**: `list` - The linked list
- **Returns**: `LINKED_LIST_OK` with the last value, `LINKED_LIST_EMPTY` if the list is empty

**`LINKED_LIST_STATUS linked_list_add_first(linked_list_t *list, void *value)`**
- Adds a value at the start of the list
- **Parameters**:
  - `list` - The linked list
  - `value` - Pointer to the value to add
- **Returns**: `LINKED_LIST_OK` on success, `LINKED_LIST_ERROR` on failure
- **Note**: The list stores the pointer as-is; the caller manages the pointed-to memory

**`LINKED_LIST_STATUS linked_list_add_last(linked_list_t *list, void *value)`**
- Adds a value at the end of the list
- **Parameters**:
  - `list` - The linked list
  - `value` - Pointer to the value to add
- **Returns**: `LINKED_LIST_OK` on success, `LINKED_LIST_ERROR` on failure
- **Note**: The list stores the pointer as-is; the caller manages the pointed-to memory

**`linked_list_result_t linked_list_pop_first(linked_list_t *list)`**
- Removes and returns the first element
- **Parameters**: `list` - The linked list
- **Returns**: `LINKED_LIST_OK` with the removed value, `LINKED_LIST_EMPTY` if the list is empty
- **Note**: The caller is responsible for freeing the returned value when needed

**`linked_list_result_t linked_list_pop_last(linked_list_t *list)`**
- Removes and returns the last element
- **Parameters**: `list` - The linked list
- **Returns**: `LINKED_LIST_OK` with the removed value, `LINKED_LIST_EMPTY` if the list is empty
- **Note**: The caller is responsible for freeing the returned value when needed

**`void linked_list_destroy(linked_list_t *list, void (*destroy_value)(void *))`**
- Frees the list structure and its nodes
- **Parameters**:
  - `list` - The linked list to destroy
  - `destroy_value` - Optional callback used to free each stored value; pass `NULL` to leave stored values untouched
- **Warning**: After destroying a list, accessing it or any values freed by `destroy_value` causes undefined behavior

---

### Binary Tree (`modules/binary_tree/include/binary_tree.h`)

A binary search tree that keeps values sorted using a user-provided comparison function.

#### Types

**`BINARY_TREE_STATUS`**
- `BINARY_TREE_OK` when an operation succeeds
- `BINARY_TREE_NOT_FOUND` when the value is not in the tree
- `BINARY_TREE_DUPLICATE` when inserting a value that is already in the tree
- `BINARY_TREE_ERROR` when an operation fails, such as memory allocation failure

**`binary_tree_result_t`**
- Result type returned by find and insert
- Contains:
  - `status` — one of the `BINARY_TREE_STATUS` values above
  - `node` — the found or inserted node, or `NULL` otherwise

**`binary_tree_remove_result_t`**
- Result type returned by remove
- Contains:
  - `status` — one of the `BINARY_TREE_STATUS` values above
  - `value` — the removed value when the operation succeeds, or `NULL` otherwise

#### Functions

**`binary_tree_t *new_binary_tree(int (*compare_func)(const void *item1, const void *item2))`**
- Creates a new binary search tree
- **Parameters**: `compare_func` - Function to compare items (negative if item1 < item2, 0 if equal, positive if item1 > item2)
- **Returns**: Pointer to the new tree, or `NULL` on failure

**`binary_tree_result_t binary_tree_find(binary_tree_t *tree, void *item)`**
- Searches for a value in the tree
- **Parameters**:
  - `tree` - The binary tree
  - `item` - Pointer to the value to find
- **Returns**: `BINARY_TREE_OK` with the node, or `BINARY_TREE_NOT_FOUND`

**`binary_tree_result_t binary_tree_insert(binary_tree_t *tree, void *item)`**
- Inserts a value into the tree
- **Parameters**:
  - `tree` - The binary tree
  - `item` - Pointer to the value to insert
- **Returns**: `BINARY_TREE_OK` with the new node, `BINARY_TREE_DUPLICATE` if the value is already in the tree, `BINARY_TREE_ERROR` on failure
- **Note**: The tree stores the pointer as-is; the caller manages the pointed-to memory

**`binary_tree_remove_result_t binary_tree_remove(binary_tree_t *tree, void *value)`**
- Removes a value from the tree
- **Parameters**:
  - `tree` - The binary tree
  - `value` - Pointer to the value to remove
- **Returns**: `BINARY_TREE_OK` with the stored value, or `BINARY_TREE_NOT_FOUND`
- **Note**: The caller is responsible for freeing the returned value when needed

**`void binary_tree_destroy(binary_tree_t *tree, void (*destroy_value)(void *))`**
- Frees the tree and all its nodes
- **Parameters**:
  - `tree` - The binary tree to destroy
  - `destroy_value` - Callback used to free each stored value
- **Warning**: After destroying a tree, accessing it or any values freed by `destroy_value` causes undefined behavior

---

### Hash Table (`modules/hash_table/include/hash_table.h`)

A hash table that provides O(1) average-case lookup, insertion and deletion. Each bucket is an AVL tree, so a bucket with many collisions still resolves in O(log n). The number of buckets doubles when the average bucket holds more than 10 pairs.

#### Types

**`HASH_TABLE_STATUS`**
- `HASH_TABLE_OK` when an operation succeeds
- `HASH_TABLE_NOT_FOUND` when the key is not in the table
- `HASH_TABLE_DUPLICATE` when putting a key that is already in the table
- `HASH_TABLE_ERROR` when an operation fails, such as memory allocation failure or a null table

**`hash_table_result_t`**
- Result type returned by get
- Contains:
  - `status` — one of the `HASH_TABLE_STATUS` values above
  - `value` — the value stored for the key, or `NULL` otherwise

**`hash_table_remove_result_t`**
- Result type returned by remove
- Contains:
  - `status` — one of the `HASH_TABLE_STATUS` values above
  - `key`, `value` — the removed key and value, or `NULL` otherwise

#### Functions

**`hash_table_t *new_hash_table(size_t capacity, int (*compare_key)(const void *key1, const void *key2), unsigned long (*hash)(const void *key))`**
- Creates a new hash table
- **Parameters**:
  - `capacity` - Initial number of buckets (at least 1)
  - `compare_key` - Function to compare keys (negative if key1 < key2, 0 if equal, positive if key1 > key2)
  - `hash` - Function to compute hash values for keys
- **Returns**: Pointer to the new hash table, or `NULL` on failure or invalid arguments

**`size_t hash_table_size(hash_table_t *table)`**
- Returns the number of key-value pairs currently in the table
- **Parameters**: `table` - The hash table
- **Returns**: Number of pairs in the table (`0` if `table` is `NULL`)

**`hash_table_result_t hash_table_get(hash_table_t *table, const void *key)`**
- Retrieves the value associated with a key
- **Parameters**:
  - `table` - The hash table
  - `key` - Pointer to the key to search for
- **Returns**: `HASH_TABLE_OK` with the value, `HASH_TABLE_NOT_FOUND` if the key is not in the table, `HASH_TABLE_ERROR` if `table` is `NULL`

**`HASH_TABLE_STATUS hash_table_put(hash_table_t *table, void *key, void *value)`**
- Inserts a key-value pair into the table
- **Parameters**:
  - `table` - The hash table
  - `key` - Pointer to the key
  - `value` - Pointer to the value
- **Returns**: `HASH_TABLE_OK` on success, `HASH_TABLE_DUPLICATE` if the key already exists (stored value not modified), `HASH_TABLE_ERROR` on failure
- **Note**: The table stores the pointers as-is; the caller manages the pointed-to memory

**`hash_table_remove_result_t hash_table_remove(hash_table_t *table, const void *key)`**
- Removes a key-value pair from the table
- **Parameters**:
  - `table` - The hash table
  - `key` - Pointer to the key to remove
- **Returns**: `HASH_TABLE_OK` with the stored key and value, `HASH_TABLE_NOT_FOUND` if the key is not in the table, `HASH_TABLE_ERROR` on failure
- **Note**: The caller is responsible for freeing the returned key and value when needed

**`void hash_table_destroy(hash_table_t *table, void (*destroy_key)(void *), void (*destroy_value)(void *))`**
- Frees the hash table and its internal storage
- **Parameters**:
  - `table` - The hash table to destroy
  - `destroy_key` - Optional callback used to free each stored key; pass `NULL` to leave keys untouched
  - `destroy_value` - Optional callback used to free each stored value; pass `NULL` to leave values untouched
- **Warning**: After destroying a table, accessing it or any keys or values freed by the callbacks causes undefined behavior

**`void hash_table_print(hash_table_t *table, void (*repr_key)(const void *key, char *buffer, size_t buffer_size), size_t key_buffer_size, void (*repr_value)(const void *value, char *buffer, size_t buffer_size), size_t value_buffer_size)`**
- Prints the hash table contents, one bucket per line
- **Parameters**:
  - `table` - The hash table to print
  - `repr_key` - Function that writes a key's string representation into `buffer`
  - `key_buffer_size` - Size of the key string buffer
  - `repr_value` - Function that writes a value's string representation into `buffer`
  - `value_buffer_size` - Size of the value string buffer

---

### AVL Tree (`modules/avl_tree/include/avl_tree.h`)

A self-balancing binary search tree that maintains O(log n) height through automatic rotations.

#### Types

**`AVL_TREE_STATUS`**
- `AVL_TREE_OK` when an operation succeeds
- `AVL_TREE_NOT_FOUND` when the value is not in the tree
- `AVL_TREE_DUPLICATE` when inserting a value that is already in the tree
- `AVL_TREE_ERROR` when an operation fails, such as memory allocation failure or a null tree

**`avl_tree_result_t`**
- Result type returned by find and insert
- Contains:
  - `status` — one of the `AVL_TREE_STATUS` values above
  - `node` — the found, inserted or already existing node, or `NULL`

**`avl_tree_remove_result_t`**
- Result type returned by remove
- Contains:
  - `status` — one of the `AVL_TREE_STATUS` values above
  - `value` — the removed value when the operation succeeds, or `NULL` otherwise

#### Functions

**`avl_tree_t *new_avl_tree(int (*compare_func)(const void *item1, const void *item2))`**
- Creates a new AVL tree
- **Parameters**: `compare_func` - Function to compare items (negative if item1 < item2, 0 if equal, positive if item1 > item2)
- **Returns**: Pointer to the new tree, or `NULL` on failure (or if `compare_func` is `NULL`)

**`size_t avl_tree_size(avl_tree_t *tree)`**
- Returns the number of values currently in the tree
- **Parameters**: `tree` - The AVL tree
- **Returns**: Number of values in the tree (`0` if `tree` is `NULL`)

**`avl_tree_result_t avl_tree_find(avl_tree_t *tree, const void *value)`**
- Searches for a value in the tree
- **Parameters**:
  - `tree` - The AVL tree
  - `value` - Pointer to the value to find
- **Returns**: `AVL_TREE_OK` with the node, `AVL_TREE_NOT_FOUND` if it is not in the tree, `AVL_TREE_ERROR` if `tree` is `NULL`

**`avl_tree_result_t avl_tree_insert(avl_tree_t *tree, void *value)`**
- Inserts a value into the tree with automatic balancing
- **Parameters**:
  - `tree` - The AVL tree
  - `value` - Pointer to the value to insert
- **Returns**: `AVL_TREE_OK` with the new node, `AVL_TREE_DUPLICATE` with the existing node, `AVL_TREE_ERROR` on failure
- **Note**: The tree stores the pointer as-is; the caller manages the pointed-to memory

**`avl_tree_remove_result_t avl_tree_remove(avl_tree_t *tree, const void *value)`**
- Removes a value from the tree with automatic rebalancing
- **Parameters**:
  - `tree` - The AVL tree
  - `value` - Pointer to the value to remove
- **Returns**: `AVL_TREE_OK` with the stored value, `AVL_TREE_NOT_FOUND` if it is not in the tree, `AVL_TREE_ERROR` on failure
- **Note**: The caller is responsible for freeing the returned value when needed

**`void avl_tree_destroy(avl_tree_t *tree, void (*destroy_value)(void *))`**
- Frees the tree and all its nodes
- **Parameters**:
  - `tree` - The AVL tree to destroy
  - `destroy_value` - Optional callback used to free each stored value; pass `NULL` to leave stored values untouched
- **Warning**: After destroying a tree, accessing it or any values freed by `destroy_value` causes undefined behavior

**`void avl_tree_print(avl_tree_t *tree, void (*repr)(avl_tree_node_t *node, char *buffer, size_t buffer_size), size_t buffer_size)`**
- Prints the tree structure (trees with height up to 6)
- **Parameters**:
  - `tree` - The AVL tree to print
  - `repr` - Function that writes a node's string representation into `buffer`
  - `buffer_size` - Size of the string buffer

---

### Bloom Filter (`modules/bloom_filter/include/bloom_filter.h`)

A probabilistic set that answers "definitely not added" or "possibly added" using a fixed-size bit array. It uses double hashing, so it needs two hash functions (for example, the ones in `modules/murmur_hash`).

#### Functions

**`bloom_filter_t *new_bloom_filter(size_t num_items, double desired_fp_rate, uint32_t (*hash1)(void *), uint32_t (*hash2)(void *))`**
- Creates a bloom filter sized for the expected number of items and false-positive rate
- **Parameters**:
  - `num_items` - Expected number of items to add
  - `desired_fp_rate` - Target false-positive rate (between 0 and 1)
  - `hash1`, `hash2` - Hash functions used to derive the bit positions
- **Returns**: Pointer to the new bloom filter, or `NULL` on failure

**`void bloom_filter_add(bloom_filter_t *filter, void *item)`**
- Adds an item to the filter
- **Parameters**:
  - `filter` - The bloom filter
  - `item` - Pointer to the item to add
- **Note**: The filter doesn't store the item, only its hashes

**`int bloom_filter_check(bloom_filter_t *filter, void *item)`**
- Checks whether an item may have been added
- **Parameters**:
  - `filter` - The bloom filter
  - `item` - Pointer to the item to check
- **Returns**: `0` if the item was definitely not added, `1` if it possibly was

**`void bloom_filter_destroy(bloom_filter_t *filter)`**
- Frees the bloom filter and its bit array
- **Parameters**: `filter` - The bloom filter to destroy
