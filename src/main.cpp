#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>

#include <chrono>
#include <mpi.h>

#include "MergeSort.hpp"
#include "InOutFuncs.hpp"


//TODO
//gets a size of a one-dimensional set and the number of substets (of similar sizes) the set shall be splittet up into
//returns vector of pairs containing begin and end indices of every subset
std::vector<std::pair<size_t, size_t>> split_even(int size_to_split, int num_splits) {

    //from stackoverflow
    /*
    for(size_t i = 0; i < vec.size(); i += world_size) {

        auto last = std::min(vec.size(), i + world_size);
        
        auto index = i / world_size;
        auto& vec = bunches[index];

        vec.reserve(last - i);

        std::move(strings.begin() + i, strings.begin() + last, std::back_inserter(vec));

    }
    */
    

    //eg. for 5 Elements splitted in two parts: {(0, 1), (2,4)}
    std::vector<std::pair<size_t, size_t>> result;

    return result;
}


void slaveMerge(std::shared_ptr<MPI_Status> status, std::shared_ptr<MPI_Rank> rank, std::shared_ptr<int> world_size) {

    auto mSort = std::make_unique<MergeSort>();

    int length;

    //reveive length of vector from master
    MPI_Recv(&length, 1, MPI_INT, 0, 666, MPI_COMM_WORLD, &status);
    
    //construct vector with given length and a ptr to it
    std::vector<std::string> vec(length);

    auto vec_ptr = std::make_shared(vec);

    //reveive vector from master
    //vec_ptr points to first element
    MPI_Recv(vec_ptr, length, MPI_CHAR, 0, 777, MPI_COMM_WORLD, &status);

    //merge sort the vector received from master
    mSort->mergeSort(vec_ptr, 0, length - 1);

    //send back the sorted vector to master
    //dereference vec_ptr gives content of vec at element 0
    MPI_Send(*vec_ptr, length, MPI_INT, 0, 777, MPI_COMM_WORLD);

}


std::shared_ptr<std::vector<std::string>> masterMerge(std::shared_ptr<std::vector<std::string>> vec_to_merge, 
    std::shared_ptr<MPI_Status> status, std::shared_ptr<MPI_Rank> rank, std::shared_ptr<int> world_size) {


    auto mSort = std::make_unique<MergeSort>();

    auto result = std::make_shared<std::vector<std::string>>();


    // split vector                
    auto boundaries = split_even(vec.size(), *world_size);


    //Send size and contents of vectors to slave nodes
    for(int i = 0; i < boundaries.size(); i++) {
        
        int begin = boundaries.at(i).first;
        int end = boundaries.at(i).second;

        int length = end - begin;

        MPI_Send(length, 1, MPI_INT, i+1, 666, MPI_COMM_WORLD);
        MPI_Send(&vec[begin], length , MPI_CHAR, i+1, 777, MPI_COMM_WORLD);
    }


    std::vector<std::vector<std::string>> sub_vecs;

    // receive sorted vectors from slaves (do we wait for each sender synchronously?)
    for(int i = 0; i < world_size; i++) {
        
        int begin = boundaries.at(i).first;
        int end = boundaries.at(i).second;

        int length = end - begin;

        std::vector<std::string> temp_vec(length);
        
        MPI_Recv(&temp_vec, length, MPI_CHAR, MPI_ANY_SOURCE, 777, MPI_COMM_WORLD, &status);

        sub_vecs.push_back(temp_vec);
    }

    //TODO: merge recursively backwards on master node using void merge() ( eg. from 8 sorted vecs to a single one)
    //result = merge_back(sub_vecs);

    return result;

}


//TODO: How to pass MPI context to other functions? 
int main(int argc, char **argv) {
    
    //basic mpi functionality to demostrate it is working
    int rank;
    int world_size;
    int length;
    char name[80];

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Get_processor_name(name, &length);

    //Some info output
    std::cout << "Hello, MPI! Rank: " << rank << " size " << world_size << " on " << name << std::endl;

    if(rank != 0) { //slave

        slaveMerge(std::make_shared<MPI_Status> status, std::make_shared<MPI_Length> length, std::make_shared<MPI_Rank> rank, std::make_shared<int> world_size);
    
    } else {    //master, menu shall run only on ONE of all nodes (=> node 0)

        std::string input;

        while(true) {

            std::cout << "[Menu] Enter a path to a text file below or 'exit':" << std::endl;

            std::getline(std::cin, input);

            if (input == "exit") {

                std::cout << "[Menu] exiting.." << std::endl;
                break;

            } else {

                //TODO: extract this whole scope to function void masterMerge(std::string input)

                std::cout << "[Menu] Reading in file " << input << " ..." << std::endl;
                
                auto start = std::chrono::system_clock::now();
                auto vec = std::make_shared<std::vector<std::string>>(IO::readStringsFromFile(input));
                auto end = std::chrono::system_clock::now();

                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                
                std::cout << "[Menu] (time elapsed: " << elapsed.count() << "ms) Starting merge sort on file " << input << " ..." << std::endl;


                start = std::chrono::system_clock::now();
                auto merged_vec = masterMerge(std::shared_ptr<std::vector<std::string>> vec_to_merge, 
                    std::make_shared<MPI_Status> status, std::make_shared<MPI_Length> length, std::make_shared<MPI_Rank> rank, std::make_shared<int> world_size);
				end = std::chrono::system_clock::now();

                elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                std::cout << "[Menu] (time elapsed: " << elapsed.count() << "ms) finished!" << std::endl;

            }

        }
    }

    MPI_Finalize();

    return 0;    

}

/*
    Splitten die Vectoren durch n 
    [nvecs]

    for Schleife bzw. Scatter

    for Schleife bzw. Gather 

    [nsortedvecs]

    while ([nsortedvecs].size > 1) {
        vec merge(sortedvec1, sortedvec2)
        merge(merge, 0, sortedvec1.end(), merge.end());
        [mergesortedvecs]
    }

    newSortedVecs = nsortedVecs;


    while (counter < n/2) {
        
        while ( i < (n/2) +1) {
            newSortedvecs.at(i) = merge(newSortedvec_i, newSortedvec_i+1)
        }

        counter++;
    }
    
    
    
auto vec_splits = std::make_shared<std::vector<std::string>>(){ vec splits }
for(slave : slaves) {
    MPI_Send();
    MPI_Recv(buffer);

}

vec[vec1, vec2, ..., vec4];
std::map m = {(vec1.at(0), 0), (vec2.at(0), 1), ...};
vec_beginnings = {vec1.at(0), vec2.at(0), ...}
mSort->mergeSort(vec_beginnings, 0, vec->size() - 1);

vec = m.at(vec_beginnings.at(0)) + m.at(vec_beginnings.at(1)) + ...

*/

