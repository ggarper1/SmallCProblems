# 📦 Data Structures

Updated documentation with status codes and return values.

## Stack (`stack.h`)

A Last-In-First-Out (LIFO) data structure with dynamic capacity.

### Type Definitions
```c
typedef struct Stack Stack_t;
typedef enum { S_ERROR, S_OK } S_STATUS;
```

### Functions

**`Stack_t *newStack(size_t capacity)`**
- Creates a new stack with an initial capacity
- **Parameters**: `capacity` - Initial number of items the stack can hold
- **Returns**: Pointer to the new Stack, or NULL on failure

**`int sLength(Stack_t *stack)`**
- Returns the number of elements currently in the stack
- **Parameters**: `stack` - The Stack
- **Returns**: Number of items in the stack

**`void *sPeek(Stack_t *stack)`**
- Retrieves the top element without removing it
- **Parameters**: `stack` - The Stack
- **Returns**: Pointer to the top element, or NULL if empty

**`S_STATUS sPush(Stack_t *stack, void *value)`**
- Adds a value to the top of the stack
- **Parameters**: 
  - `stack` - The Stack
  - `value` - Pointer to the value to add
- **Returns**: `S_OK` on success, `S_ERROR` on failure (e.g., memory allocation error)
- **Note**: Caller manages memory for the value

**`void *sPop(Stack_t *stack)`**
- Removes and returns the top element
- **Parameters**: `stack` - The Stack
- **Returns**: Pointer to the removed value, or NULL if empty
- **Note**: Caller is responsible for freeing the returned value

**`void sDestroy(Stack_t *stack)`**
- Frees the stack structure only
- **Parameters**: `stack` - The Stack to destroy
- **Note**: Does not free stored values

**`void sDestroyAll(Stack_t *stack)`**
- Frees the stack structure and all stored values
- **Parameters**: `stack` - The Stack to destroy
- **Warning**: After calling this, accessing previously stored elements causes undefined behavior

**`void printStack(Stack_t *stack, void (*repr)(void *value, char *buffer, int bufferSize), int bufferSize)`**
- Prints the stack contents
- **Parameters**:
  - `stack` - The Stack to print
  - `repr` - Function to convert values to string representation
  - `bufferSize` - Size of the string buffer

---

## BoolStack (`boolStack.h`)

A specialized stack for boolean values with dynamic capacity. Values are stored as bits.

### Type Definitions
```c
typedef struct Stack BoolStack_t;
typedef enum { BS_ERROR, BS_OK } BS_STATUS;
```

### Functions

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

## Queue (`queue.h`)

A First-In-First-Out (FIFO) data structure with dynamic capacity.

### Type Definitions
```c
typedef struct Queue Queue_t;
typedef enum { Q_OK, Q_ERROR } Q_STATUS;
```

### Functions

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

## Linked List (`linkedList.h`)

A doubly-linked list data structure that allows efficient insertion and removal at both ends.

### Type Definitions
```c
typedef struct LinkedList LinkedList_t;
typedef enum { LL_OK, LL_ERROR } LL_STATUS;
```

### Functions

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

## Binary Tree (`binaryTree.h`)

A binary search tree implementation that maintains sorted order based on a user-provided comparison function.

### Type Definitions
```c
typedef enum { BT_NOT_FOUND, BT_DUPLICATE, BT_ERROR, BT_OK } BT_STATUS;

typedef struct BTNode {
  void *value;
  struct BTNode *right;
  struct BTNode *left;
} BTNode_t;

typedef struct BinaryTree {
  size_t length;
  int height;
  BTNode_t *root;
  int (*compare_func)(const void *item1, const void *item2);
} BinaryTree_t;
```

### Functions

**`BinaryTree_t *newBinaryTree(int (*compare_func)(const void *item1, const void *item2))`**
- Creates a new Binary Search Tree
- **Parameters**: `compare_func` - Function to compare items (should return negative if item1 < item2, 0 if equal, positive if item1 > item2)
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

**`int btRemove(BinaryTree_t *tree, void *value)`**
- Removes an item from the tree
- **Parameters**: 
  - `tree` - The Binary Tree
  - `value` - Pointer to the value to remove
- **Returns**: Status code indicating result of operation

**`void btDestroy(BinaryTree_t *tree)`**
- Frees the tree structure only
- **Parameters**: `tree` - The BinaryTree to destroy
- **Note**: Does not free stored values

**`void btDestroyAll(BinaryTree_t *tree)`**
- Frees the tree structure and all stored values
- **Parameters**: `tree` - The Binary Tree to destroy
- **Warning**: After calling this, accessing previously stored elements causes undefined behavior

**`void printTree(BinaryTree_t *tree, void (*repr)(BTNode_t *value, char *buffer, int bufferSize), int bufferSize)`**
- Prints the tree structure
- **Parameters**:
  - `tree` - The Binary Tree to print
  - `repr` - Function to convert node values to string representation
  - `bufferSize` - Size of the string buffer

