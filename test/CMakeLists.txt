#  CMake file for local tests of Merge Sort
#  Author: Patrick Madlindl
#  May 2021


add_executable(runTest testMergeSort.cpp)

target_link_libraries(runTest PUBLIC
    mpiChallenge_lib
    gtest 
    gtest_main
    pthread
)

add_test(runTest runTest)