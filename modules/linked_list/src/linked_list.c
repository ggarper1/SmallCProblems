#include "../include/linked_list.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  struct node *next;
  struct node *prev;
  void *value;
} node_t;

typedef struct linked_list_t {
  node_t *first;
  node_t *last;
  size_t size;
} linked_list_t;

linked_list_t *new_linked_list() {
  linked_list_t *list = malloc(sizeof(linked_list_t));
  if (list == NULL) {
    return NULL;
  }

  list->size = 0;
  list->first = NULL;
  list->last = NULL;

  return list;
}

size_t linked_list_size(linked_list_t *list) { return list->size; }

linked_list_result_t linked_list_peek_first(linked_list_t *list) {
  linked_list_result_t result;
  if (list->size == 0) {
    result.status = LINKED_LIST_EMPTY;
    result.value = NULL;
    return result;
  }
  result.status = LINKED_LIST_OK;
  result.value = list->first->value;
  return result;
}

linked_list_result_t linked_list_peek_last(linked_list_t *list) {
  linked_list_result_t result;
  if (list->size == 0) {
    result.status = LINKED_LIST_EMPTY;
    result.value = NULL;
    return result;
  }
  result.status = LINKED_LIST_OK;
  result.value = list->last->value;
  return result;
}

LINKED_LIST_STATUS linked_list_add_last(linked_list_t *list, void *value) {
  node_t *node = malloc(sizeof(node_t));
  if (node == NULL) {
    return LINKED_LIST_ERROR;
  }

  node->value = value;
  node->next = NULL;

  if (list->size == 0) {
    node->prev = NULL;
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }

  list->size++;
  return LINKED_LIST_OK;
}

LINKED_LIST_STATUS linked_list_add_first(linked_list_t *list, void *value) {
  node_t *node = malloc(sizeof(node_t));
  if (node == NULL) {
    return LINKED_LIST_ERROR;
  }

  node->value = value;
  node->prev = NULL;

  if (list->size == 0) {
    node->next = NULL;
    list->first = node;
    list->last = node;
  } else {
    list->first->prev = node;
    node->next = list->first;
    list->first = node;
  }

  list->size++;
  return LINKED_LIST_OK;
}

linked_list_result_t linked_list_pop_first(linked_list_t *list) {
  linked_list_result_t result;
  if (list->size == 0) {
    result.status = LINKED_LIST_EMPTY;
    result.value = NULL;
    return result;
  }

  result.value = list->first->value;
  if (list->first->next == NULL) {
    free(list->first);
    list->first = NULL;
    list->last = NULL;
  } else {
    list->first = list->first->next;
    free(list->first->prev);
    list->first->prev = NULL;
  }
  list->size--;

  result.status = LINKED_LIST_OK;
  return result;
}

linked_list_result_t linked_list_pop_last(linked_list_t *list) {
  linked_list_result_t result;
  if (list->size == 0) {
    result.status = LINKED_LIST_EMPTY;
    result.value = NULL;
    return result;
  }

  result.value = list->last->value;
  if (list->last->prev == NULL) {
    free(list->last);
    list->first = NULL;
    list->last = NULL;
  } else {
    list->last = list->last->prev;
    free(list->last->next);
    list->last->next = NULL;
  }
  list->size--;

  result.status = LINKED_LIST_OK;
  return result;
}

void linked_list_destroy(linked_list_t *list, void (*destroy_value)(void *)) {
  node_t *node = list->first;
  node_t *nextNode;
  for (int i = 0; i < list->size; i++) {
    nextNode = node->next;
    if (destroy_value != NULL) {
      destroy_value(node->value);
    }
    free(node);
    node = nextNode;
  }
  free(list);
}
