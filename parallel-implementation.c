// AUTHORS:
// PRITAM SUWAL SHRESTHA (23771397)
// RASPREET KHANUJA (23308425)

// TODO: ALWAYS SET THE NUMBER OF THREADS TO IN SETONIX:
// TODO: export OMP_NUM_THREADS=<NUMBER OF THREADS>

#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LAKE_X_MIN (-100.0) // Minimum x coordinate of the lake
#define LAKE_X_MAX 100.0    // Maximum x coordinate of the lake
#define LAKE_Y_MIN (-100.0) // Minimum y coordinate of the lake
#define LAKE_Y_MAX 100.0    // Maximum y coordinate of the lake

#define MIN_SWIM_DISTANCE (-0.1) // Minimum distance a fish can swim in one step
#define MAX_SWIM_DISTANCE 0.1    // Maximum distance a fish can swim in one step

#define MAX_FISH_WEIGHT 2.0 // Maximum weight of a fish
#define MIN_FISH_WEIGHT 0.0 // Minimum weight of a fish

// TODO: CHANGE THE NUMBER OF FISHES AND SIMULATION STEPS FOR EXPERIMENT
#define NUM_FISHES 1000
#define NUM_SIMULATION_STEPS 100 // Number of simulation steps

double square(double num) { return num * num; }

// Define fish structure and use it as alias
typedef struct {
  double x, y;             // Coordinates of a fish
  double distanceTraveled; // Distance traveled by a fish in one step
  double weight;           // Weight of a fish
} Fish;

// BOUNDARY CONDITION OF LAKE
bool isFishOutsideLake(Fish *fish) {
  return (fish->x < LAKE_X_MIN || fish->x > LAKE_X_MAX ||
          fish->y < LAKE_Y_MIN || fish->y > LAKE_Y_MAX);
}

double getRandomNumberInRange(double minValue, double maxValue) {
  // Generate a random double between minValue and maxValue
  double randomDouble = ((double)rand() / RAND_MAX);

  // Scale and shift the random double to the specified range
  double randomNumber = randomDouble * (maxValue - minValue) + minValue;

  return randomNumber;
}

double getRandomCoordinateInRange(double minValue, double maxValue) {
  return getRandomNumberInRange(minValue, maxValue);
}

double calculateDistanceFromOrigin(double x, double y) {
  return sqrt(square(x) + square(y));
}

// Function to calculate the objective function
double calculateObjectiveFunction(Fish *fishes, int numFishes) {
  double sum = 0.0;

#pragma omp parallel for reduction(+ : sum)

  for (int i = 0; i < numFishes; i++) {

    // sqrt(x^2 + y^2)
    sum += calculateDistanceFromOrigin((fishes + i)->x, (fishes + i)->y);
  }

  // TODO: COMMENT OUT THE PRINTF STATEMENT FOR EXPERIMENT
  printf("Objective Function: %.2f\n", sum);

  return sum;
}

// Function to simulate fish swimming
void swim(Fish *fish) {
  if (!isFishOutsideLake(fish)) {
    fish->x += getRandomNumberInRange(MIN_SWIM_DISTANCE, MAX_SWIM_DISTANCE);
    fish->y += getRandomNumberInRange(MIN_SWIM_DISTANCE, MAX_SWIM_DISTANCE);
  }
}

void updateWeight(Fish *fish, double maxDistanceTraveledInFishSchool) {
  double weightChange =
      fish->distanceTraveled / maxDistanceTraveledInFishSchool;

  if (fish->weight + weightChange < MIN_FISH_WEIGHT) {
    fish->weight = MIN_FISH_WEIGHT;
    return;
  }

  if (fish->weight + weightChange > MAX_FISH_WEIGHT) {
    fish->weight = MAX_FISH_WEIGHT;
    return;
  }

  fish->weight += weightChange;
}

