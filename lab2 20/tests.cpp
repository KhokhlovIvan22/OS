#include <gtest/gtest.h>
#include "array_functions.h"
#include "min_max.h"
#include "average.h"
#include <sstream>
#include <vector>

using std::vector;
using std::istringstream;
using std::ostringstream;
using std::invalid_argument;

// checks if the input array from the stream is valid
TEST(VectorFunc, InputValid) {
    istringstream in("3 10 20 30\n");
    vector<int> arr;
    in >> arr;
    EXPECT_EQ(arr.size(), 3u);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

// checks for 0-size input
TEST(VectorFunc, InputInvalidSizeThrows) {
    istringstream in("0\n");
    vector<int> arr;
    EXPECT_THROW(in >> arr, invalid_argument);
}

// checks the output format of an array
TEST(VectorFunc, CorrectOutput) {
    vector<int> arr{ 1, 2, 3 };
    ostringstream out;
    out << arr;
    EXPECT_EQ(out.str(), "1 2 3 \n");
}

// checks function modify
TEST(VectorFunc, ReplaceMinMaxWithAvg) {
    vector<int> arr{ 1, 5, 9 };
    int min = 1, max = 9;
    int avg = 5;
    modify(arr, min, max, avg);
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 5);
    EXPECT_EQ(arr[2], 5);
}

// checks minMaxThread work
TEST(Threads, MinMaxThreadFindsCorrectValues) {
    vector<int> arr{ 7, 2, 10, 5 };
    int min = 0, max = 0;
    MinMax mm{ arr, min, max };
    minmaxThread(mm);
    EXPECT_EQ(min, 2);
    EXPECT_EQ(max, 10);
}

// checks averageThread work
TEST(Threads, AverageThreadComputesCorrectValue) {
    vector<int> arr{ 2, 4, 6 };
    int avg = 0;
    AverageEl av{ arr, avg };
    averageThread(av);
    EXPECT_EQ(avg, 4);
}

// end-to-end test: checks how all array functions excluding io and threads work together
TEST(EndToEnd, FullArrayProcessing) {
    vector<int> arr{ -5, 0, 5 };
    int min = 0, max = 0, avg = 0;
    MinMax mm{ arr, min, max };
    AverageEl av{ arr, avg };
    minmaxThread(mm);
    averageThread(av);
    modify(arr, min, max, avg);
    EXPECT_EQ(min, -5);
    EXPECT_EQ(max, 5);
    EXPECT_EQ(avg, 0);
    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[1], 0);
    EXPECT_EQ(arr[2], 0);
}