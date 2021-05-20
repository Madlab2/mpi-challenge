#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>

#include <chrono>
#include <mpi.h>

#include "MergeSort.hpp"
#include "InOutFuncs.hpp"



int main(int argc, char **argv) {
    
    //basic mpi functionality to demostrate it is working
    int rank;
    int size;
    int length;
    char name[80];

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(name, &length);

    //Some info output
    std::cout << "Hello, MPI! Rank: " << rank << " size " << size << " on " << name << std::endl;




    auto mSort = std::make_unique<MergeSort>();

    std::string input;
    
    //menu shall run only on ONE of all nodes (=> node 0)
    if(rank == 0) {

        while(true) {

            std::cout << "[Menu] Enter a path to a text file below or 'exit':" << std::endl;

            std::getline(std::cin, input);

            if (input == "exit") {

                std::cout << "[Menu] exiting.." << std::endl;
                break;

            } else {

                std::cout << "[Menu] Reading in file " << input << " ..." << std::endl;
                
                auto start = std::chrono::system_clock::now();
                auto vec = std::make_shared<std::vector<std::string>>(IO::readStringsFromFile(input));
                auto end = std::chrono::system_clock::now();

                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                std::cout << "[Menu] (time elapsed: " << elapsed.count() << "ms) Starting merge sort on file " << input << " ..." << std::endl;

                
                start = std::chrono::system_clock::now();
                mSort->mergeSort(vec, 0, vec->size() - 1);
                end = std::chrono::system_clock::now();

                elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                std::cout << "[Menu] (time elapsed: " << elapsed.count() << "ms) finished!" << std::endl;

                //IO::printVector(vec);
                //IO::printVectorToFile(vec);
                //wil automatically delete vec a this point as going out of scope

            }

        }
    }

    MPI_Finalize();

    return 0;

    

    

}