void simulationStep(Fish *fishes, int numFishes) {

  double maxDistanceTraveledInFishSchool = 0.0;

  //  TODO: PRITAM SUWAL SHRESTHA (23771397)

#pragma omp parallel for reduction(max : maxDistanceTraveledInFishSchool)
  for (int i = 0; i < numFishes; i++) {
    double prevDistance =
        calculateDistanceFromOrigin((fishes + i)->x, (fishes + i)->y);

    swim(fishes + i);

    double nextDistance =
        calculateDistanceFromOrigin((fishes + i)->x, (fishes + i)->y);

    (fishes + i)->distanceTraveled = nextDistance - prevDistance;

    // Compare absolute value of distance traveled
    if (fabs((fishes + i)->distanceTraveled) >
        maxDistanceTraveledInFishSchool) {
      maxDistanceTraveledInFishSchool = fabs((fishes + i)->distanceTraveled);
    }
  }

// FIND: WHETHER WE NEED BARRIER HERE OR NOT

//  TODO: PRITAM SUWAL SHRESTHA (23771397)
#pragma omp parallel for
  for (int i = 0; i < numFishes; i++) {
    updateWeight((fishes + i), maxDistanceTraveledInFishSchool);

    // TODO: COMMENT OUT THE PRINTF STATEMENT FOR EXPERIMENT

    printf("Thread : %d -> Fish %d: x = %.2f, y = %.2f, distanceTraveled = "
           "%.2f, weight = %.2f\n",
           omp_get_thread_num(), i, (fishes + i)->x, (fishes + i)->y,
           (fishes + i)->distanceTraveled, (fishes + i)->weight);
  }
}

void calculateBarycenter(Fish *fishes, int numFishes) {
  double weightSum = 0.0;
  double distanceSum = 0.0;

  //  TODO: RASPREET KHANUJA (23308425)

#pragma omp parallel for reduction(+ : weightSum, distanceSum)
  for (int i = 0; i < numFishes; i++) {
    weightSum += (fishes + i)->weight *
                 calculateDistanceFromOrigin((fishes + i)->x, (fishes + i)->y);
    distanceSum +=
        calculateDistanceFromOrigin((fishes + i)->x, (fishes + i)->y);
  }

  // Check if distanceSum is non-zero to prevent division by zero
  if (distanceSum < 0.0) {
    printf("Distance sum is zero. Cannot calculate barycenter.\n");
    return;
  }

  double barycenter = weightSum / distanceSum;

  // TODO: COMMENT OUT THE PRINTF STATEMENT FOR EXPERIMENT
  printf("Barycenter: %.2f\n", barycenter);
}

// NOTE: CONSIDER BOUNDARY CONDITION WHERE FISH SHOULD NOT GO OUT OF BOUNDARY
void initializeInitialLakeState(Fish *fishes, int numFishes) {

//  TODO: RASPREET KHANUJA (23308425)
#pragma omp parallel for
  for (int i = 0; i < numFishes; i++) {
    (fishes + i)->x = getRandomCoordinateInRange(LAKE_X_MIN, LAKE_X_MAX);
    (fishes + i)->y = getRandomCoordinateInRange(LAKE_Y_MIN, LAKE_Y_MAX);
    (fishes + i)->distanceTraveled = 0.0;
    (fishes + i)->weight = 1.0; // You can set the initial weight here 'w'

    // TODO: COMMENT OUT THE PRINTF STATEMENT FOR EXPERIMENT
    printf(
        "Fish %d: x = %.2f, y = %.2f, distanceTraveled = %.2f, weight = %.2f\n",
        i, (fishes + i)->x, (fishes + i)->y, (fishes + i)->distanceTraveled,
        (fishes + i)->weight);
  }
}

int main() {
  // Seed the random number generator with the current time
  srand(time(NULL));

  Fish *fishes; // Declare a pointer to the structure Fish
  fishes = (Fish *)malloc(NUM_FISHES * sizeof(Fish));

  if (fishes == NULL) {
    fprintf(stderr, "Memory allocation failed. Exiting...\n");
    return 1; // Return a non-zero exit code to indicate an error
  }

  initializeInitialLakeState(fishes, NUM_FISHES);

  // Run the simulation
  double startTime = omp_get_wtime();

  //  CHECK WHETHER WE NEED PARALLIZATION HERE OR NOT

  for (int step = 0; step < NUM_SIMULATION_STEPS; step++) {

    // TODO: COMMENT OUT THE PRINTF STATEMENT FOR EXPERIMENT
    printf("Simulation Step: %d\n", step);
    printf("##################################################\n");
    simulationStep(fishes, NUM_FISHES);
    calculateObjectiveFunction(fishes, NUM_FISHES);
    calculateBarycenter(fishes, NUM_FISHES);
    printf("--------------------------------------------------\n");
  }

  double endTime = omp_get_wtime();

  double time_taken = endTime - startTime;
  printf("Parallel Elapsed time: %.4f seconds\n", time_taken);

  // Free allocated memory
  free(fishes);

  return 0;
}
