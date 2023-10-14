#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the total number of processes and the rank of the current process
    int numProcesses, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Print a "Hello, World!" message from each process
    printf("Hello, World! I am process %d out of %d processes.\n", rank, numProcesses);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
