#pragma once


#include <string>

#include <memory>
#include <deque>
#include <vector>

#include <algorithm>


class MergeSort {

    
    public:
        MergeSort();
        ~MergeSort();

        void mergeSort(std::shared_ptr<std::vector<std::string>> vec, int start, int end);

        void merge_back(std::deque<std::vector<std::string>> & vecs_to_merge);

    private:
        void merge(std::shared_ptr<std::vector<std::string>> vec, int left, int center, int right);

        bool isLess(std::string base, std::string comparison);
};

