#include "employee.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <vector>
using std::ifstream;
using std::ofstream;
using std::istream;
using std::runtime_error;
using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::ios;
using std::fixed;
using std::left;
using std::setw;
using std::setprecision;
using std::make_pair;
using std::cerr;
using std::exception;

istream& operator>>(istream& in, employee& emp) {
    string tmpName;
    cout << "Enter employee number: ";
    in >> emp.num;
    cout << "Enter employee name: ";
    in >> tmpName;
    strcpy_s(emp.name, sizeof(emp.name), tmpName.c_str());
    cout << "Enter number of hours worked: ";
    in >> emp.hours;
    return in;
}

ofstream& operator << (ofstream& out, const pair <employee,double>& emp) {
    const employee& e = emp.first;
    double rate = emp.second;
    double salary = rate * e.hours;
	out << left << setw(10) << e.num << setw(20) << e.name << setw(16) << e.hours << setw(12) << salary << "\n";
	return out;
}

bool employee::operator < (const employee& other) const
{
    return num < other.num;
}

int readFromBinary(const string& binFileName, vector<employee>& employees) {
    try {
        ifstream inFile(binFileName, ios::binary);
        if (!inFile.is_open())
            throw runtime_error("Error: could not open input file");
        employee e;
        while (inFile.read(reinterpret_cast<char*>(&e), sizeof(e)))
            employees.push_back(e);
        inFile.close();
        return 0;
    }
    catch (const exception& ex) {
        cerr << ex.what() << "\n";
        return 3;
    }
}

int writeReport(const string& outFileName, const string& binFileName, const vector <employee>& employees, double rate)
{
    try {
        ofstream outFile(outFileName);
        if (!outFile.is_open()) {
            throw runtime_error("Error: could not open report file");
        }
        outFile << "Report on file \"" << binFileName << "\"\n" << left << fixed << setprecision(2);
        outFile << left << setw(10) << "Number" << setw(20) << "Name" << setw(16) << "Hours worked" << setw(12) << "Salary" << "\n";
        for (const auto& e : employees)
            outFile << make_pair(e, rate);
        outFile.close();
        return 0;
    }
    catch (const exception& ex) {
        cerr << ex.what() << "\n";
        return 3;
    }
}