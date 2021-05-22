
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
    auto words = std::make_shared<std::vector<std::string>>(IO::readStringsFromFile("../../test/ExampleTexts/text1.txt"));

    mSort->mergeSort(words, 0, words->size() - 1);

    //read in solution
    auto groundTrouth = std::make_shared<std::vector<std::string>>(IO::readStringsFromFile("../../test/ExampleSolutions/solution1.txt"));

    //Check lenghts
    ASSERT_EQ(words->size(), groundTrouth->size());
    EXPECT_NE(words->size(), 0);

    //check order
    for(int element = 0; element < words->size(); ++element) {

        EXPECT_EQ(words->at(element), groundTrouth->at(element));
    }
    

}


int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}