#pragma once
#include <vector>
#include <windows.h>
using std::vector;

struct AverageEl {
	vector<int>& arr;
	int& avg;
};

static const int averageSleepTime = 7;

DWORD WINAPI averageThread(LPVOID param);
