#pragma once
#include <vector>
#include <memory>
#include <deque>
#include <assert.h>
#include <iostream>

#include "MergeSort.hpp"


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


void merge_back(std::deque<std::vector<std::string>> & vecs_to_merge) {

    /*
    1 2 3 4 5 6
    3 4 5 6 1*
    5 6 1* 3*
    1* 3* 5*
    5* 1** 
    5**      --> return
    */
    auto mSort = std::make_unique<MergeSort>();

    int left = 0;
    
    while (vecs_to_merge.size() > 1) {
        
        size_t size_vec1 = vecs_to_merge.at(0).size();
        size_t size_vec2 = vecs_to_merge.at(1).size();

        int center = size_vec1 - 1;
        int right = size_vec1 + size_vec2 - 1;
        
        //concatenate first and second vector in vecs_to_merge to one vector (will be merged in merge())
        std::vector<std::string> vec_to_merge = *vecs_to_merge.begin();
        vec_to_merge.insert(vec_to_merge.end(), (*(vecs_to_merge.begin() + 1)).begin(), (*(vecs_to_merge.begin() + 1)).end());

        auto vec_to_merge_ptr = std::make_shared<std::vector<std::string>>(vec_to_merge);

        //remove 
        vecs_to_merge.pop_front();
        vecs_to_merge.pop_front();
        
        mSort->merge(vec_to_merge_ptr, left, center, right);

        //push back sorted vector of strings
        vecs_to_merge.push_back(*vec_to_merge_ptr);
    }
    
}