#include <omp.h>
#include <stdio.h>

int main() {
  int B = 0;

#pragma omp parallel for private(B)
  for (int i = 0; i < 10; i++) {
    B = 100;
    printf("B with thread %d is %d\n", omp_get_thread_num(), B);
  }

  printf("Outside %d\n", B); // B is not defined here (error: "B undeclared

  return 0;
}
