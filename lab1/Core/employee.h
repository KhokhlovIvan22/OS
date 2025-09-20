#pragma once
#include <fstream>
#include <vector>
#include <string>
using std::istream;
using std::ofstream;
using std::vector;
using std::string;
using std::pair;

struct employee
{
	int num;
	char name[10];
	double hours;
	friend istream& operator >> (istream& in, employee& emp);
	friend ofstream& operator << (ofstream& out, const pair <employee, double>& emp);
};

int readFromBinary (const string& binFileName, vector <employee>& employees);
int writeReport(const string& outFileName, const string& binFileName, const vector <employee>& employees, double rate);
