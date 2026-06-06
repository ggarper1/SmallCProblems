#include "../include/print.h"
#include <stdbool.h>
#include <stdio.h>

void printIntList(int *list, int length) {
  printf("[");
  for (int i = 0; i < length - 1; i++) {
    printf("%d, ", list[i]);
  }
  printf("%d]\n", list[length - 1]);
}
