#pragma once


#include <assert.h>
#include <iostream>

#include <vector>



//gets a size of a one-dimensional set and the number of substets (of similar sizes) the set shall be splittet up into
//returns vector of pairs containing begin and end indices of every subset
std::vector<std::pair<size_t, size_t>> split_even(const size_t size_to_split, const size_t num_splits) {

    //eg. for 5 Elements splitted in two parts: {(0, 1), (2, 4)}  

    if(num_splits <= 1 || size_to_split <= num_splits) {
        //do basically nothing if we shall split in only one part or the parts would be too small
        return std::vector<std::pair<size_t, size_t>>{std::make_pair(0, size_to_split-1)};
    }

    size_t pos = 0;
    const size_t end = size_to_split;     //analogue to iterator.end()
    const size_t last = end - 1;          //last valid idx to an element

    size_t block_size = size_to_split;

    if(size_to_split % num_splits == 0) {

        block_size = size_to_split/num_splits;

    } else {
        
        // + 1 so most blocks will be a little bigger
        block_size = (size_to_split/num_splits) + 1;    

        if (size_to_split % block_size == 0) {

            //in case our +1 makes the modulo suddenly be 0, we would have one num_split too few! So in this case, we decrease blocksize again
            block_size--;
        }
           
    }
    
    std::vector<std::pair<size_t, size_t>> result;

    while(pos != end) {

       if((end - pos >= block_size) && result.size() < (num_splits - 1)) {

           result.push_back(std::make_pair(pos, pos + block_size -1));
           pos += block_size;

       } else {

           result.push_back(std::make_pair(pos, last));
           pos = end;
       }
    }

    assert(result.size() == num_splits);

    return result;
}


/*
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
*/



