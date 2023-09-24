

#include <omp.h>
#include <stdio.h>
#include <unistd.h> // Include for the sleep function

#define NUM_THREADS 3
#define NUM_LOOPS 25
#define SLEEP_EVERY_N 3

int main() {
  int nStatic1[NUM_LOOPS];

  omp_set_num_threads(NUM_THREADS);

  // Parallel region begins here
#pragma omp parallel
  {
    // Parallel for loop with static scheduling and chunk size of 1
#pragma omp for schedule(guided)
    for (int i = 0; i < NUM_LOOPS; ++i) {
      if ((i % SLEEP_EVERY_N) == 0)
        sleep(1); // Sleep for 1 second on every third iteration
      nStatic1[i] = omp_get_thread_num();
    }
  } // Parallel region ends here

  // Print the values of nStatic1
  for (int i = 0; i < NUM_LOOPS; ++i) {
    printf("nStatic1[%d] = %d\n", i, nStatic1[i]);
  }

  return 0;
}
