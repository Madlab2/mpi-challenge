
/*
  Testfile for Merge Sort MPI Coding Challenge
  Author: Patrick Madlindl
  May 2021
*/

#include "gtest/gtest.h"

#include "MergeSort.hpp"
#include "InOutFuncs.hpp"



TEST (MergeSortTestBench, fooTest) {

  EXPECT_EQ(true, true);
  EXPECT_NE(true, false);

}

//TODO: test single method (is private however)
TEST (MergeSortTestBench, testMerge) {

  EXPECT_EQ(true, true);
  EXPECT_NE(true, false);

}

//TODO: throws segmentation fault with empty files!
TEST (MergeSortTestBench, testMergeSort) {

    auto mSort = std::make_unique<MergeSort>();
    //we are at build/test and go to test/ExampleTexts/
    auto words = std::make_shared<std::vector<std::string>>(IO::readStringsFromFile("../../test/ExampleTexts/text1.txt"));

    //TODO: throws segmentation fault with empty files!
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