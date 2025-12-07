# 📦 Data Structures

## Stack (`stack.h`)

A Last-In-First-Out (LIFO) data structure with dynamic capacity.

### Type Definition
```c
typedef struct Stack Stack_t;
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

**`void sPush(Stack_t *stack, void *value)`**
- Adds a value to the top of the stack
- **Parameters**: 
  - `stack` - The Stack
  - `value` - Pointer to the value to add
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

### Type Definition
```c
typedef struct Stack BoolStack_t;
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

**`void bsPush(BoolStack_t *stack, int value)`**
- Adds a value to the top of the stack
- **Parameters**: 
  - `stack` - The BoolStack
  - `value` - The integer/boolean value to add

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

### Type Definition
```c
typedef struct Queue Queue_t;
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

**`void qAdd(Queue_t *queue, void *value)`**
- Adds a value to the back of the queue
- **Parameters**: 
  - `queue` - The Queue
  - `value` - Pointer to the value to add
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

### Type Definition
```c
typedef struct LinkedList LinkedList_t;
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

**`int llAddFirst(LinkedList_t *list, void *value)`**
- Adds a value at the start of the list
- **Parameters**: 
  - `list` - The LinkedList
  - `value` - Pointer to the value to add
- **Returns**: Status code
- **Note**: Caller manages memory for the value

**`int llAddLast(LinkedList_t *list, void *value)`**
- Adds a value at the end of the list
- **Parameters**: 
  - `list` - The LinkedList
  - `value` - Pointer to the value to add
- **Returns**: Status code
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

**`BTNode_t *btInsert(BinaryTree_t *tree, void *item)`**
- Inserts an item into the tree
- **Parameters**: 
  - `tree` - The Binary Tree
  - `item` - Pointer to the item to insert
- **Returns**: Pointer to the new node, or NULL on failure
- **Note**: Caller manages memory for the item

**`void *btRemove(BinaryTree_t *tree, const void *value)`**
- Removes an item from the tree
- **Parameters**: 
  - `tree` - The Binary Tree
  - `value` - Pointer to the value to remove
- **Returns**: Pointer to the removed value, or NULL if not found
- **Note**: Caller is responsible for freeing the returned value

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

## AVL Binary Tree (`avlBinaryTree.h`)

A self-balancing binary search tree that maintains O(log n) height through automatic rotations.

### Type Definitions
```c
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

**`AVLNode_t *avlInsert(AVLBinaryTree_t *tree, void *item)`**
- Inserts an item into the tree with automatic balancing
- **Parameters**: 
  - `tree` - The AVL Binary Tree
  - `item` - Pointer to the item to insert
- **Returns**: Pointer to the new node, or NULL on failure
- **Note**: Caller manages memory for the item

**`void *avlRemove(AVLBinaryTree_t *tree, const void *value)`**
- Removes an item from the tree with automatic rebalancing
- **Parameters**: 
  - `tree` - The AVL Binary Tree
  - `value` - Pointer to the value to remove
- **Returns**: Pointer to the removed value, or NULL if not found
- **Note**: Caller is responsible for freeing the returned value

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

// Create a stack for integers
Stack_t *stack = newStack(10);

// Push values
int *val = malloc(sizeof(int));
*val = 42;
sPush(stack, val);

// Pop value
int *retrieved = (int *)sPop(stack);
printf("Value: %d\n", *retrieved);
free(retrieved);

// Clean up
sDestroy(stack);
```

## BoolStack Example
```c
#include "utils/include/boolStack.h"

// Create a boolean stack
BoolStack_t *stack = newBoolStack(10);

// Push values
bsPush(stack, 1);
bsPush(stack, 0);
bsPush(stack, 1);

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
qAdd(queue, val);

// Remove value
int *retrieved = (int *)qRemove(queue);
free(retrieved);

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
llAddLast(list, val1);

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
btInsert(tree, val);

// Find value
BTNode_t *node = btFind(tree, val);
if (node) {
    printf("Found: %d\n", *(int *)node->value);
}

// Remove and clean up
void *removed = btRemove(tree, val);
free(removed);
btDestroy(tree);
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
avlInsert(tree, val1);

int *val2 = malloc(sizeof(int));
*val2 = 25;
avlInsert(tree, val2);

int *val3 = malloc(sizeof(int));
*val3 = 75;
avlInsert(tree, val3);

// Find value
AVLNode_t *node = avlFind(tree, val1);
if (node) {
    printf("Found: %d\n", *(int *)node->value);
}

// Remove and clean up
void *removed = avlRemove(tree, val1);
free(removed);
avlDestroy(tree);
