#pragma once
#include <iostream>
#include <vector>
using std::istream;
using std::ostream;
using std::vector;

istream& operator >> (istream& in, vector<int>& arr);
ostream& operator << (ostream& out, const vector<int>& arr);
void modify(vector<int>& arr, int min, int max, int avg);