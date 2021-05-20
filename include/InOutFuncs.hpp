#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>


//currently, files we read in must have ";" as delimiter between words 
#define DELIMITER ";"


namespace IO {
    
    //TODO: add optional arg to write to file path if provided and not print out
    void printVector(std::shared_ptr<std::vector<std::string>> vec) {
        /*
        for (unsigned int i = 0; i < vec.size(); i++) {
            std::cout << vec[i] << EOL;
        }
        */
        std::stringstream outstream;
        //using iterators or short for loop
        for (auto& word : *vec) {
            outstream << word << std::endl;
        }

        std::cout << outstream.str();
    }



    std::vector<std::string> readStringsFromFile(std::string filename) {

       
        std::string inputString;
        std::vector<std::string> vec;

        std::ifstream inputFile;
        
        inputFile.open(filename);

        if(!inputFile.is_open()) {

            //could not open file
            // vec size is 0
            std::cout << "[IO] Could not open file." << std::endl;
            return vec;
        }

        //read in the file
        inputFile >> inputString;
        inputFile.close();

        size_t pos = 0;
        std::string substr;

        //split the file along defined delimiter
        while ((pos = inputString.find(DELIMITER)) != std::string::npos) {

            substr = inputString.substr(0, pos);
            vec.push_back(substr);
            inputString.erase(0, pos + 1);
        }

        //adding last element
        if (inputString.length() > 0) {
            vec.push_back(inputString);
        }
        
        return vec;
    }
    
} // namespace IO

