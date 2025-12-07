#include "../include/linkedList.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  struct Node *next;
  struct Node *prev;
  void *value;
} Node_t;

typedef struct LinkedList {
  Node_t *first;
  Node_t *last;
  int length;
} LinkedList_t;

// --- Public Function Prototypes ---
LinkedList_t *newLinkedList() {
  LinkedList_t *list = malloc(sizeof(LinkedList_t));
  if (list == NULL) {
    return NULL;
  }

  list->length = 0;
  list->first = NULL;
  list->last = NULL;

  return list;
}

int llLength(LinkedList_t *list) { return list->length; }

void *llPeekFirst(LinkedList_t *list) {
  if (list->length == 0) {
    return NULL;
  }
  return list->first->value;
}

void *llPeekLast(LinkedList_t *list) {
  if (list->length == 0) {
    return NULL;
  }
  return list->last->value;
}

int llAddLast(LinkedList_t *list, void *value) {
  Node_t *node = malloc(sizeof(Node_t));
  if (node == NULL) {
    return 0;
  }

  node->value = value;
  node->next = NULL;

  if (list->length == 0) {
    node->prev = NULL;
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }
  list->length++;
  return 1;
}

int llAddFirst(LinkedList_t *list, void *value) {
  Node_t *node = malloc(sizeof(Node_t));
  if (node == NULL) {
    return 0;
  }

  node->value = value;
  node->prev = NULL;

  if (list->length == 0) {
    node->next = NULL;
    list->first = node;
    list->last = node;
  } else {
    list->first->prev = node;
    node->next = list->first;
    list->first = node;
  }
  list->length++;
  return 1;
}

void *llPopFirst(LinkedList_t *list) {
  if (list->length == 0) {
    return NULL;
  }

  void *value = list->first->value;
  if (list->first->next == NULL) {
    free(list->first);
    list->first = NULL;
    list->last = NULL;
  } else {
    list->first = list->first->next;
    free(list->first->prev);
    list->first->prev = NULL;
  }

  list->length--;

  return value;
}

void *llPopLast(LinkedList_t *list) {
  if (list->length == 0) {
    return NULL;
  }

  void *value = list->last->value;
  if (list->last->prev == NULL) {
    free(list->last);
    list->first = NULL;
    list->last = NULL;
  } else {
    list->last = list->last->prev;
    free(list->last->next);
    list->last->next = NULL;
  }
  list->length--;
  return value;
}

void llDestroy(LinkedList_t *list) {
  Node_t *node = list->first;
  Node_t *nextNode;
  for (int i = 0; i < list->length; i++) {
    nextNode = node->next;
    free(node);
    node = nextNode;
  }
  free(list);
}

void llDestroyAll(LinkedList_t *list) {
  Node_t *node = list->first;
  Node_t *nextNode;
  for (int i = 0; i < list->length; i++) {
    nextNode = node->next;
    free(node->value);
    free(node);
    node = nextNode;
  }
  free(list);
}

void printLinkedList(LinkedList_t *list,
                     void (*repr)(void *value, char *buffer, int bufferSize),
                     int bufferSize) {
  if (list->length == 0) {
    printf("[ ]\n");
    return;
  }

  Node_t *node = list->first;
  char *buffer = malloc(sizeof(char) * bufferSize);
  repr(node->value, buffer, bufferSize);
  printf("[ %s", buffer);
  for (int i = 1; i < list->length; i++) {
    node = node->next;
    // printf("\nnode==NULL: %d, i: %d, le: %d\n", node == NULL, i,
    // list->length);
    repr(node->value, buffer, bufferSize);
    printf(", %s", buffer);
  }
  printf(" ]\n");
  free(buffer);
}
