#pragma once
#include <vector>
#include <windows.h>
using std::vector;

struct MinMax {
	vector<int>& arr;
	int& min;
	int& max;
};

DWORD WINAPI minmaxThread(LPVOID param);