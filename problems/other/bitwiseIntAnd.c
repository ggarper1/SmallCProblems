#include "../../utils/include/random.h"
#include <math.h>
#include <stdio.h>

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a >= _b ? _a : _b;                                                        \
  })

int smarter(int N, int K) {
  if ((K - 1 | K) <= N) {
    return K - 1;
  }
  return K - 2;
}

int smart(int N, int K) {
  int current = K - 1;
  while (current > 0) {
    int shifted = current;
    int ctr = 0;
    while (shifted > 0) {
      if (!(shifted & 1)) {
        if (current + (int)pow(2, ctr) <= N) {
          return current;
        } else {
          break;
        }
      }
      shifted >>= 1;
      ctr++;
    }
    if (current + pow(2, ctr) <= N) {
      return current;
    }
    current--;
  }
  return 0;
}

int brute(int N, int K) {
  int a, b;
  int result = -1;
  for (int i = 1; i <= N; i++) {
    for (int j = i + 1; j <= N; j++) {
      int current = i & j;
      if (current < K) {
        result = max(result, i & j);
        a = i, b = j;
      }
    }
  }
  return result;
}

void runTests() {
  for (int i = 0; i < 10000; i++) {
    int n = randInt(2, 1000);
    int k = randInt(2, n);

    int smarterResult = smarter(n, k);
    int smartResult = smart(n, k);
    int bruteResult = brute(n, k);

    if (smartResult != bruteResult) {
      printf("🚨 Error for n=%d and k=%d\n\tReturned: %d\n\tActual: %d\n", n, k,
             smartResult, bruteResult);
      return;
    }
    if (smarterResult != bruteResult) {
      printf("🚨 (smarter) Error for n=%d and k=%d\n\tReturned: %d\n\tActual: "
             "%d\n",
             n, k, smarterResult, bruteResult);
      return;
    }
  }
  int smarterResult = smarter(2, 2);
  int smartResult = smart(2, 2);
  int bruteResult = brute(2, 2);

  if (smartResult != bruteResult) {
    printf("🚨 Error for n=2 and k=2\n\tReturned: %d\n\tActual: %d\n",
           smartResult, bruteResult);
    return;
  }

  if (smarterResult != bruteResult) {
    printf("🚨 (smarter) Error for n=2 and k=2\n\tReturned: %d\n\tActual: %d\n",
           smarterResult, bruteResult);
    return;
  }
  printf("✅ Tests passed!\n");
}

int main(void) { runTests(); }
