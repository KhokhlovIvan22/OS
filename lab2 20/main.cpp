#include "array_functions.h"
#include "min_max.h"
#include "average.h"
#include <iostream>
#include <thread>
#include <vector>
#include <stdexcept>
using std::vector;
using std::cin;
using std::cout;
using std::cerr;
using std::invalid_argument;
using std::runtime_error;
using std::thread;

int main() {
    try {
        vector<int> arr;
        cin >> arr;
        cout << "\n";
        int min = 0, max = 0, avg = 0;
        MinMax mm{ arr, min, max };
        AverageEl av{ arr, avg };
        thread minMaxThreadHandler(minmaxThread, mm);
        thread averageThreadHandler(averageThread, av);
        minMaxThreadHandler.join();
        averageThreadHandler.join();
        modify(arr, min, max, avg);
        cout << "\nModified array:\n" << arr;
        return 0;
    }
    catch (const invalid_argument& ex) {
        cerr << ex.what() << "\n";
        return 1;
    }
    catch (const runtime_error& ex) {
        cerr << "Thread error: " << ex.what() << "\n";
        return 2;
    }
}