#include <omp.h>
#include <stdio.h>

/*
 Code Block 1 uses the #pragma omp for directive for automatic loop work
 sharing, where the OpenMP runtime automatically distributes loop iterations
 among the threads.
 */

#define N 100

int main() {
  int a[N], b[N], c[N];

  omp_set_num_threads(4);

  // Initialize arrays a and b
  for (int i = 0; i < N; i++) {
    a[i] = i;
    b[i] = 2 * i;
  }

  //  the standard way of parallelizing loops with OpenMP. This directive
  //  combines the parallel region creation and loop work sharing into a single
  //  construct.

#pragma omp parallel for
  for (int i = 0; i < N; i++) {
    c[i] = a[i] + b[i];
    printf("Inside: Thread %d is processing element %d\n", omp_get_thread_num(),
           i);
  }

  return 0;
}
