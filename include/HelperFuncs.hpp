#pragma once
#include <vector>
#include <assert.h>
#include <iostream>

//TODO
//gets a size of a one-dimensional set and the number of substets (of similar sizes) the set shall be splittet up into
//returns vector of pairs containing begin and end indices of every subset
std::vector<std::pair<size_t, size_t>> split_even(const size_t size_to_split, const size_t num_splits) {

    //eg. for 5 Elements splitted in two parts: {(0, 1), (2,4)}

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