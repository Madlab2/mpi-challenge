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
        
        std::cout << "[Menu] Enter a pah to a text file below or 'exit':" << std::endl;

        std::getline(std::cin, input);

        if (input == "exit") {

            std::cout << "[Menu] exiting.." << std::endl;
            break;

        } else {

            std::cout << "[Menu] Reading in file " << input << " ..." << std::endl;
            
            auto vec = std::make_shared<std::vector<std::string>>(IO::readStringsFromFile(input));

            std::cout << "[Menu] starting merge sort on file " << input << " ..." << std::endl;

            //TODO: start time measurement
            mSort->mergeSort(vec, 0, vec->size() - 1);

            std::cout << "[Menu] finished! Result will be printed below: \n\n\n";

            IO::printVector(vec);

            //wil automatically delete vec a this point as going out of scope

        }

    }

}


