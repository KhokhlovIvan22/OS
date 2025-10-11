#pragma once
#include <vector>
using std::vector;

struct MinMax {
	vector<int>& arr;
	int& min;
	int& max;
};

static const int minMaxSleepTime = 12;

void minmaxThread(MinMax args);