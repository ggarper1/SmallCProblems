#include "../../utils/include/random.h"
#include <stdio.h>
#include <stdlib.h>

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

int recursiveBruteForce(int deliveries1, int deliveries2, int charge1,
                        int charge2, int hour) {
  if (deliveries1 == 0 && deliveries2 == 0) {
    return hour - 1;
  } else if (hour % charge1 == 0 && hour % charge2 == 0) {
    return recursiveBruteForce(deliveries1, deliveries2, charge1, charge2,
                               hour + 1);
  } else if (hour % charge1 == 0) {
    if (deliveries2 > 0) {
      return recursiveBruteForce(deliveries1, deliveries2 - 1, charge1, charge2,
                                 hour + 1);
    } else {
      return recursiveBruteForce(deliveries1, deliveries2, charge1, charge2,
                                 hour + 1);
    }
  } else if (hour % charge2 == 0) {
    if (deliveries1 > 0) {
      return recursiveBruteForce(deliveries1 - 1, deliveries2, charge1, charge2,
                                 hour + 1);
    } else {
      return recursiveBruteForce(deliveries1, deliveries2, charge1, charge2,
                                 hour + 1);
    }
  } else {
    if (deliveries1 == 0) {
      return recursiveBruteForce(deliveries1, deliveries2 - 1, charge1, charge2,
                                 hour + 1);
    } else if (deliveries2 == 0) {
      return recursiveBruteForce(deliveries1 - 1, deliveries2, charge1, charge2,
                                 hour + 1);
    } else {
      return min(recursiveBruteForce(deliveries1 - 1, deliveries2, charge1,
                                     charge2, hour + 1),
                 recursiveBruteForce(deliveries1, deliveries2 - 1, charge1,
                                     charge2, hour + 1));
    }
  }
}

int bruteForce(int deliveries1, int deliveries2, int charge1, int charge2) {
  return recursiveBruteForce(deliveries1, deliveries2, charge1, charge2, 1);
}

int euclideanAlgorithm(int n1, int n2) {
  while (n1 > 0 && n2 > 0) {
    if (n1 > n2) {
      n1 = n1 % n2;
    } else if (n2 > n1) {
      n2 = n2 % n1;
    } else {
      return n1;
    }
  }

  return max(n1, n2);
}

int smart(int deliveries1, int deliveries2, int charge1, int charge2) {
  int chargeTime1 =
      deliveries1 / (charge1 - 1) - (deliveries1 % (charge1 - 1) == 0);
  int chargeTime2 =
      deliveries2 / (charge2 - 1) - (deliveries2 % (charge2 - 1) == 0);

  int gcd = euclideanAlgorithm(charge1, charge2);
  int lcm;

  if (gcd > 0) {
    lcm = (charge1 * charge2) / gcd;
  } else {
    lcm = deliveries1 * deliveries2;
  }

  int factor1 = (deliveries1 + chargeTime1) / lcm -
                ((deliveries1 + chargeTime1) % lcm == 0);
  if (chargeTime1 - factor1 > deliveries2) {
    return deliveries1 + chargeTime1;
  }

  int factor2 = (deliveries2 + chargeTime2) / lcm -
                ((deliveries2 + chargeTime2) % lcm == 0);
  if (chargeTime2 - factor2 > deliveries1) {
    return deliveries2 + chargeTime2;
  }

  int totalDeliveries = deliveries1 + deliveries2;
  return totalDeliveries + totalDeliveries / (lcm - 1) -
         (totalDeliveries % (lcm - 1) == 0);
}

void runTests() {
  int numTests = 10000;

  for (int i = 0; i < numTests; i++) {
    int deliveries1 = randInt(1, 10), deliveries2 = randInt(1, 10);
    int charge1 = randInt(2, 30), charge2 = randInt(2, 30);

    int smartResult = smart(deliveries1, deliveries2, charge1, charge2);
    int bruteResult = bruteForce(deliveries1, deliveries2, charge1, charge2);

    if (smartResult != bruteResult) {
      printf("❌ Error for input %d, %d, %d, %d:\n\tBrute: %d\n\tSmart: %d\n",
             deliveries1, deliveries2, charge1, charge2, bruteResult,
             smartResult);
      return;
    }
  }
  printf("✅ All tests passed!\n");
}

int main(void) {
  srand(42);

  runTests();

  return 0;
}
