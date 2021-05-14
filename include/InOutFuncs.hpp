#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>


//; --> csv-files?
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

    void printVectorToFile(std::shared_ptr<std::vector<std::string>> vec) {
        std::ofstream outputFile;
        outputFile.open("output.txt");
        if (!outputFile.is_open()) {
            std::cout << "[IO] Could not open output file." << std::endl;
            return;
        }
        for (const auto& value : *vec) {
            outputFile << value << DELIMITER;
        }
        outputFile.close();
        return;
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


        inputFile >> inputString;
        inputFile.close();

        size_t pos = 0;
        std::string substr;
        const char* delimiter = DELIMITER;

        std::string token;
        std::istringstream tokenStream(inputString);
        while (std::getline(tokenStream, token, *delimiter))
        {
            vec.push_back(token);
        }
        return vec;
    }
    
} // namespace IO

