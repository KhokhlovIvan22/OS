#include"min_max.h"
#include <iostream>
#include <vector>
using std::vector;
using std::cout;

DWORD WINAPI minmaxThread(LPVOID param) {
	MinMax* args = static_cast<MinMax*>(param);
	vector<int>& a = args->arr;
	if (a.empty()) return 0;
	int mn = a[0], mx = a[0];
	for (int i = 1; i < a.size(); ++i)
	{
		if (a[i] < mn)
			mn = a[i];
		if (a[i] > mx)
			mx = a[i];
		Sleep(minMaxSleepTime);
	}
	cout << "Min element = " << mn << ", max element = " << mx << "\n";
	args->min = mn;
	args->max = mx;
	return 0;
}
