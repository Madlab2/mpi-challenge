### Message Passing Interface (MPI) Project

This is the repository for Group 3's solution to the mpi challenge of the lectures Informatik IV.

See instructions below for building and starting the appplication.

When you are in the programme, type `strg+c` or `exit` to leave.

#### Requirements
- CMake

### Instructions:

#### I) build the project:
    1. $ cd build
    2. $ cmake .. && make

#### II) run the tests:
    1. $ cd build
    2. $ ctest -VV

#### III) run application:
    1. $ cd build
    2. $ mpirun -np <num-nodes> ./main
    3. enter path to file, e.g. ../test/ExampleTexts/text2.txt

By default, the tests are built with the project. 

To skip building them, comment out `enable_testing()` in the project's CMakeLists.txt.
