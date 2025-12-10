#include "../include/linkedListStack.h"
#include "../include/random.h"
#include "../include/stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int maxDataPoints = 10000;

void generateDataset(int **data) {
  *data = malloc(sizeof(int) * maxDataPoints);
  for (int i = 0; i < maxDataPoints; i++) {
    (*data)[i] = randInt(0, 10000);
  }
}

void generatedLLStackData(int *data, clock_t **times) {
  int numMeasurements = maxDataPoints / 10;
  *times = malloc(sizeof(clock_t) * numMeasurements);
  int timeIndex = 0;

  for (int i = 10; i <= maxDataPoints; i += 10) {
    clock_t time = 0;
    LLStack_t *stack = newLLStack();
    clock_t start, end;
    LLS_STATUS status;

    for (int j = 0; j < i; j++) {
      start = clock();
      status = llsPush(stack, data + j);
      end = clock();
      if (status == LLS_ERROR) {
        printf("🚨 Error when pushing to linked list!\n");
        llsDestroy(stack);
        return;
      }
      time += end - start;
    }
    (*times)[timeIndex++] = time;
    llsDestroy(stack);
  }
}

void generatedStackData(int *data, clock_t **times) {
  int numMeasurements = maxDataPoints / 10;
  *times = malloc(sizeof(clock_t) * numMeasurements);
  int timeIndex = 0;

  for (int i = 10; i <= maxDataPoints; i += 10) {
    clock_t time = 0;
    Stack_t *stack = newStack(10);
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

void writeData(clock_t *llsTimes, clock_t *sTimes) {
  FILE *file = fopen("./utils/benchmarks/data/stack1.csv", "w");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  fprintf(file, "length,linkedListStack,arrayStack\n");
  int numMeasurements = maxDataPoints / 10;

  for (int i = 0; i < numMeasurements; i++) {
    long index_multiple = (long)(i + 1) * 10;
    fprintf(file, "%ld,%ld,%ld\n", index_multiple, (long)llsTimes[i],
            (long)sTimes[i]);
  }

  fclose(file);
  printf("Successfully wrote data to file\n");
}

void generateData() {
  int *data;
  generateDataset(&data);

  clock_t *llsTimes;
  generatedLLStackData(data, &llsTimes);

  clock_t *sTimes;
  generatedStackData(data, &sTimes);

  writeData(llsTimes, sTimes);

  free(data);
  free(llsTimes);
  free(sTimes);
}

int main(int argc, char *argv[]) {
  generateData();
  return 0;
}