---

## Hash Table (`hashTable.h`)

A hash table implementation that provides O(1) average-case lookup, insertion, and deletion operations using separate chaining for collision resolution.

### Type Definitions
```c
typedef enum { HT_OK, HT_ERROR, HT_NOT_FOUND, HT_DUPLICATE } HT_STATUS;
typedef struct HashTable HashTable_t;
```

### Functions

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

**`HT_STATUS htRemove(HashTable_t *hashTable, void *key)`**
- Removes a key-value pair from the table
- **Parameters**: 
  - `hashTable` - The HashTable
  - `key` - Pointer to the key to remove
- **Returns**: Status code indicating result of operation

**`void htDestroy(HashTable_t *hashTable)`**
- Frees the hash table structure only
- **Parameters**: `hashTable` - The HashTable to destroy
- **Note**: Does not free stored keys or values

**`void htDestroyAll(HashTable_t *hashTable)`**
- Frees the hash table structure and all stored keys and values
- **Parameters**: `hashTable` - The HashTable to destroy
- **Warning**: After calling this, accessing previously stored elements causes undefined behavior

**`void printHashTable(HashTable_t *hashTable, void (*reprKey)(void *value, char *buffer, int bufferSize), int keyBufferSize, void (*reprValue)(void *value, char *buffer, int bufferSize), int valueBufferSize)`**
- Prints the hash table contents
- **Parameters**:
  - `hashTable` - The HashTable to print
  - `reprKey` - Function to convert keys to string representation
  - `keyBufferSize` - Size of the key string buffer
  - `reprValue` - Function to convert values to string representation
  - `valueBufferSize` - Size of the value string buffer

---

## AVL Binary Tree (`avlBinaryTree.h`)

A self-balancing binary search tree that maintains O(log n) height through automatic rotations.

### Type Definitions
```c
typedef enum { AVL_OK, AVL_ERROR, AVL_NOT_FOUND, AVL_DUPLICATE } AVL_STATUS;

typedef struct AVLNode {
  void *value;
  int lHeight;
  int rHeight;
  struct AVLNode *right;
  struct AVLNode *left;
} AVLNode_t;

typedef struct AVLBinaryTree {
  size_t length;
  int height;
  int capacity;
  AVLNode_t *root;
  int (*compare_func)(const void *item1, const void *item2);
} AVLBinaryTree_t;
```

### Functions

**`AVLBinaryTree_t *newAVLBinaryTree(int (*compare_func)(const void *item1, const void *item2))`**
- Creates a new AVL Binary Search Tree
- **Parameters**: `compare_func` - Function to compare items (should return negative if item1 < item2, 0 if equal, positive if item1 > item2)
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

**`AVL_STATUS avlRemove(AVLBinaryTree_t *tree, const void *value)`**
- Removes an item from the tree with automatic rebalancing
- **Parameters**: 
  - `tree` - The AVL Binary Tree
  - `value` - Pointer to the value to remove
- **Returns**: 
  - `AVL_OK` - Item successfully removed
  - `AVL_NOT_FOUND` - Item does not exist in tree
  - `AVL_ERROR` - Removal failed

**`void avlDestroy(AVLBinaryTree_t *tree)`**
- Frees the tree structure only
- **Parameters**: `tree` - The AVLBinaryTree to destroy
- **Note**: Does not free stored values

**`void avlDestroyAll(AVLBinaryTree_t *tree)`**
- Frees the tree structure and all stored values
- **Parameters**: `tree` - The AVL Binary Tree to destroy
- **Warning**: After calling this, accessing previously stored elements causes undefined behavior

**`void avlPrintTree(AVLBinaryTree_t *tree, void (*repr)(AVLNode_t *value, char *buffer, int bufferSize), int bufferSize)`**
- Prints the tree structure
- **Parameters**:
  - `tree` - The AVL Binary Tree to print
  - `repr` - Function to convert node values to string representation
  - `bufferSize` - Size of the string buffer

---

# 💡 Usage Examples

## Stack Example
```c
#include "utils/include/stack.h"
#include <stdlib.h>
#include <stdio.h>

// Create a stack for integers
Stack_t *stack = newStack(10);

// Push values
int *val = malloc(sizeof(int));
*val = 42;
S_STATUS status = sPush(stack, val);
if (status == S_ERROR) {
    fprintf(stderr, "Failed to push value\n");
}

// Pop value
int *retrieved = (int *)sPop(stack);
if (retrieved) {
    printf("Value: %d\n", *retrieved);
    free(retrieved);
}

// Clean up
sDestroy(stack);
```

