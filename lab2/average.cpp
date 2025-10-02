#include"average.h"
#include <iostream>
#include <vector>
using std::vector;
using std::cout;

DWORD WINAPI averageThread(LPVOID param) {
	AverageEl* args = static_cast<AverageEl*>(param);
	vector<int>& a = args->arr;
	if (a.empty())
		return 0;
	long long sum = 0;
	for (int i = 0; i < a.size(); ++i)
	{
		sum += a[i];
		Sleep(averageSleepTime);
	}
	double acc = static_cast<double>(sum) / a.size();
	args->avg = static_cast<int>(acc >= 0 ? acc + 0.5 : acc - 0.5);
	cout << "Average element = " << args->avg <<  "\n";
	return 0;
}
