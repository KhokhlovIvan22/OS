#include "employee.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

using namespace std;

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

    vector <employee> emloyess;
    try {
        ifstream inFile(binFileName, ios::binary);
        if (!inFile.is_open())
            throw runtime_error("Error: could not open input file");
        employee e;
        while (inFile.read(reinterpret_cast<char*>(&e), sizeof(e))) {
            emloyess.push_back(e);
        }
        inFile.close();
        sort(emloyess.begin(), emloyess.end(), [](const employee& a, const employee& b) {return a.num < b.num; });

        ofstream outFile(reportFileName);
        if (!outFile.is_open()) {
            throw runtime_error("Error: could not open report file");
        }

        outFile << "Report on file \"" << binFileName << "\"\n" << left << fixed << setprecision(2);
        outFile << left << setw(10) << "Number" << setw(20) << "Name" << setw(16) << "Hours worked" << setw(12) << "Salary" << "\n";
        double salary = 0.0;
        for (const auto& e : emloyess) {
            salary = e.hours * rate;
            outFile << left << setw(10) << e.num << setw(20) << e.name << setw(16) << e.hours << setw(12) << salary << "\n";
        }
        outFile.close();
    }
    catch (const exception& ex) {
        cerr << ex.what() << "\n";
        return 3;
    }

    cout << "Report successfully written to file \"" << reportFileName << "\"\n";
    return 0;
}
