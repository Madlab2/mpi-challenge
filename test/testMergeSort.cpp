
/*
  Testfile for Merge Sort MPI Coding Challenge
  Author: Patrick Madlindl
  May 2021
*/

#include "gtest/gtest.h"

#include "MergeSort.hpp"
#include "InOutFuncs.hpp"

#include "TestSplitEven.hpp"  //import tests for split_even()


TEST (MergeSortTestBench, testMergeSort) {

    auto mSort = std::make_unique<MergeSort>();
    //we are at build/test and go to test/ExampleTexts/
    std::vector<std::string> words;

    IO::readStringsFromFile("../../test/ExampleTexts/text1.txt", words);

    auto words_sorted = std::make_shared<std::vector<std::string>>(words);

    mSort->mergeSort(words_sorted, 0, words_sorted->size() - 1);

    //read in solution
    std::vector<std::string> words_truth;
    IO::readStringsFromFile("../../test/ExampleSolutions/solution1.txt", words_truth);

    auto groundTrouth = std::make_shared<std::vector<std::string>>(words_truth);

    //Check lenghts
    ASSERT_EQ(words_sorted->size(), groundTrouth->size());
    EXPECT_NE(words_sorted->size(), 0);

    //check order
    for(int element = 0; element < words_sorted->size(); ++element) {

        EXPECT_EQ(words_sorted->at(element), groundTrouth->at(element));
    }

}

TEST (MergeSortTestBench, testMergeBack) {

    auto mSort = std::make_unique<MergeSort>();

    int num_slaves = 4;

    std::vector<std::string> sorted_1{"Hallo", "hier", "Morgen", "numpy"};
    std::vector<std::string> sorted_2{"Sigmund", "Tor", "Unimog", "Xilinx"};
    std::vector<std::string> sorted_3{"3d", "Adalbert", "das", "der", "die"};
    std::vector<std::string> sorted_4{"3d", "Samuel", "witzig"};

    //storage for result vectors
    std::deque<std::vector<std::string>> sorted_vecs{sorted_1, sorted_2, sorted_3, sorted_4};

    mSort->merge_back(sorted_vecs);

    std::vector<std::string> expected_result{"3d", "3d", "Adalbert", "das", "der", 
                                            "die", "Hallo", "hier", "Morgen", "numpy", 
                                            "Samuel", "Sigmund", "Tor", "Unimog", "witzig", "Xilinx"};
    

    EXPECT_EQ(expected_result.size(), (sorted_vecs.front()).size());

    //check order
    for(int element = 0; element < expected_result.size(); ++element) {

        EXPECT_EQ(expected_result.at(element), (sorted_vecs.front()).at(element));
    }

}


int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}