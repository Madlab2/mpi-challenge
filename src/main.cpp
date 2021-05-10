#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>

#define DELIMITER ";"
#define EOL "\n"

void printVector(std::vector<std::string> vec);
void mergeSort(std::vector<std::string>* vec, int start, int end);
void merge(std::vector<std::string>* vec, int left, int center, int right);
bool isLess(std::string base, std::string comparison);
std::vector<std::string> readStringsFromFile(std::string filename);

int main()
{
    std::vector<std::string> vec(readStringsFromFile("example.txt"));
    //std::string arr[3] = { "Hallo","Alfa","Zebra" };
    //int size = sizeof(arr) / sizeof(arr[0]);
    mergeSort(&vec, 0, vec.size() - 1);
    printVector(vec);
}

void printVector(std::vector<std::string> vec) {
    for (unsigned int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << EOL;
    }
}

void mergeSort(std::vector<std::string>* vec, int start, int end) {
    if (start < end) {
        int center = start + (end - start) / 2;
        mergeSort(vec, start, center);
        mergeSort(vec, center + 1, end);
        merge(vec, start, center, end);
    }
}

void merge(std::vector<std::string>* vec, int left, int center, int right) {
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
            vec->at(cnt) = vec_right.at(cnt_right);
            cnt++;
            cnt_right++;
        }
        else {
            vec->at(cnt) = vec_left.at(cnt_left);
            cnt++;
            cnt_left++;
        }
    }
    while (cnt_left < num_left) {
        vec->at(cnt) = vec_left.at(cnt_left);
        cnt++;
        cnt_left++;
    }
    while (cnt_right < num_right) {
        vec->at(cnt) = vec_right.at(cnt_right);
        cnt++;
        cnt_right++;
    }
}

bool isLess(std::string base, std::string comparison) {
    transform(base.begin(), base.end(), base.begin(), ::tolower);
    transform(comparison.begin(), comparison.end(), comparison.begin(), ::tolower);
    if (comparison < base) {
        return true;
    }
    else {
        return false;
    }
}

std::vector<std::string> readStringsFromFile(std::string filename) {
    std::ifstream inputFile;
    std::string inputString;
    std::vector<std::string> vec;
    inputFile.open(filename);
    inputFile >> inputString;
    inputFile.close();
    size_t pos = 0;
    std::string substr;
    while ((pos = inputString.find(DELIMITER)) != std::string::npos) {
        substr = inputString.substr(0, pos);
        vec.push_back(substr);
        inputString.erase(0, pos + 1);
    }
    if (inputString.length() > 0) {
        vec.push_back(inputString);
    }
    return vec;
}
