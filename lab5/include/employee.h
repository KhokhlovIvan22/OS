#pragma once
#include <iostream>
#include <string>

using std::istream;
using std::ostream;
using std::string;
using std::streamoff;

struct employee {
    int num;
    char name[10];
    double hours;

    bool validate(string& err) const;

    friend istream& operator>>(istream& in, employee& e);
    friend ostream& operator<<(ostream& out, const employee& e);

    istream& readBin(istream& in);
    ostream& writeBin(ostream& out) const;

    static istream& readBinAt(istream& in, streamoff index, employee& e);
    static ostream& writeBinAt(ostream& out, streamoff index, const employee& e);
};
