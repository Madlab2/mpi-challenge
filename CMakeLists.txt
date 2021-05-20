#  Top Level CMake file for MPI Challenge
#  Author: Patrick Madlindl
#  May 2021

cmake_minimum_required(VERSION 3.11)

project(CodingChallenge VERSION 0.1.0
        DESCRIPTION "Merge Sort MPI Challenge by Group 3"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

##############
# From gtest documentation: fetches GoogleTest and makes it available fot this project
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.10.0 
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
##############


include_directories(
    include
)


# adding openmpi and making it required
find_package(MPI REQUIRED)


#import test lib
add_subdirectory(test)


#add lib for io functions (header only, thus INTERFACE)
add_library(io_lib INTERFACE)

target_include_directories(io_lib INTERFACE
    include
)

#poject library that contains the src for the entire project
add_library(mpiChallenge_lib STATIC
    src/MergeSort.cpp
)


target_link_libraries(mpiChallenge_lib
    io_lib
    MPI::MPI_CXX    #links mpi to projec lib
)

add_executable(main 
    src/main.cpp
)

#the exec is linked to the src library
target_link_libraries(main
    mpiChallenge_lib
)