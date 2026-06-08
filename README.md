# 🎉 SmallCProblems 🎉

This repo contains a series of data structures written from scratch for the purpose of becoming a better programmer and understanding things. Additionally, there are some simple LeetCode-type problems solved in this repo.

AI has only been used in this project to automate boring tasks such as writing documentation. It has also been used to propose refactors and code modifications in order to obtain more readable and prettier code. No agents have been used to implement features since the purpose of this project is to gain insights about programming, data structures and solving problems. The purpose is not productivity.

## 📦 Data Structures

Updated documentation with status codes and return values.

### Stack (`modules/stack/include/stack.h`)

A Last-In-First-Out (LIFO) data structure with dynamic capacity.

#### Functions

**`Stack_t *newStack(size_t capacity)`**
- Creates a new stack with an initial capacity
- **Parameters**: `capacity` - Initial number of items the stack can hold
- **Returns**: Pointer to the new Stack, or NULL on failure

**`size_t stackSize(Stack_t *stack)`**
- Returns the number of elements currently in the stack
- **Parameters**: `stack` - The Stack
- **Returns**: Number of items in the stack

**`StackResult stackPeek(Stack_t *stack)`**
- Retrieves the top element without removing it
- **Parameters**: `stack` - The Stack
- **Returns**: `StackResult` with:
  - `STACK_OK` and the top element in `value` when successful
  - `STACK_EMPTY` and `NULL` when the stack is empty
  - `STACK_ERROR` and `NULL` when `stack` is NULL

**`STACK_STATUS stackPush(Stack_t *stack, void *value)`**
- Adds a value to the top of the stack
- **Parameters**: 
  - `stack` - The Stack
  - `value` - Pointer to the value to add
- **Returns**: `STACK_OK` on success, `STACK_ERROR` on failure (e.g., NULL stack or memory allocation error)
- **Note**: Caller manages memory for the value

**`StackResult stackPop(Stack_t *stack)`**
- Removes and returns the top element
- **Parameters**: `stack` - The Stack
- **Returns**: `StackResult` with:
  - `STACK_OK` and the removed element in `value` when successful
  - `STACK_EMPTY` and `NULL` when the stack is empty
  - `STACK_ERROR` and `NULL` when `stack` is NULL
- **Note**: Caller is responsible for freeing the returned value

**`void stackDestroy(Stack_t *stack, void (*destroyValue)(void *))`**
- Frees the stack structure and its internal storage
- **Parameters**:
  - `stack` - The Stack to destroy
  - `destroyValue` - Optional callback used to free each stored value; pass NULL to leave stored values untouched
- **Warning**: After destroying a stack, accessing the stack or values freed by `destroyValue` causes undefined behavior

---

### BoolStack (`boolStack.h`)

A specialized stack for boolean values with dynamic capacity. Values are stored as bits.

#### Functions

**`BoolStack_t *newBoolStack(size_t capacity)`**
- Creates a new boolean stack with an initial capacity
- **Parameters**: `capacity` - Initial number of items the stack can hold
- **Returns**: Pointer to the new BoolStack, or NULL on failure

**`int bsLength(BoolStack_t *stack)`**
- Returns the number of elements currently in the stack
- **Parameters**: `stack` - The BoolStack
- **Returns**: Number of items in the stack

**`int bsPeek(BoolStack_t *stack)`**
- Retrieves the top element without removing it
- **Parameters**: `stack` - The BoolStack
- **Returns**: The top element value

**`BS_STATUS bsPush(BoolStack_t *stack, int value)`**
- Adds a value to the top of the stack
- **Parameters**: 
  - `stack` - The BoolStack
  - `value` - The integer/boolean value to add
- **Returns**: `BS_OK` on success, `BS_ERROR` on failure (e.g., memory allocation error)

**`int bsPop(BoolStack_t *stack)`**
- Removes and returns the top element
- **Parameters**: `stack` - The BoolStack
- **Returns**: The removed value

**`void bsDestroy(BoolStack_t *stack)`**
- Frees the stack structure
- **Parameters**: `stack` - The BoolStack to destroy

**`void printBoolStack(BoolStack_t *stack)`**
- Prints the stack contents
- **Parameters**: `stack` - The BoolStack to print

---

### Queue (`queue.h`)

A First-In-First-Out (FIFO) data structure with dynamic capacity.

#### Functions

