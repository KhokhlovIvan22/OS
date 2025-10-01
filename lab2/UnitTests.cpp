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

//ñhecks if the input array from the stream is valid.
TEST(VectorFunc, InputValid) {
    istringstream in("3 10 20 30\n");
    vector<int> arr;
    in >> arr;
    EXPECT_EQ(arr.size(), 3u);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

//checks for 0-size inout
TEST(VectorFunc, InputInvalidSizeThrows) {
    istringstream in("0\n");
    vector<int> arr;
    EXPECT_THROW(in >> arr, invalid_argument);
}

//checks the output format of an array
TEST(VectorFunc, CorrectOutput) {
    vector<int> arr{ 1, 2, 3 };
    ostringstream out;
    out << arr;
    EXPECT_EQ(out.str(), "1 2 3 \n");
}

//checks functrion modify
TEST(VectorFunc, ReplaceMinMaxWithAvg) {
    vector<int> arr{ 1, 5, 9 };
    int min = 1, max = 9;
    double avg = 5.0;  
    modify(arr, min, max, avg);
    EXPECT_EQ(arr[0], 5); 
    EXPECT_EQ(arr[1], 5); 
    EXPECT_EQ(arr[2], 5); 
}

//checks minMaxThread work
TEST(Threads, MinMaxThreadFindsCorrectValues) {
    vector<int> arr{ 7, 2, 10, 5 };
    int min = 0, max = 0;
    MinMax args{ arr, min, max };
    minmaxThread(&args);
    EXPECT_EQ(min, 2);
    EXPECT_EQ(max, 10);
}

//checks averageThread work
TEST(Threads, AverageThreadComputesCorrectValue) {
    vector<int> arr{ 2, 4, 6 };
    double avg = 0.0;
    AverageEl args{ arr, avg };
    averageThread(&args);
    EXPECT_DOUBLE_EQ(avg, 4.0); // (2+4+6)/3 = 4.0
}