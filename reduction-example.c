#include <omp.h>
#include <stdio.h>

int main() {
  int sum = 0;

  /*
   * Without reduction clause, there might be race-condition

   The reduction(+:sum) clause is added to the OpenMP directive. This clause
  indicates that a reduction operation will be performed on the variable sum. In
  this case, we are performing addition (+) to compute the sum.

  Inside the loop, each thread increments its local copy of sum with the value
  of i. The reduction clause ensures that the local sums from all threads are
  correctly combined to produce the final sum.
  */

  // Use OpenMP to parallelize the loop
#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < 10; i++) {
    sum += i;
    printf("Sum with thread %d is %d\n", omp_get_thread_num(), sum);
  }

  printf("Sum: %d\n", sum);

  return 0;
}
