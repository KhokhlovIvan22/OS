#include"min_max.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using std::vector;
using std::cout;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

void minmaxThread(MinMax args) {
	if (args.arr.empty()) return;
	args.min = args.arr[0];
	args.max = args.arr[0];
	for (int x : args.arr) {
		if (x < args.min)
			args.min = x;
		if (x > args.max)
			args.max = x;
		sleep_for(milliseconds(minMaxSleepTime));
	}
	cout << "Min element = " << args.min << ", max element = " << args.max << "\n";
}