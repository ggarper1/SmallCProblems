#include "../include/boolStack.h"
#include "../include/random.h"
#include "../include/stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int maxDataPoints = 10000;

void generateDataset(int **data) {
  *data = malloc(sizeof(int) * maxDataPoints);
  for (int i = 0; i < maxDataPoints; i++) {
    (*data)[i] = randInt(0, 1);
  }
}

void generatedBoolStackData(int *data, clock_t **times) {
  int numMeasurements = maxDataPoints / 10;
  *times = malloc(sizeof(clock_t) * numMeasurements);
  int timeIndex = 0;

  for (int i = 10; i <= maxDataPoints; i += 10) {
    clock_t time = 0;
    BoolStack_t *stack = newBoolStack(maxDataPoints);
    clock_t start, end;
    BS_STATUS status;

    for (int j = 0; j < i; j++) {
      int value = *(data + j);
      start = clock();
      status = bsPush(stack, value);
      end = clock();
      if (status == BS_ERROR) {
        printf("🚨 Error when pushing to linked list!\n");
        bsDestroy(stack);
        return;
      }
      time += end - start;
    }
    (*times)[timeIndex++] = time;
    bsDestroy(stack);
  }
}

void generatedStackData(int *data, clock_t **times) {
  int numMeasurements = maxDataPoints / 10;
  *times = malloc(sizeof(clock_t) * numMeasurements);
  int timeIndex = 0;

  for (int i = 10; i <= maxDataPoints; i += 10) {
    clock_t time = 0;
    Stack_t *stack = newStack(maxDataPoints);
    clock_t start, end;
    S_STATUS status;

    for (int j = 0; j < i; j++) {
      start = clock();
      status = sPush(stack, data + j);
      end = clock();
      if (status == S_ERROR) {
        printf("🚨 Error when pushing to stack!\n");
        sDestroy(stack);
        return;
      }
      time += end - start;
    }
    (*times)[timeIndex++] = time;
    sDestroy(stack);
  }
}

void writeData(clock_t *bsTimes, clock_t *sTimes) {
  FILE *file = fopen("./utils/benchmarks/data/boolStack.csv", "w");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  fprintf(file, "length,boolStack,arrayStack\n");
  int numMeasurements = maxDataPoints / 10;

  for (int i = 0; i < numMeasurements; i++) {
    long index_multiple = (long)(i + 1) * 10;
    fprintf(file, "%ld,%ld,%ld\n", index_multiple, (long)bsTimes[i],
            (long)sTimes[i]);
  }

  fclose(file);
  printf("Successfully wrote data to file\n");
}

void generateData() {
  int *data;
  generateDataset(&data);

  clock_t *bsTimes;
  generatedBoolStackData(data, &bsTimes);

  clock_t *sTimes;
  generatedStackData(data, &sTimes);

  writeData(bsTimes, sTimes);

  free(data);
  free(bsTimes);
  free(sTimes);
}

int main(int argc, char *argv[]) {
  generateData();
  return 0;
}
