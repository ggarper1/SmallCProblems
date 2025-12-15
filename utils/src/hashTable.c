#include "../include/hashTable.h"
#include "../include/avlBinaryTree.h"
#include "../include/stack.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct HashTable {
  size_t capacity;
  int length;
  AVLBinaryTree_t **buckets;
  int (*keyCompare)(const void *key1, const void *key2);
  unsigned long (*hash)(void *key);

} HashTable_t;

typedef struct Pair {
  void *key;
  void *value;

} Pair_t;

typedef enum { KEY, PAIR } ITEM_TYPE;

typedef union Pair_Or_Key {
  void *key;
  Pair_t *pair;
} Pair_Or_Key_t;

typedef struct Bucket_Item {
  ITEM_TYPE type;
  Pair_Or_Key_t data;
  int (*keyCompare)(const void *key1, const void *key2);
} Bucket_Item_t;

int innerCompare(const void *pairKey1, const void *pairKey2) {
  Bucket_Item_t *bi1 = (Bucket_Item_t *)pairKey1;
  Bucket_Item_t *bi2 = (Bucket_Item_t *)pairKey2;
  void *key1 = bi1->type == KEY ? bi1->data.key : bi1->data.pair->key;
  void *key2 = bi2->data.pair->key;
  return bi2->keyCompare(key1, key2);
}

int computeBucketIdx(HashTable_t *hashTable, void *key) {
  return (int)(hashTable->hash(key) % hashTable->capacity);
}

HT_STATUS innerPut(HashTable_t *hashTable, Bucket_Item_t *bi) {
  if (bi->type != PAIR) {
    return HT_ERROR;
  }
  int bucketIdx = computeBucketIdx(hashTable, bi->data.pair->key);

  AVLBinaryTree_t *tree = hashTable->buckets[bucketIdx];
  AVLNode_t *node;
  AVL_STATUS status = avlInsert(tree, bi, &node);
  switch (status) {
  case AVL_OK:
    return HT_OK;
  default:
    return HT_ERROR;
  }
}

HT_STATUS reallocateHashTable(HashTable_t *hashTable) {
  AVLBinaryTree_t **oldBuckets = hashTable->buckets;
  size_t oldCapacity = hashTable->capacity;

  hashTable->capacity *= 2;
  hashTable->buckets = malloc(sizeof(AVLBinaryTree_t *) * hashTable->capacity);
  if (hashTable->buckets == NULL) {
    hashTable->buckets = oldBuckets;
    hashTable->capacity /= 2;
    return HT_ERROR;
  }

  for (int i = 0; i < hashTable->capacity; i++) {
    hashTable->buckets[i] = newAVLBinaryTree(innerCompare);
    if (hashTable->buckets[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(hashTable->buckets[j]);
      }
      free(hashTable->buckets);
      hashTable->buckets = oldBuckets;
      hashTable->capacity /= 2;
      return HT_ERROR;
    }
  }

  // FIX: fix
  Stack_t *stack = newStack(21);
  for (int i = 0; i < oldCapacity; i++) {
    AVLBinaryTree_t *tree = oldBuckets[i];

    if (tree->length == 0) {
      continue;
    }

    sPush(stack, tree->root);
    while (sLength(stack) > 0) {
      AVLNode_t *node = sPop(stack);
      Bucket_Item_t *bi = node->value;
      HT_STATUS status = innerPut(hashTable, bi);
      if (status == HT_ERROR) {
        for (int j = 0; j < hashTable->capacity; j++) {
          free(hashTable->buckets[j]);
        }
        free(hashTable->buckets);
        hashTable->buckets = oldBuckets;
        hashTable->capacity /= 2;
        return HT_ERROR;
      }

      if (node->right != NULL) {
        sPush(stack, node->right);
      }
      if (node->left != NULL) {
        sPush(stack, node->left);
      }
    }
  }
  sDestroy(stack);
  for (int i = 0; i < oldCapacity; i++) {
    avlDestroy(oldBuckets[i]);
  }
  free(oldBuckets);
  return HT_OK;
}

// --- Public Function Prototypes ---
HashTable_t *newHashTable(size_t capacity,
                          int (*keyCompare)(const void *key1, const void *key2),
                          unsigned long (*hash)(void *key)) {
  HashTable_t *hashTable = malloc(sizeof(HashTable_t));
  if (hashTable == NULL) {
    return NULL;
  }

  hashTable->buckets = malloc(sizeof(AVLBinaryTree_t *) * capacity);
  if (hashTable->buckets == NULL) {
    free(hashTable);
    return NULL;
  }

  for (int i = 0; i < capacity; i++) {
    AVLBinaryTree_t *tree = newAVLBinaryTree(innerCompare);
    if (tree == NULL) {
      for (int j = 0; j < i; j++) {
        free(hashTable->buckets[j]);
      }
      free(hashTable->buckets);
      free(hashTable);
      return NULL;
    }
    hashTable->buckets[i] = tree;
  }

  hashTable->capacity = capacity;
  hashTable->length = 0;
  hashTable->keyCompare = keyCompare;
  hashTable->hash = hash;

  return hashTable;
}

