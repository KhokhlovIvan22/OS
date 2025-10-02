#pragma once
#include <vector>
#include <windows.h>
using std::vector;

struct MinMax {
	vector<int>& arr;
	int& min;
	int& max;
};

static const int minMaxSleepTime = 12;

DWORD WINAPI minmaxThread(LPVOID param);