**`Queue_t *newQueue(size_t capacity)`**
- Creates a new queue with an initial capacity
- **Parameters**: `capacity` - Initial number of items the queue can hold
- **Returns**: Pointer to the new Queue, or NULL on failure

**`int qLength(Queue_t *queue)`**
- Returns the number of elements currently in the queue
- **Parameters**: `queue` - The Queue
- **Returns**: Number of items in the queue

**`void *qPeek(Queue_t *queue)`**
- Retrieves the front element without removing it
- **Parameters**: `queue` - The Queue
- **Returns**: Pointer to the front element, or NULL if empty

**`Q_STATUS qAdd(Queue_t *queue, void *value)`**
- Adds a value to the back of the queue
- **Parameters**: 
  - `queue` - The Queue
  - `value` - Pointer to the value to add
- **Returns**: `Q_OK` on success, `Q_ERROR` on failure (e.g., memory allocation error)
- **Note**: Caller manages memory for the value

**`void *qRemove(Queue_t *queue)`**
- Removes and returns the front element
- **Parameters**: `queue` - The Queue
- **Returns**: Pointer to the removed value, or NULL if empty
- **Note**: Caller is responsible for freeing the returned value

**`void qDestroy(Queue_t *queue)`**
- Frees the queue structure only
- **Parameters**: `queue` - The Queue to destroy
- **Note**: Does not free stored values

**`void qDestroyAll(Queue_t *queue)`**
- Frees the queue structure and all stored values
- **Parameters**: `queue` - The Queue to destroy
- **Warning**: After calling this, accessing previously stored elements causes undefined behavior

**`void printQueue(Queue_t *queue, void (*repr)(void *value, char *buffer, int bufferSize), int bufferSize)`**
- Prints the queue contents
- **Parameters**:
  - `queue` - The Queue to print
  - `repr` - Function to convert values to string representation
  - `bufferSize` - Size of the string buffer

---

### Linked List (`linkedList.h`)

A doubly-linked list data structure that allows efficient insertion and removal at both ends.

#### Functions

**`LinkedList_t *newLinkedList()`**
- Creates a new linked list
- **Returns**: Pointer to the new LinkedList, or NULL on failure

**`int llLength(LinkedList_t *list)`**
- Returns the number of elements currently in the list
- **Parameters**: `list` - The LinkedList
- **Returns**: Number of items in the list

**`void *llPeekFirst(LinkedList_t *list)`**
- Retrieves the first element without removing it
- **Parameters**: `list` - The LinkedList
- **Returns**: Pointer to the first element, or NULL if empty

**`void *llPeekLast(LinkedList_t *list)`**
- Retrieves the last element without removing it
- **Parameters**: `list` - The LinkedList
- **Returns**: Pointer to the last element, or NULL if empty

**`LL_STATUS llAddFirst(LinkedList_t *list, void *value)`**
- Adds a value at the start of the list
- **Parameters**: 
  - `list` - The LinkedList
  - `value` - Pointer to the value to add
- **Returns**: `LL_OK` on success, `LL_ERROR` on failure (e.g., memory allocation error)
- **Note**: Caller manages memory for the value

**`LL_STATUS llAddLast(LinkedList_t *list, void *value)`**
- Adds a value at the end of the list
- **Parameters**: 
  - `list` - The LinkedList
  - `value` - Pointer to the value to add
- **Returns**: `LL_OK` on success, `LL_ERROR` on failure (e.g., memory allocation error)
- **Note**: Caller manages memory for the value

**`void *llPopFirst(LinkedList_t *list)`**
- Removes and returns the first element
- **Parameters**: `list` - The LinkedList
- **Returns**: Pointer to the removed value, or NULL if empty
- **Note**: Caller is responsible for freeing the returned value

**`void *llPopLast(LinkedList_t *list)`**
- Removes and returns the last element
- **Parameters**: `list` - The LinkedList
- **Returns**: Pointer to the removed value, or NULL if empty
- **Note**: Caller is responsible for freeing the returned value

**`void llDestroy(LinkedList_t *list)`**
- Frees the list structure only
- **Parameters**: `list` - The LinkedList to destroy
- **Note**: Does not free stored values

**`void llDestroyAll(LinkedList_t *list)`**
- Frees the list structure and all stored values
- **Parameters**: `list` - The LinkedList to destroy
- **Warning**: After calling this, accessing previously stored elements causes undefined behavior