## BoolStack Example
```c
#include "utils/include/boolStack.h"

// Create a boolean stack
BoolStack_t *stack = newBoolStack(10);

// Push values
BS_STATUS status = bsPush(stack, 1);
if (status == BS_OK) {
    bsPush(stack, 0);
    bsPush(stack, 1);
}

// Pop value
int value = bsPop(stack);
printf("Value: %d\n", value);

// Clean up
bsDestroy(stack);
```

## Queue Example
```c
#include "utils/include/queue.h"
#include <stdlib.h>

// Create a queue
Queue_t *queue = newQueue(10);

// Add values
int *val = malloc(sizeof(int));
*val = 100;
Q_STATUS status = qAdd(queue, val);
if (status == Q_ERROR) {
    fprintf(stderr, "Failed to add value\n");
    free(val);
}

// Remove value
int *retrieved = (int *)qRemove(queue);
if (retrieved) {
    free(retrieved);
}

// Clean up
qDestroy(queue);
```

## Linked List Example
```c
#include "utils/include/linkedList.h"
#include <stdlib.h>

// Create a linked list
LinkedList_t *list = newLinkedList();

// Add values
int *val1 = malloc(sizeof(int));
*val1 = 10;
LL_STATUS status = llAddLast(list, val1);
if (status == LL_ERROR) {
    fprintf(stderr, "Failed to add value\n");
    free(val1);
}

int *val2 = malloc(sizeof(int));
*val2 = 20;
llAddFirst(list, val2);

// Remove values
int *first = (int *)llPopFirst(list);
int *last = (int *)llPopLast(list);
free(first);
free(last);

// Clean up
llDestroy(list);
```

## Binary Tree Example
```c
#include "utils/include/binaryTree.h"
#include <stdlib.h>

// Comparison function for integers
int compareInts(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Create tree
BinaryTree_t *tree = newBinaryTree(compareInts);

// Insert values
int *val = malloc(sizeof(int));
*val = 50;
BTNode_t *node = NULL;
BT_STATUS status = btInsert(tree, val, &node);
if (status == BT_OK) {
    printf("Inserted successfully\n");
} else if (status == BT_DUPLICATE) {
    printf("Value already exists\n");
    free(val);  // Don't insert duplicates
}

// Find value
BTNode_t *found = btFind(tree, val);
if (found) {
    printf("Found: %d\n", *(int *)found->value);
}

// Clean up
btDestroyAll(tree);
```

## Hash Table Example
```c
#include "utils/include/hashTable.h"
#include <stdlib.h>
#include <string.h>

// Comparison function for string keys
int compareStrings(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

// Simple hash function for strings
unsigned long hashString(void *key) {
    unsigned long hash = 5381;
    const char *str = (const char *)key;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// Create hash table
HashTable_t *table = newHashTable(10, compareStrings, hashString);

// Insert key-value pairs
char *key1 = strdup("name");
char *val1 = strdup("Alice");
HT_STATUS status = htPut(table, key1, val1);
if (status == HT_OK) {
    printf("Inserted successfully\n");
} else if (status == HT_DUPLICATE) {
    printf("Key already exists\n");
    free(key1);
    free(val1);
}

char *key2 = strdup("age");
int *val2 = malloc(sizeof(int));
*val2 = 30;
htPut(table, key2, val2);

// Retrieve value
void *retrieved = NULL;
status = htGet(table, "name", &retrieved);
if (status == HT_OK) {
    printf("Name: %s\n", (char *)retrieved);
} else if (status == HT_NOT_FOUND) {
    printf("Key not found\n");
}

// Remove key-value pair
status = htRemove(table, "age");
if (status == HT_OK) {
    printf("Removed successfully\n");
    free(key2);
    free(val2);
}

// Clean up
htDestroyAll(table);
```

## AVL Binary Tree Example
```c
#include "utils/include/avlBinaryTree.h"
#include <stdlib.h>

// Comparison function for integers
int compareInts(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Create AVL tree
AVLBinaryTree_t *tree = newAVLBinaryTree(compareInts);

// Insert values (automatically balanced)
int *val1 = malloc(sizeof(int));
*val1 = 50;
AVLNode_t *node = NULL;
AVL_STATUS status = avlInsert(tree, val1, &node);
if (status == AVL_OK) {
    printf("Inserted successfully\n");
} else if (status == AVL_DUPLICATE) {
    printf("Value already exists\n");
    free(val1);
}

int *val2 = malloc(sizeof(int));
*val2 = 25;
avlInsert(tree, val2, &node);

int *val3 = malloc(sizeof(int));
*val3 = 75;
avlInsert(tree, val3, &node);

// Find value
AVLNode_t *found = avlFind(tree, val1);
if (found) {
    printf("Found: %d\n", *(int *)found->value);
}

// Remove value
status = avlRemove(tree, val1);
if (status == AVL_OK) {
    printf("Removed successfully\n");
    free(val1);
} else if (status == AVL_NOT_FOUND) {
    printf("Value not found\n");
}

// Clean up
avlDestroy(tree);
```
