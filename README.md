# High Performance Computing (HPC) Project 1

**GitHub Repository**: [HPC-Project-1-CLion](https://github.com/iheathers/HPC-Project-1-CLion)

## Overview

This project involves the development and performance analysis of a simulation program designed to replicate fish school behavior. The project explores both sequential and parallel implementations, focusing on the evaluation of execution time, speedup, and efficiency. The simulations were tested on the Setonix platform, a high-performance computing environment, to explore the impact of various thread scheduling methods and to understand the limitations of parallel computing.

## Project Structure

- **`sequential.c`**: Contains the sequential implementation of the fish school simulation.
- **`parallel-implementation.c`**: Contains the parallel implementation using OpenMP for multi-threading.
- **`CMakeLists.txt`**: The CMake configuration file to build the project.
- **`.clang-format`**: Defines the coding style guidelines for the project.
- **`.clang-tidy`**: Defines linting rules for code quality checks.
- **`Project 1 HPC Illustration.pdf`**: An illustrative guide related to the project.
- **`Project 1 Instruction v2.pdf`**: Instructions and guidelines provided for the project.
- **`Project1-Report-23771397-23308425.pdf`**: The final project report detailing the experiments, results, and analysis.

## Setup Instructions

### Prerequisites

- A C/C++ compiler supporting OpenMP (e.g., GCC).
- CMake (for building the project).

### Building the Project

1. Clone the repository to your local machine:
    ```sh
    git clone https://github.com/iheathers/HPC-Project-1-CLion
    ```
2. Navigate to the project directory:
    ```sh
    cd HPC-Project-1-CLion-main
    ```
3. Create a build directory and navigate into it:
    ```sh
    mkdir build && cd build
    ```
4. Run CMake to configure the project:
    ```sh
    cmake ..
    ```
5. Compile the project:
    ```sh
    make
    ```

### Running the Simulations

After building the project, you can run the simulation programs:

- For the sequential implementation:
    ```sh
    ./sequential
    ```

- For the parallel implementation:
    ```sh
    ./parallel-implementation
    ```

### Experimentation

The project report (`Project1-Report-23771397-23308425.pdf`) provides a detailed analysis of the experiments conducted on the Setonix platform. It includes variations in the number of threads, number of fishes, and simulation steps, and discusses the performance implications of different thread scheduling methods.

## Results

The report concludes that the parallel implementation faced challenges in achieving significant speedup due to parallelization overhead and other factors such as Amdahl's Law. The choice of scheduling method (auto, static, dynamic, or guided) did not significantly impact the overall performance. The experiments highlighted the complexities of parallel computing in a high-performance environment like Setonix.

