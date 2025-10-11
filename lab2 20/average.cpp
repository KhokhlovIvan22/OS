#include"average.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>

using std::vector;
using std::cout;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::round;

void averageThread(AverageEl args) {
    if (args.arr.empty()) return;
    long long sum = 0;
    for (int x: args.arr) {
        sum += x;
        sleep_for(milliseconds(averageSleepTime));
    }
    double raw = static_cast<double>(sum) / args.arr.size();
    args.avg = static_cast<int>(round(raw));
    cout << "Average element = " << args.avg << "\n";
}