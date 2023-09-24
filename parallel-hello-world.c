#include<stdio.h>
#include <omp.h>
int main()
{
    // it says give me a bunch of threads (if we don't say it, that will be a default number)
    #pragma omp parallel
    { 
        int ID=omp_get_thread_num();
        printf("hello: Thread (%d): \n", ID);
        printf("world: Thread (%d): \n", ID);
    }
}
 