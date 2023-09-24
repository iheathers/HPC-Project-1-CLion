#include <omp.h>
#include <stdio.h>

int main() {
  int tmp = 0;

#pragma omp parallel for firstprivate(tmp)
  for (int i = 0; i < 10; i++) {
    //    tmp += i;
    printf("tmp with thread %d is %d\n", omp_get_thread_num(), tmp);
  }

  printf("Outside %d\n", tmp); // B is not defined here (error: "B undeclared
}
