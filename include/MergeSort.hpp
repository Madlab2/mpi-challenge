#pragma once

#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <fstream>


class MergeSort {

    
    public:
        MergeSort();
        ~MergeSort();

        void mergeSort(std::shared_ptr<std::vector<std::string>> vec, int start, int end);

        void merge(std::shared_ptr<std::vector<std::string>> vec, int left, int center, int right);
        
    private:
        bool isLess(std::string base, std::string comparison);
};

