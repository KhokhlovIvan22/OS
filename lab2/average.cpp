#include"average.h"
#include <iostream>
#include <vector>
using std::vector;
using std::cout;
const int sleepTime = 12;

DWORD WINAPI averageThread(LPVOID param) {
	AverageEl* args = static_cast<AverageEl*>(param);
	vector<int>& a = args->arr;
	if (a.empty())
		return 0;
	long long sum = 0.0;
	for (int i = 0; i < a.size(); ++i)
	{
		sum += a[i];
		Sleep(sleepTime);
	}
	args->avg = sum / a.size();
	cout << "Average element = " << args->avg <<  "\n";
	return 0;
}