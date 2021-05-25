
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


int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}