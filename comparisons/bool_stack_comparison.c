#include "../modules/bool_stack/include/bool_stack.h"
#include "../modules/stack/include/stack.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#define DATA_DIR "./utils/benchmarks/data"
#define OUTPUT_FILE DATA_DIR "/boolStack.csv"

const int max_data_points = 10000;
const int step = 10;

int rand_int(int min, int max) {
  int range = max - min + 1;
  return (rand() % range) + min;
}

int *generate_dataset(void) {
  int *data = malloc(sizeof(int) * max_data_points);
  if (data == NULL) {
    return NULL;
  }

  for (int i = 0; i < max_data_points; i++) {
    data[i] = rand_int(0, 1);
  }
  return data;
}

/**
 * Measures the time to push the first n values (n = step, 2*step, ...) into a
 * fresh bool stack. Returns NULL on failure.
 */
clock_t *bool_stack_times(int *data) {
  int num_measurements = max_data_points / step;
  clock_t *times = malloc(sizeof(clock_t) * num_measurements);
  if (times == NULL) {
    return NULL;
  }

  for (int m = 0; m < num_measurements; m++) {
    int n = (m + 1) * step;
    bool_stack_t *stack = new_bool_stack(max_data_points);
    if (stack == NULL) {
      printf("🚨 Error when creating bool stack!\n");
      free(times);
      return NULL;
    }

    clock_t start = clock();
    for (int j = 0; j < n; j++) {
      if (bool_stack_push(stack, data[j]) == BOOL_STACK_ERROR) {
        printf("🚨 Error when pushing to bool stack!\n");
        bool_stack_destroy(stack);
        free(times);
        return NULL;
      }
    }
    times[m] = clock() - start;

    bool_stack_destroy(stack);
  }
  return times;
}

/**
 * Same measurement for the generic (pointer) stack. Pointers into `data` are
 * stored, so values are not destroyed with the stack.
 */
clock_t *stack_times(int *data) {
  int num_measurements = max_data_points / step;
  clock_t *times = malloc(sizeof(clock_t) * num_measurements);
  if (times == NULL) {
    return NULL;
  }

  for (int m = 0; m < num_measurements; m++) {
    int n = (m + 1) * step;
    my_stack_t *stack = new_stack(max_data_points);
    if (stack == NULL) {
      printf("🚨 Error when creating stack!\n");
      free(times);
      return NULL;
    }

    clock_t start = clock();
    for (int j = 0; j < n; j++) {
      if (stack_push(stack, data + j) == STACK_ERROR) {
        printf("🚨 Error when pushing to stack!\n");
        stack_destroy(stack, NULL);
        free(times);
        return NULL;
      }
    }
    times[m] = clock() - start;

    stack_destroy(stack, NULL);
  }
  return times;
}

int write_data(clock_t *bs_times, clock_t *s_times) {
  if (mkdir("./utils/benchmarks", 0755) != 0 && errno != EEXIST) {
    perror("Error creating benchmarks directory");
    return 0;
  }
  if (mkdir(DATA_DIR, 0755) != 0 && errno != EEXIST) {
    perror("Error creating data directory");
    return 0;
  }

  FILE *file = fopen(OUTPUT_FILE, "w");
  if (file == NULL) {
    perror("Error opening file");
    return 0;
  }

  fprintf(file, "length,boolStack,arrayStack\n");
  int num_measurements = max_data_points / step;
  for (int m = 0; m < num_measurements; m++) {
    fprintf(file, "%d,%ld,%ld\n", (m + 1) * step, (long)bs_times[m],
            (long)s_times[m]);
  }

  fclose(file);
  printf("Successfully wrote data to %s\n", OUTPUT_FILE);
  return 1;
}

int main(int argc, char *argv[]) {
  srand((unsigned)time(NULL));

  int *data = generate_dataset();
  if (data == NULL) {
    printf("🚨 Error when generating dataset!\n");
    return 1;
  }

  clock_t *bs_times = bool_stack_times(data);
  clock_t *s_times = stack_times(data);

  int ok = bs_times != NULL && s_times != NULL && write_data(bs_times, s_times);

  free(data);
  free(bs_times);
  free(s_times);
  return ok ? 0 : 1;
}
