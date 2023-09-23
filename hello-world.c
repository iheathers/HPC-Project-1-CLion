
#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("OpenMP running with %d threads\n", omp_get_max_threads());

#pragma omp parallel default(none)
  { printf("Hello World from thread %d\n", omp_get_thread_num()); }

  return 0;
}