**`void printLinkedList(LinkedList_t *list, void (*repr)(void *value, char *buffer, int bufferSize), int bufferSize)`**
- Prints the linked list contents
- **Parameters**:
  - `list` - The LinkedList to print
  - `repr` - Function to convert values to string representation
  - `bufferSize` - Size of the string buffer

---

### Binary Tree (`binaryTree.h`)

A binary search tree implementation that maintains sorted order based on a user-provided comparison function.

#### Functions

**`BinaryTree_t *newBinaryTree(int (*compare_func)(const void *item1, const void *item2), void (*freeItem)(void *item))`**
- Creates a new Binary Search Tree
- **Parameters**: 
  - `compare_func` - Function to compare items (should return negative if item1 < item2, 0 if equal, positive if item1 > item2)
  - `freeItem` - Function to free individual items when using `btDestroyAll()`
- **Returns**: Pointer to the new BinaryTree, or NULL on failure

**`BTNode_t *btFind(BinaryTree_t *tree, void *item)`**
- Searches for an element in the tree
- **Parameters**: 
  - `tree` - The Binary Tree
  - `item` - Pointer to the item to find
- **Returns**: Pointer to the node containing the element, or NULL if not found

**`BT_STATUS btInsert(BinaryTree_t *tree, void *item, BTNode_t **node)`**
- Inserts an item into the tree
- **Parameters**: 
  - `tree` - The Binary Tree
  - `item` - Pointer to the item to insert
  - `node` - Output parameter: pointer to the new node (or existing node if duplicate)
- **Returns**: 
  - `BT_OK` - Item successfully inserted
  - `BT_DUPLICATE` - Item already exists in tree (node points to existing node)
  - `BT_ERROR` - Insertion failed (e.g., memory allocation error)
- **Note**: Caller manages memory for the item

**`void *btRemove(BinaryTree_t *tree, void *value)`**
- Removes an item from the tree
- **Parameters**: 
  - `tree` - The Binary Tree
  - `value` - Pointer to the value to remove
- **Returns**: Pointer to the removed value, or NULL if the item does not exist
- **Note**: Caller is responsible for freeing the returned value

**`void btDestroy(BinaryTree_t *tree)`**
- Frees the tree structure only
- **Parameters**: `tree` - The BinaryTree to destroy
- **Note**: Does not free stored values

**`void btDestroyAll(BinaryTree_t *tree)`**
- Frees the tree structure and all stored values using the `freeItem` function
- **Parameters**: `tree` - The Binary Tree to destroy
- **Warning**: After calling this, accessing previously stored elements causes undefined behavior

**`void printTree(BinaryTree_t *tree, void (*repr)(BTNode_t *value, char *buffer, int bufferSize), int bufferSize)`**
- Prints the tree structure
- **Parameters**:
  - `tree` - The Binary Tree to print
  - `repr` - Function to convert node values to string representation
  - `bufferSize` - Size of the string buffer

---

### Hash Table (`hashTable.h`)

A hash table implementation that provides O(1) average-case lookup, insertion, and deletion operations using separate chaining for collision resolution.

#### Functions

**`HashTable_t *newHashTable(size_t capacity, int (*compare)(const void *key1, const void *key2), unsigned long (*hash)(void *key))`**
- Creates a new hash table
- **Parameters**: 
  - `capacity` - Initial number of items the table can hold
  - `compare` - Function to compare keys (should return 0 if equal, non-zero otherwise)
  - `hash` - Function to compute hash values for keys
- **Returns**: Pointer to the new HashTable, or NULL on failure

**`int htLength(HashTable_t *hashTable)`**
- Returns the number of key-value pairs currently in the table
- **Parameters**: `hashTable` - The HashTable
- **Returns**: Number of items in the table

**`HT_STATUS htGet(HashTable_t *hashTable, void *key, void **value)`**
- Retrieves the value associated with a key
- **Parameters**: 
  - `hashTable` - The HashTable
  - `key` - Pointer to the key to search for
  - `value` - Output parameter: pointer where the value will be stored
- **Returns**: 
  - `HT_OK` - Key found, value stored in the output parameter
  - `HT_NOT_FOUND` - Key does not exist in the table

**`HT_STATUS htPut(HashTable_t *hashTable, void *key, void *value)`**
- Inserts a key-value pair into the table
- **Parameters**: 
  - `hashTable` - The HashTable
  - `key` - Pointer to the key
  - `value` - Pointer to the value
- **Returns**: 
  - `HT_OK` - Key-value pair successfully inserted
  - `HT_DUPLICATE` - Key already exists (value not modified)
