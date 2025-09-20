#include "employee.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
using std::string;
using std::stod;
using std::cerr;
using std::out_of_range;
using std::exception;
using std::sort;
using std::runtime_error;
using std::cout;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Error: invalid startup format\n";
        return 1;
    }

    string binFileName = argv[1];
    string reportFileName = argv[2];
    double rate = 0.0;
    try {
        rate = stod(argv[3]);
        if (rate <= 0.0)
            throw out_of_range("Error: hourly rate must be greater than zero");
    }
    catch (const exception& ex) {
        cerr << ex.what() << "\n";
        return 2;
    }

    vector <employee> employees;
    if (readFromBinary(binFileName, employees) != 0)
        return 3;
    sort(employees.begin(), employees.end());
    if (writeReport(reportFileName, binFileName, employees, rate) != 0)
        return 3;

    cout << "Report successfully written to file \"" << reportFileName << "\"\n";
    return 0;
}
