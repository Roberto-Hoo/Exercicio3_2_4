cmake_minimum_required(VERSION 3.17)
project(Exercicio3_2_4)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror")

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpic++)

set(SOURCE_FILES E324.cpp)
add_executable(${CMAKE_PROJECT_NAME} E324.cpp)

# Rode com mpic++ E311.cpp -o E311
# depois mpirun -np 4 E311
# mpirun -np 8 -use-hwthread-cpus E311
# mpirun -np 9 -oversubscribe E311