int htLength(HashTable_t *hashTable) { return hashTable->length; }

HT_STATUS htPut(HashTable_t *hashTable, void *key, void *value) {
  if (hashTable->length / hashTable->capacity > 10) {
    HT_STATUS status = reallocateHashTable(hashTable);
    if (status == HT_ERROR) {
      return HT_ERROR;
    }
  }
  int bucketIdx = computeBucketIdx(hashTable, key);

  Pair_t *pair = malloc(sizeof(Pair_t));
  pair->key = key;
  pair->value = value;
  if (pair == NULL) {
    return HT_ERROR;
  }
  Bucket_Item_t *bi = malloc(sizeof(Bucket_Item_t));
  if (bi == NULL) {
    free(pair);
    return HT_ERROR;
  }
  bi->type = PAIR;
  bi->data = (Pair_Or_Key_t)pair;
  bi->keyCompare = hashTable->keyCompare;

  AVLBinaryTree_t *tree = hashTable->buckets[bucketIdx];

  AVLNode_t *node;
  AVL_STATUS status = avlInsert(tree, bi, &node);
  switch (status) {
  case AVL_OK:
    hashTable->length++;
    return HT_OK;
  case AVL_DUPLICATE:
    free(pair);
    free(bi);
    return HT_DUPLICATE;
  default:
    free(pair);
    free(bi);
    return HT_ERROR;
  }
}

HT_STATUS htGet(HashTable_t *hashTable, void *key, void **value) {
  int bucketIdx = computeBucketIdx(hashTable, key);

  Bucket_Item_t *bi = &(Bucket_Item_t){.type = KEY, .data = (Pair_Or_Key_t)key};

  AVLBinaryTree_t *tree = hashTable->buckets[bucketIdx];
  AVLNode_t *node = avlFind(tree, bi);
  if (node == NULL) {
    return HT_NOT_FOUND;
  } else {
    Bucket_Item_t *bi = node->value;
    if (bi->type != PAIR) {
      return HT_ERROR;
    }
    *value = bi->data.pair->value;
    return HT_OK;
  }
}

HT_STATUS htRemove(HashTable_t *hashTable, void *key) {
  int bucketIdx = computeBucketIdx(hashTable, key);

  Bucket_Item_t *bi = &(Bucket_Item_t){.type = KEY, .data = (Pair_Or_Key_t)key};

  AVLBinaryTree_t *tree = hashTable->buckets[bucketIdx];
  AVL_STATUS status = avlRemove(tree, bi);
  switch (status) {
  case AVL_OK:
    return HT_OK;
  case AVL_NOT_FOUND:
    return HT_NOT_FOUND;
  default:
    return HT_ERROR;
  }
}

void htDestroy(HashTable_t *hashTable) {
  for (int i = 0; i < hashTable->capacity; i++) {
    avlDestroy(hashTable->buckets[i]);
  }
  free(hashTable->buckets);
  free(hashTable);
}

void htDestroyAll(HashTable_t *hashTable) {
  for (int i = 0; i < hashTable->capacity; i++) {
    avlDestroyAll(hashTable->buckets[i]);
  }
  free(hashTable->buckets);
  free(hashTable);
}

void printHashTable(HashTable_t *hashTable,
                    void (*reprKey)(void *value, char *buffer, int bufferSize),
                    int keyBufferSize,
                    void (*reprValue)(void *value, char *buffer,
                                      int bufferSize),
                    int valueBufferSize) {

  if (hashTable->length == 0) {
    printf("Empty hashtable\n");
  }

  char *keyBuffer = malloc(keyBufferSize * sizeof(char));
  char *valueBuffer = malloc(valueBufferSize * sizeof(char));
  for (int i = 0; i < hashTable->capacity; i++) {
    if (i == hashTable->capacity / 2) {
      printf("Hashtable:\t|");
    } else {
      printf("\t\t|");
    }

    AVLBinaryTree_t *tree = hashTable->buckets[i];
    if (tree->length == 0) {
      printf("\n");
      continue;
    }
    Bucket_Item_t *bi = tree->root->value;
    reprKey(bi->data.pair->key, keyBuffer, keyBufferSize);
    reprValue(bi->data.pair->value, valueBuffer, valueBufferSize);
    printf(" [(%s, %s)", keyBuffer, valueBuffer);

    // FIX: fix
    Stack_t *stack = newStack(21);
    if (tree->root->right != NULL) {
      sPush(stack, tree->root->right);
    }
    if (tree->root->left != NULL) {
      sPush(stack, tree->root->left);
    }

    while (sLength(stack) > 0) {
      AVLNode_t *node = sPop(stack);
      Bucket_Item_t *bi = node->value;

      reprKey(bi->data.pair->key, keyBuffer, keyBufferSize);
      reprValue(bi->data.pair->value, valueBuffer, valueBufferSize);
      printf(", (%s, %s)", keyBuffer, valueBuffer);

      if (node->right != NULL) {
        sPush(stack, node->right);
      }
      if (node->left != NULL) {
        sPush(stack, node->left);
      }
    }
    sDestroy(stack);
    printf("]\n");
  }
  free(keyBuffer);
  free(valueBuffer);
}
