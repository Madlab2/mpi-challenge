#include "MergeSort.hpp"



MergeSort::MergeSort() {}

MergeSort::~MergeSort() {}




void MergeSort::mergeSort(std::shared_ptr<std::vector<std::string>> vec, int start, int end) {

    if (start < end) {
        int center = start + (end - start) / 2;

        mergeSort(vec, start, center);
        mergeSort(vec, center + 1, end);

        merge(vec, start, center, end);
    }

}

void MergeSort::merge(std::shared_ptr<std::vector<std::string>> vec, int left, int center, int right) {

    const int num_left = center - left + 1;
    const int num_right = right - center;

    std::vector<std::string> vec_left, vec_right;

    for (int i = 0; i < num_left; i++) {
        vec_left.push_back(vec->at(left + i));
    }

    for (int j = 0; j < num_right; j++) {
        vec_right.push_back(vec->at(center + j + 1));
    }

    int cnt_left, cnt_right, cnt;
    cnt_left = 0;
    cnt_right = 0;
    cnt = left;

    while (cnt_left < num_left && cnt_right < num_right) {
        
        if (isLess(vec_left.at(cnt_left), vec_right.at(cnt_right))) {
            //TODO: make vec thread-safe! (mutex block)
            //Question: how to make things thread safe not only accross threads but across HOSTS ?
            vec->at(cnt) = vec_right.at(cnt_right);
            cnt++;
            cnt_right++;
        }
        else {
            //TODO: make vec thread-safe!
            vec->at(cnt) = vec_left.at(cnt_left);
            cnt++;
            cnt_left++;
        }
    }

    while (cnt_left < num_left) {
        //TODO: make vec thread-safe!
        vec->at(cnt) = vec_left.at(cnt_left);
        cnt++;
        cnt_left++;
    }
    
    while (cnt_right < num_right) {
        //TODO: make vec thread-safe!
        vec->at(cnt) = vec_right.at(cnt_right);
        cnt++;
        cnt_right++;
    }
    
}

bool MergeSort::isLess(std::string base, std::string comparison) {

    //lower cast
    transform(base.begin(), base.end(), base.begin(), ::tolower);
    transform(comparison.begin(), comparison.end(), comparison.begin(), ::tolower);

    if (comparison < base) {
        return true;
    }
    else {
        return false;
    }
}