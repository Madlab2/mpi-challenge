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

void MergeSort::merge_back(std::deque<std::vector<std::string>> & vecs_to_merge) {

    /*
    1 2 3 4 5 6
    3 4 5 6 1*
    5 6 1* 3*
    1* 3* 5*
    5* 1** 
    5**      --> return
    */
    

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
        
        merge(vec_to_merge_ptr, left, center, right);

        //TODO: do not copy things, ultra inefficient!!!
        //push back sorted vector of strings
        vecs_to_merge.push_back(*vec_to_merge_ptr);
    }
    
}