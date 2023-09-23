
#include <omp.h>
#include <stdio.h>

#define N 5

void do_something(int i) {
  printf("Thread %d is processing element %d\n", omp_get_thread_num(), i);
}

int main() {

#pragma omp parallel default(none)
  {

//    Part 1: Loop worksharing construct
#pragma omp for
    for (int i = 0; i < N; i++) {
      do_something(i);

      printf("Inside: Thread %d is processing element %d\n",
             omp_get_thread_num(), i);
    }
  }

  return 0;
}
