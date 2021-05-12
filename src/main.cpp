#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>


#include "MergeSort.hpp"
#include "InOutFuncs.hpp"



//TODO: use smart pointers instead of raw

int main() {

    auto mSort = std::make_unique<MergeSort>();
    

    std::string input;
    
    std::cout << "Enter a pah to a text file below or 'exit':" << std::endl;

    while(true) {

        std::getline(std::cin, input);

        if (input == "exit") {

            std::cout << "exiting.." << std::endl;
            break;

        } else {

            std::cout << "starting merge sort on file " << input << " ..." << std::endl;
            
            auto vec = std::make_shared<std::vector<std::string>>(IO::readStringsFromFile(input));

            mSort->mergeSort(std::move(vec), 0, vec->size() - 1);

            std::cout << "[Menu] finished! Result will be printed below: \n\n\n";

            IO::printVector(vec);

            //wil automatically delete vec a this point as going out of scope

        }

    }

}


