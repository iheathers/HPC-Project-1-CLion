#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_FISHES 10

typedef struct {
  double x, y;
  double distanceTraveled;
  double weight;
} Fish;

void initializeInitialLakeState(Fish *fishes, int numFishes) {
  for (int i = 0; i < numFishes; i++) {
    fishes[i].x = ((double)rand() / RAND_MAX) * 200.0 - 100.0;
    fishes[i].y = ((double)rand() / RAND_MAX) * 200.0 - 100.0;
    fishes[i].distanceTraveled = 0.0;
    fishes[i].weight = 1.0;
  }
}

void writeFishDataToFile(const char *filename, Fish *fishes, int numFish) {
  FILE *file = fopen(filename, "w");
  if (file != NULL) {
    for (int i = 0; i < numFish; i++) {
      fprintf(file, "%.2f %.2f %.2f %.2f\n", fishes[i].x, fishes[i].y,
              fishes[i].distanceTraveled, fishes[i].weight);
    }
    fclose(file);
  } else {
    fprintf(stderr, "Error opening file for writing: %s\n", filename);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int numtasks, taskid;
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

  Fish *fishes;
  int numFishInWorker;
  int mtype;

  // Master generates and distributes fish data
  if (taskid == 0) {
    srand(time(NULL));
    fishes = (Fish *)malloc(NUM_FISHES * sizeof(Fish));

    if (fishes == NULL) {
      fprintf(stderr, "Memory allocation failed. Exiting...\n");
      MPI_Abort(MPI_COMM_WORLD, 1);
    }

    initializeInitialLakeState(fishes, NUM_FISHES);

    // Output initial fish data to a file
    writeFishDataToFile("fish_data_initial.txt", fishes, NUM_FISHES);

    // Calculate distribution
    numFishInWorker = NUM_FISHES / numtasks;
    int extra = NUM_FISHES % numtasks;
    int offset = 0;

    // Master sends fish data to all nodes (including itself)
    for (int dest = 0; dest < numtasks; dest++) {
      int numFishToDistribute = (dest < extra) ? numFishInWorker + 1 : numFishInWorker;

      mtype = 1;  // FROM_MASTER
      MPI_Send(&numFishToDistribute, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
      MPI_Send(fishes + offset, numFishToDistribute * sizeof(Fish), MPI_BYTE, dest, mtype, MPI_COMM_WORLD);

      offset += numFishToDistribute;
    }
  } else {
    // Workers receive fish data
    mtype = 1;  // FROM_MASTER
    for (int source = 0; source < numtasks; source++) {
      MPI_Recv(&numFishInWorker, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      fishes = (Fish *)malloc(numFishInWorker * sizeof(Fish));
      MPI_Recv(fishes, numFishInWorker * sizeof(Fish), MPI_BYTE, source, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


      // Send the received data back to the master
      mtype = 2;  // FROM_WORKER
      MPI_Send(&numFishInWorker, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
      MPI_Send(fishes, numFishInWorker * sizeof(Fish), MPI_BYTE, 0, mtype, MPI_COMM_WORLD);

      free(fishes);
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  // The master node receives data from workers
  if (taskid == 0) {
    Fish *masterFishes = (Fish *)malloc(NUM_FISHES * sizeof(Fish));
    int offset = 0;

    for (int source = 1; source < numtasks; source++) {
      // Receive fish data from workers
      mtype = 2;  // FROM_WORKER
      MPI_Recv(&numFishInWorker, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      fishes = (Fish *)malloc(numFishInWorker * sizeof(Fish));
      MPI_Recv(fishes, numFishInWorker * sizeof(Fish), MPI_BYTE, source, mtype, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // Process received data and write to file
      char recvFileName[50];
      snprintf(recvFileName, sizeof(recvFileName), "fish_data_received_%d.txt", source);
      writeFishDataToFile(recvFileName, fishes, numFishInWorker);

      // Accumulate data in the master array
      for (int i = 0; i < numFishInWorker; i++) {
        masterFishes[offset + i] = fishes[i];
      }

      offset += numFishInWorker;

      free(fishes);
    }

    // Write the master's data to a file
    writeFishDataToFile("fish_data_master.txt", masterFishes, NUM_FISHES);
    free(masterFishes);
  }

  MPI_Finalize();

  return 0;
}