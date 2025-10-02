#include "array_functions.h"
#include "min_max.h"
#include "average.h"
#include <windows.h>
#include <iostream>
#include <vector>
using std::vector;
using std::cin;
using std::cout;
using std::invalid_argument;
using std::cerr;
using std::runtime_error;

int main() 
{
    try {
        vector<int> arr;
        cin >> arr;
        cout << "\n";
        int min = 0, max = 0, avg = 0;
        MinMax mm{ arr, min, max };
        AverageEl av{ arr, avg };
        HANDLE hMinMaxThread = CreateThread(nullptr, 0, minmaxThread, &mm, 0, nullptr);
        if (!hMinMaxThread) {
            throw runtime_error("Failed to create minmaxThread");
        }
        HANDLE hAverageThread = CreateThread(nullptr, 0, averageThread, &av, 0, nullptr);
        if (!hAverageThread) {
            CloseHandle(hMinMaxThread);
            throw runtime_error("Failed to create averageThread");
        }
        WaitForSingleObject(hMinMaxThread, INFINITE);
        WaitForSingleObject(hAverageThread, INFINITE);
        CloseHandle(hMinMaxThread);
        CloseHandle(hAverageThread);
        modify(arr, min, max, avg);
        cout << "\n";
        cout << "Modified array:\n" << arr;
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
