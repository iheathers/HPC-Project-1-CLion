
#include <omp.h>
#include <stdio.h>

/*
#pragma omp for schedule(static) ordered: Within the parallel region, this
directive instructs OpenMP to parallelize the loop that follows.

The schedule(static) clause specifies that loop iterations should be statically
scheduled among the threads, meaning that each thread gets an approximately
equal number of iterations.

The ordered clause ensures that the printed output
maintains the original order of the loop.
*/

/*
Basic Functionality: The "ordered" clause is often used in conjunction with the
"for" or "parallel for" constructs in OpenMP. When applied to a loop construct,
it ensures that the iterations of the loop are executed in the order defined by
their indices, regardless of how threads are scheduled.
*/

// https://stackoverflow.com/questions/13224155/how-does-the-omp-ordered-clause-work

// https://www.ibm.com/docs/nl/zos/2.3.0?topic=processing-pragma-omp-ordered

void test(int first, int last) {
// #pragma omp parallel
#pragma omp parallel for schedule(static) ordered
  for (int i = first; i <= last; ++i) {

    printf("I am thread %d\n", omp_get_thread_num() + 1);

    if (i % 2) {
#pragma omp ordered
      printf("Thread %d - test() iteration %d\n", omp_get_thread_num() + 1, i);
    }
  }
}

int main() {
  omp_set_num_threads(3);
  test(1, 9);
}
