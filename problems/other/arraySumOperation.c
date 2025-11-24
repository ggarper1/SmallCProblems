#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int readOutput(long **output) {
  FILE *file = fopen("output13.txt", "r");
  if (!file) {
    perror("Error opening file");
    return 1;
  }

  size_t capacity = 100000;
  size_t count = 0;
  *output = malloc(capacity * sizeof(long));
  if (!*output) {
    perror("Memory allocation failed");
    fclose(file);
    return 1;
  }

  // Read each number line by line
  while (1) {
    long value;
    if (fscanf(file, "%ld", &value) != 1) {
      break;
    }
    (*output)[count++] = value;
  }

  fclose(file);
  return 0;
}

long *performOperations(int N, int op_count, int *op, int *result_count) {
  long sum = (long)N * (N + 1) / 2 - N - 1;
  long first = 1, last = (long)N;

  long *results = malloc(op_count * sizeof(long));
  for (int i = 0; i < op_count; i++) {
    int opi = op[i];
    if ((opi >= 2 && opi <= N - 1) || opi == first || opi == last) {
      int intermediate = first;
      first = last;
      last = intermediate;
    } else {
      last = opi;
    }
    results[i] = sum + first + last;
  }
  return results;
}

int readInput(int *N, int *M, int **op) {
  FILE *file = fopen("input13.txt", "r");
  if (!file) {
    perror("Error opening file");
    return 1;
  }

  // Read first two numbers
  if (fscanf(file, "%d %d", N, M) != 2) {
    fprintf(stderr, "Error reading N and M\n");
    fclose(file);
    return 1;
  }

  size_t capacity = 100000;
  size_t count = 0;
  *op = malloc(capacity * sizeof(long));
  if (!*op) {
    perror("Memory allocation failed");
    fclose(file);
    return 1;
  }

  long value;
  while (fscanf(file, "%ld", &value) == 1) {
    (*op)[count++] = value;
  }

  fclose(file);
  return 0;
}

int runTest() {
  int N;
  int M;
  int *op;
  printf("Reading input\n");
  readInput(&N, &M, &op);

  int result_count;
  long *output;
  printf("Calculating...\n");
  long *result = performOperations(N, M, op, &result_count);

  printf("Reading output...\n");
  readOutput(&output);

  bool error = false;
  for (int i = 0; i < result_count; i++) {
    if (output[i] != result[i]) {
      error = true;
      break;
    }
  }
  if (error) {
    printf("Error, result is not valid\n");
  } else {
    printf("Test case passed!\n");
  }
  return 0;
}

int main() {
  runTest();
  return 0;
}
