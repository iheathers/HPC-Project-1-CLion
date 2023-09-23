#include <omp.h>
#include <stdio.h>

#define N 16

int main() {
  int a[N], b[N], c[N];

  omp_set_num_threads(3);

  // Initialize arrays a and b
  for (int i = 0; i < N; i++) {
    a[i] = i;
    b[i] = 2 * i;
  }

#pragma omp parallel default(none) shared(a, b, c)
  {
    int id = omp_get_thread_num();
    int Nthrds = omp_get_num_threads();
    int istart = id * N / Nthrds;
    int iend = (id + 1) * N / Nthrds;

    if (id == Nthrds - 1) {
      iend = N;
    }

    printf("Thread %d: istart = %d, iend = %d\n", id, istart, iend);
    printf("--------------------\n");

    // Parallel computation of c[i] = a[i] + b[i]
    for (int i = istart; i < iend; i++) {
      c[i] = a[i] + b[i];
      printf("Thread %d , id = %d\n", omp_get_thread_num(), id);
      printf("a[%d] = %d + b[%d] = %d -> c[%d] = %d\n", i, a[i], i, b[i], i,
             c[i]);
    }

    printf("####################\n");
  }

  return 0;
}
