#pragma once
#include <vector>
#include <windows.h>
using std::vector;

struct AverageEl {
	vector<int>& arr;
	double& avg;
};

DWORD WINAPI averageThread(LPVOID param);
