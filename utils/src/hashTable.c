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

typedef enum { KEY, PAIR } ITEM_TYPE;

typedef struct Bucket_Item {
  void *key;
  void *value;
  int (*keyCompare)(const void *key1, const void *key2);
} Bucket_Item_t;

int innerCompare(const void *pairKey1, const void *pairKey2) {
  Bucket_Item_t *bi1 = (Bucket_Item_t *)pairKey1;
  Bucket_Item_t *bi2 = (Bucket_Item_t *)pairKey2;
  return bi2->keyCompare(bi1->key, bi2->key);
}

int computeBucketIdx(HashTable_t *hashTable, void *key) {
  return (int)(hashTable->hash(key) % hashTable->capacity);
}

HT_STATUS innerPut(HashTable_t *hashTable, Bucket_Item_t *bi) {
  int bucketIdx = computeBucketIdx(hashTable, bi->key);

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

void removeBucketItem(void *bi) {
  Bucket_Item_t *b = (Bucket_Item_t *)bi;
  free(b->key);
  free(b->value);
  free(b);
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
    hashTable->buckets[i] = newAVLBinaryTree(innerCompare, &removeBucketItem);
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
    AVLBinaryTree_t *tree = newAVLBinaryTree(innerCompare, &removeBucketItem);
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

  Bucket_Item_t *bi = malloc(sizeof(Bucket_Item_t));
  if (bi == NULL) {
    return HT_ERROR;
  }
  bi->key = key;
  bi->value = value;
  bi->keyCompare = hashTable->keyCompare;

  AVLBinaryTree_t *tree = hashTable->buckets[bucketIdx];

  AVLNode_t *node;
  AVL_STATUS status = avlInsert(tree, bi, &node);
  switch (status) {
  case AVL_OK:
    hashTable->length++;
    return HT_OK;
  case AVL_DUPLICATE:
    free(bi);
    return HT_DUPLICATE;
  default:
    free(bi);
    return HT_ERROR;
  }
}

HT_STATUS htGet(HashTable_t *hashTable, void *key, void **value) {
  int bucketIdx = computeBucketIdx(hashTable, key);

  Bucket_Item_t *bi = &(Bucket_Item_t){.key = key};

  AVLBinaryTree_t *tree = hashTable->buckets[bucketIdx];
  AVLNode_t *node = avlFind(tree, bi);
  if (node == NULL) {
    return HT_NOT_FOUND;
  } else {
    Bucket_Item_t *bi = node->value;
    *value = bi->value;
    return HT_OK;
  }
}

HT_STATUS htRemove(HashTable_t *hashTable, void *key, void **pair) {
  int bucketIdx = computeBucketIdx(hashTable, key);

  Bucket_Item_t *bi = &(Bucket_Item_t){.key = key};

  AVLBinaryTree_t *tree = hashTable->buckets[bucketIdx];
  Bucket_Item_t *toRemove;
  AVL_STATUS status = avlRemove(tree, bi, (void **)&toRemove);
  switch (status) {
  case AVL_OK:
    *pair = malloc(sizeof(Pair_t));
    ((Pair_t *)*pair)->key = toRemove->key;
    ((Pair_t *)*pair)->value = toRemove->value;
    free(toRemove);
    return HT_OK;
  case AVL_NOT_FOUND:
    return HT_NOT_FOUND;
  default:
    return HT_ERROR;
  }
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
    reprKey(bi->key, keyBuffer, keyBufferSize);
    reprValue(bi->value, valueBuffer, valueBufferSize);
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

      reprKey(bi->key, keyBuffer, keyBufferSize);
      reprValue(bi->value, valueBuffer, valueBufferSize);
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
