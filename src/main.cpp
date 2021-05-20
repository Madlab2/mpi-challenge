#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>

#include <chrono>

#include "MergeSort.hpp"
#include "InOutFuncs.hpp"



//TODO: use smart pointers instead of raw

int main() {

    auto mSort = std::make_unique<MergeSort>();


    std::string input;
    
    

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


