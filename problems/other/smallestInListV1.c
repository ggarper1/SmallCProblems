#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comapareIntsAsc(const void *a, const void *b) {
    const int *val_a = (const int *)a;
    const int *val_b = (const int *)b;

    int num_a = *val_a;
    int num_b = *val_b;

    if (num_a < num_b) {
        return -1;
    } else if (num_a > num_b) {
        return 1;
    } else {
        return 0;
    }
}

int smart(int list[], size_t length, int maxOperations) {
  if (maxOperations > 2) {
    return 0;
  } else {
    if (length < 2) {
      return list[0];
    }
 
    int sorted[length];

    memcpy(sorted, list, length * sizeof(int));

    qsort(
        sorted,
        length,
        sizeof(int),
        comapareIntsAsc
    );

    int min = list[1] - list[0]; 
    for (int i = 2; i < length; i++) {
      if (min > list[i] - list[i - 1]) {
          min = list[i] - list[i - 1];
      }
    }

    return min;
  }
}

int main(void) {

  int list[4] = {10, 30, 32, 43};

  int result = smart(list, 4, 4);
  printf("%d\n", result);

  result = smart(list, 4, 2);
  printf("%d\n", result);

  return 0;
}
