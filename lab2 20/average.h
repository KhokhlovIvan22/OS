#pragma once
#include <vector>
using std::vector;

struct AverageEl {
	vector<int>& arr;
	int& avg;
};

static const int averageSleepTime = 7;

void averageThread(AverageEl args);
