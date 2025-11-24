#include "../../utils/include/print.h"
#include "../../utils/include/random.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

int *subsetSum(int list[], size_t length, int target, int *subsetLength) {
  int biggestSum = 0;
  int smallestSum = 0;
  for (int i = 0; i < length; i++) {
    if (list[i] < 0) {
      smallestSum += list[i];
    } else {
      biggestSum += list[i];
    }
  }

  int idx = 0;
  int indices[length];
  bool targetSmaller = target <= smallestSum;
  bool targetBigger = target >= biggestSum;
  if (targetSmaller) {
    for (int i = 0; i < length; i++) {
      if (list[i] < 0) {
        indices[idx] = list[i];
        idx++;
      }
    }
  } else if (targetBigger) {
    for (int i = 0; i < length; i++) {
      if (list[i] > 0) {
        indices[idx] = list[i];
        idx++;
      }
    }
  }
  if (targetSmaller || targetBigger) {
    int *results = malloc(idx * sizeof(int));
    memcpy(results, indices, idx * sizeof(int));
    *subsetLength = idx;
    return results;
  }

  int cols = biggestSum - smallestSum + 1;
  int rows = length;
  bool dp[rows * cols];
  memset(dp, 0, rows * cols * sizeof(bool));

  for (int i = 0; i < cols; i++) {
    dp[i] = list[0] == i + smallestSum;
  }
  for (int j = 0; j < rows; j++) {
    dp[j * cols - smallestSum] = true;
  }

  for (int j = 1; j < rows; j++) {
    for (int i = 0; i < cols; i++) {
      dp[j * cols + i] = dp[(j - 1) * cols + i];
      if (i - list[j] < cols && i - list[j] > -1) {
        dp[j * cols + i] = dp[j * cols + i] || dp[(j - 1) * cols + i - list[j]];
      }
    }
  }

  int closestTarget;
  int i = 0;
  while (true) {
    if (dp[(length - 1) * cols - smallestSum + i + target]) {
      closestTarget = target + i;
      break;
    }

    if (dp[(length - 1) * cols - smallestSum - i + target]) {
      closestTarget = target - i;
      break;
    }
    i++;
  }

  idx = 0;
  int j = length - 1;
  i = closestTarget - smallestSum;
  while (j > 0) {
    if (i + smallestSum == 0) {
      break;
    } else if (i - list[j] > -1 && i - list[j] < cols &&
               dp[(j - 1) * cols + i - list[j]]) {
      indices[idx] = list[j];
      idx++;
      i -= list[j];
    }
    j--;
  }
  if (j == 0 && i + smallestSum != 0) {
    indices[idx] = list[j];
    idx++;
  }

  int *results = malloc(idx * sizeof(int));
  memcpy(results, indices, idx * sizeof(int));
  *subsetLength = idx;
  return results;
}

int bruteRecursive(int list[], size_t length, int target, int idx) {
  if (idx == length - 1) {
    int keepsCurrent = abs(target - list[idx]);
    return keepsCurrent < abs(target) ? list[idx] : 0;
  } else {
    int keepsCurrent =
        bruteRecursive(list, length, target - list[idx], idx + 1);
    int notKeepsCurrent = bruteRecursive(list, length, target, idx + 1);

    return abs(target - keepsCurrent - list[idx]) <
                   abs(target - notKeepsCurrent)
               ? keepsCurrent + list[idx]
               : notKeepsCurrent;
  }
}

int brute(int list[], size_t length, int target) {
  return bruteRecursive(list, length, target, 0);
}

void runTests() {
  int l = 6;
  for (int i = 0; i < 10000; i++) {
    int target = randInt(-25, 40);
    int list[7] = {randInt(-5, 5), randInt(-5, 5), randInt(-5, 5),
                   randInt(-5, 5), randInt(-5, 5), randInt(-5, 5),
                   randInt(-5, 5)};

    int bruteResult = brute(list, 7, target);
    int length;
    int *smartResult = subsetSum(list, 7, target, &length);

    int sum = 0;
    for (int i = 0; i < length; i++) {
      sum += smartResult[i];
    }

    if (abs(target - bruteResult) != abs(target - sum)) {
      printf("Error! target: %d,  list: ", target);
      printIntList(list, 7);
      printf("\tBrute result: %d\nSmart result: %d, ", bruteResult, sum);
      printIntList(smartResult, length);
      return;
    }
  }
  printf("All tests passed!\n");
}

int main(void) {
  srand(42);

  runTests();
  return 0;
}