- **Note**: Caller manages memory for both key and value

**`HT_STATUS htRemove(HashTable_t *hashTable, void *key, void **pair)`**
- Removes a key-value pair from the table
- **Parameters**: 
  - `hashTable` - The HashTable
  - `key` - Pointer to the key to remove
  - `pair` - Output parameter: pointer where the removed Pair_t structure will be returned
- **Returns**: Status code indicating result of operation
- **Note**: Caller is responsible for freeing the returned pair and its contents

**`void htDestroyAll(HashTable_t *hashTable)`**
- Frees the hash table structure and all stored keys and values
- **Parameters**: `hashTable` - The HashTable to destroy
- **Warning**: After calling this, accessing previously stored elements causes undefined behavior
- **Note**: No separate `htDestroy()` function exists - only `htDestroyAll()`

**`void printHashTable(HashTable_t *hashTable, void (*reprKey)(void *value, char *buffer, int bufferSize), int keyBufferSize, void (*reprValue)(void *value, char *buffer, int bufferSize), int valueBufferSize)`**
- Prints the hash table contents
- **Parameters**:
  - `hashTable` - The HashTable to print
  - `reprKey` - Function to convert keys to string representation
  - `keyBufferSize` - Size of the key string buffer
  - `reprValue` - Function to convert values to string representation
  - `valueBufferSize` - Size of the value string buffer

---

### AVL Binary Tree (`avlBinaryTree.h`)

A self-balancing binary search tree that maintains O(log n) height through automatic rotations.

#### Functions

**`AVLBinaryTree_t *newAVLBinaryTree(int (*compare_func)(const void *item1, const void *item2), void (*freeItem)(void *item))`**
- Creates a new AVL Binary Search Tree
- **Parameters**: 
  - `compare_func` - Function to compare items (should return negative if item1 < item2, 0 if equal, positive if item1 > item2)
  - `freeItem` - Function to free individual items when using `avlDestroyAll()`
- **Returns**: Pointer to the new AVLBinaryTree, or NULL on failure

**`AVLNode_t *avlFind(AVLBinaryTree_t *tree, void *item)`**
- Searches for an element in the tree
- **Parameters**: 
  - `tree` - The AVL Binary Tree
  - `item` - Pointer to the item to find
- **Returns**: Pointer to the node containing the element, or NULL if not found

**`AVL_STATUS avlInsert(AVLBinaryTree_t *tree, void *item, AVLNode_t **node)`**
- Inserts an item into the tree with automatic balancing
- **Parameters**: 
  - `tree` - The AVL Binary Tree
  - `item` - Pointer to the item to insert
  - `node` - Output parameter: pointer to the newly created node (or existing node if duplicate)
- **Returns**: 
  - `AVL_OK` - Item successfully inserted
  - `AVL_DUPLICATE` - Item already exists in tree (node points to existing node)
  - `AVL_ERROR` - Insertion failed (e.g., memory allocation error)
- **Note**: Caller manages memory for the item

**`AVL_STATUS avlRemove(AVLBinaryTree_t *tree, const void *value, void **retValue)`**
- Removes an item from the tree with automatic rebalancing
- **Parameters**: 
  - `tree` - The AVL Binary Tree
  - `value` - Pointer to the value to remove
  - `retValue` - Output parameter: pointer where the removed value will be returned
- **Returns**: 
  - `AVL_OK` - Item successfully removed
  - `AVL_NOT_FOUND` - Item does not exist in tree
  - `AVL_ERROR` - Removal failed
- **Note**: Caller is responsible for freeing the returned value

**`void avlDestroy(AVLBinaryTree_t *tree)`**
- Frees the tree structure only
- **Parameters**: `tree` - The AVLBinaryTree to destroy
- **Note**: Does not free stored values

**`void avlDestroyAll(AVLBinaryTree_t *tree)`**
- Frees the tree structure and all stored values using the `freeItem` function
- **Parameters**: `tree` - The AVL Binary Tree to destroy
- **Warning**: After calling this, accessing previously stored elements causes undefined behavior

**`void avlPrintTree(AVLBinaryTree_t *tree, void (*repr)(AVLNode_t *value, char *buffer, int bufferSize), int bufferSize)`**
- Prints the tree structure
- **Parameters**:
  - `tree` - The AVL Binary Tree to print
  - `repr` - Function to convert node values to string representation
  - `bufferSize` - Size of the string buffer
