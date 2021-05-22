
/*
  Testfile for Merge Sort MPI Coding Challenge
  Author: Patrick Madlindl
  May 2021
*/

#include "gtest/gtest.h"

#include "HelperFuncs.hpp"



TEST (SplitTestBench, testSplitEven_10_3) {

 
  size_t size_to_split = 10;
  size_t num_splits = 3;

  //not optimal result, but it will do
  std::vector<std::pair<size_t, size_t>> expectedResult {std::make_pair(0, 3), std::make_pair(4, 7), std::make_pair(8, 9)};

  auto actualResult = split_even(size_to_split, num_splits);

  ASSERT_EQ (expectedResult.size(), actualResult.size());

  for (size_t i = 0; i < expectedResult.size(); i++) {

    EXPECT_EQ(expectedResult.at(i), actualResult.at(i));
  }
}

TEST (SplitTestBench, testSplitEven_9_3) {
  
  size_t size_to_split = 9;
  size_t num_splits = 3;

  std::vector<std::pair<size_t, size_t>> expectedResult {std::make_pair(0, 2), std::make_pair(3, 5), std::make_pair(6, 8)};

  auto actualResult = split_even(size_to_split, num_splits);

  ASSERT_EQ (expectedResult.size(), actualResult.size());

  for (size_t i = 0; i < expectedResult.size(); i++) {

    EXPECT_EQ(expectedResult.at(i), actualResult.at(i));
  }
}

TEST (SplitTestBench, testSplitEven_11_3) {

  size_t size_to_split = 11;
  size_t num_splits = 3;

  std::vector<std::pair<size_t, size_t>> expectedResult {std::make_pair(0, 3), std::make_pair(4, 7), std::make_pair(8, 10)};

  auto actualResult = split_even(size_to_split, num_splits);

  ASSERT_EQ (expectedResult.size(), actualResult.size());

  for (size_t i = 0; i < expectedResult.size(); i++) {

    EXPECT_EQ(expectedResult.at(i), actualResult.at(i));
  }
}

TEST (SplitTestBench, testSplitEven_9_4) {

 
  size_t size_to_split = 9;
  size_t num_splits = 4;

  std::vector<std::pair<size_t, size_t>> expectedResult {std::make_pair(0, 1), std::make_pair(2, 3), std::make_pair(4, 5), std::make_pair(6, 8)};

  auto actualResult = split_even(size_to_split, num_splits);

  ASSERT_EQ (expectedResult.size(), actualResult.size());

  for (size_t i = 0; i < expectedResult.size(); i++) {

    EXPECT_EQ(expectedResult.at(i), actualResult.at(i));
  }
}

TEST (SplitTestBench, testSplitEven_9min2) {
  
  size_t size_to_split = 9;
  size_t num_splits = -2;

  std::vector<std::pair<size_t, size_t>> expectedResult {std::make_pair(0, 8)};

  auto actualResult = split_even(size_to_split, num_splits);

  ASSERT_EQ (expectedResult.size(), actualResult.size());

  for (size_t i = 0; i < expectedResult.size(); i++) {

    EXPECT_EQ(expectedResult.at(i), actualResult.at(i));
  }
}

TEST (SplitTestBench, testSplitEven_9_0) {
  
  size_t size_to_split = 9;
  size_t num_splits = 0;

  std::vector<std::pair<size_t, size_t>> expectedResult {std::make_pair(0, 8)};

  auto actualResult = split_even(size_to_split, num_splits);

  ASSERT_EQ (expectedResult.size(), actualResult.size());

  for (size_t i = 0; i < expectedResult.size(); i++) {

    EXPECT_EQ(expectedResult.at(i), actualResult.at(i));
  }
}

TEST (SplitTestBench, testSplitEven_9_1) {
  
  size_t size_to_split = 9;
  size_t num_splits = 0;

  std::vector<std::pair<size_t, size_t>> expectedResult {std::make_pair(0, 8)};

  auto actualResult = split_even(size_to_split, num_splits);

  ASSERT_EQ (expectedResult.size(), actualResult.size());

  for (size_t i = 0; i < expectedResult.size(); i++) {

    EXPECT_EQ(expectedResult.at(i), actualResult.at(i));
  }
}
