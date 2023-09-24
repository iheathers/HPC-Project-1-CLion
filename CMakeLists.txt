cmake_minimum_required(VERSION 3.26)
project(Project1 C)

# Specify the path to GCC-13 (replace with your actual path)
set(CMAKE_C_COMPILER /opt/homebrew/bin/gcc-13)

set(CMAKE_C_STANDARD 11)

# Find OpenMP package
find_package(OpenMP REQUIRED)

add_executable(Project1
        parallel-implementation.c)

# Enable OpenMP support
if (OpenMP_C_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    target_link_libraries(Project1 PRIVATE ${OpenMP_C_LIBRARIES})
endif ()
