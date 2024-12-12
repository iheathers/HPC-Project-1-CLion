```c
int main() {
   
   // CODE HERE

  for (int step = 0; step < NUM_SIMULATION_STEPS; step++) {

    // TODO: COMMENT OUT THE PRINTF STATEMENT FOR EXPERIMENT
    printf("Simulation Step: %d\n", step);
    printf("##################################################\n");
    simulationStep(fishes, NUM_FISHES);
    calculateObjectiveFunction(fishes, NUM_FISHES);
    calculateBarycenter(fishes, NUM_FISHES);
    printf("--------------------------------------------------\n");
  }

   // CODE HERE 
}

```

The above part does not need parallization because the steps of the simulation are inherently sequential and have
dependencies that prevent effective parallelization within the loop.

1. **Data Dependency**: In the given code, each step of the simulation (`simulationStep`), the calculation of the
   objective function (`calculateObjectiveFunction`), and the calculation of the barycenter (`calculateBarycenter`)
   depend on the results of the previous step. These calculations are inherently sequential and cannot be parallelized
   within the same simulation step.

2. **Sequential Dependence**: The simulation steps are sequential; each step depends on the state of the fishes after
   the previous step. Parallelizing the entire loop would require each step to be independent of the others, which is
   not the case here.

3. **Race Conditions**: If you were to parallelize the entire loop, you would introduce race conditions, as multiple
   threads would be trying to update and access the same `fishes` data concurrently. This would lead to unpredictable
   and incorrect results.

4. **Limited Parallelism**: Even if you attempted to parallelize certain parts of each step (e.g., parallelizing the
   swimming of individual fish within a step), it's unlikely that you would achieve significant speedup because the work
   within each step is relatively small compared to the overhead of thread management and synchronization.

5. **Overhead**: Introducing parallelism comes with its own overhead, such as thread creation and synchronization. In
   this case, the overhead may outweigh any potential performance gains from parallelization.

In summary, while parallelization can be beneficial for certain types of simulations and algorithms, it's not suitable
for every scenario. In this particular code, the steps of the simulation are inherently sequential and have dependencies
that prevent effective parallelization within the loop.

### defining schedule and chunk size

In OpenMP, you can set the chunk size and schedule dynamically by using environment variables. In a Setonix environment,
you can typically set these environment variables before running your OpenMP program. Here's how you can define the
chunk size and schedule using environment variables:

1. **Setting the Chunk Size:**

   You can set the chunk size using the `OMP_SCHEDULE` environment variable. The `OMP_SCHEDULE` environment variable has
   the following format:

   ```
   OMP_SCHEDULE="schedule_type,chunk_size"
   ```

    - `schedule_type` can be one of the following:
        - `"static"` for static scheduling
        - `"dynamic"` for dynamic scheduling
        - `"guided"` for guided scheduling
        - `"auto"` for automatic scheduling

    - `chunk_size` is an integer value representing the chunk size for static or dynamic scheduling. For guided
      scheduling, it represents the minimum chunk size.

   For example, to set a static schedule with a chunk size of 10:

   ```bash
   export OMP_SCHEDULE="static,10"
   ```

   To set a dynamic schedule with a chunk size of 5:

   ```bash
   export OMP_SCHEDULE="dynamic,5"
   ```

   To set a guided schedule with a minimum chunk size of 1:

   ```bash
   export OMP_SCHEDULE="guided,1"
   ```

   ```bash
   export OMP_SCHEDULE="auto"
   ```

2. **Running Your Program:**

   After setting the `OMP_SCHEDULE` environment variable, you can run your OpenMP program as usual. Your program will
   use the scheduling policy and chunk size specified in the environment variable.

   For example:

   ```bash
   ./your_openmp_program
   ```

3. **Unsetting the Environment Variable:**

   If you want to revert to the default scheduling behavior, you can unset the `OMP_SCHEDULE` environment variable:

   ```bash
   unset OMP_SCHEDULE
   ```

   This will cause OpenMP to use its default scheduling policy and chunk size.

Remember that the exact method for setting environment variables may vary depending on your specific Setonix
environment, so consult the documentation or environment-specific instructions for details on how to set environment
variables in your environment.

### OpenMP Scheduling

You do not need to specify `schedule(runtime)` explicitly if you want to use environmental variables
like `OMP_SCHEDULE` to control the scheduling behavior. When you set the `OMP_SCHEDULE` environment variable to specify
the scheduling policy and chunk size, OpenMP will automatically use those values for scheduling your parallel loops.

Here's how it works:

1. If you set the `OMP_SCHEDULE` environment variable with a valid schedule type and chunk size, such
   as `export OMP_SCHEDULE="static,10"`, OpenMP will use the specified scheduling policy (`static` in this case) and
   chunk size (`10`) for all parallel regions in your program.

2. You do not need to explicitly specify `schedule(runtime)` in your OpenMP pragmas when you use environmental variables
   like `OMP_SCHEDULE`. OpenMP will automatically consider the values from the environment variable.

3. When you unset the `OMP_SCHEDULE` environment variable or leave it undefined, OpenMP will revert to its default
   scheduling behavior, and you can use `schedule(static)`, `schedule(dynamic)`, etc., in your pragmas as needed.

In summary, environmental variables like `OMP_SCHEDULE` provide a way to control the scheduling behavior of your OpenMP
program without the need to specify `schedule(runtime)` explicitly in your code. OpenMP will use the values from the
environment variable when scheduling parallel loops.
