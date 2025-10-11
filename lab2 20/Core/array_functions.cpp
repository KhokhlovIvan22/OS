#include "array_functions.h"
#include <iostream>
#include <vector>
using std::istream;
using std::ostream;
using std::invalid_argument;
using std::cout;
using std::vector;

istream& operator >> (istream& in, vector<int>& arr) {
	cout << "Enter array size: ";
	int n;
	in >> n;
	if (n <= 0) 
		throw invalid_argument("Error! Invalid array size\n");
	arr.resize(n);
	cout << "Enter " << n << " array elements:\n";
	for (int& x: arr)
		in >> x;
	return in;
}

ostream& operator << (ostream& out, const vector<int>& arr) {
	for (const int&x : arr)
		out << x << " ";
	out << "\n";
	return out;
}

void modify(vector<int>& arr, int min, int max, int avg) {
	if (arr.empty()) return;
	for (int& x: arr) {
		if (x == min || x == max) {
			x = avg;
		}
	}
}
