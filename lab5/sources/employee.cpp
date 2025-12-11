#include "employee.h"
#include <cstring>
#include <iomanip>

using std::istream;
using std::ostream;
using std::string;
using std::streamoff;
using std::memcpy;
using std::setprecision;
using std::fixed;

bool employee::validate(string& err) const {
    err.clear();
    if (num <= 0) {
        err = "ID must be positive";
        return false;
    }
    if (name[0] == '\0') {
        err = "Name must be non-empty";
        return false;
    }
    if (hours < 0.0) {
        err = "Hours must be non-negative";
        return false;
    }
    return true;
}

istream& operator>>(istream& in, employee& e) {
    string name;
    if (in >> e.num >> name >> e.hours) {
        int length;
        if (name.length() < 9) 
            length = name.length();
        else 
            length = 9;
        memcpy(e.name, name.data(), length);
        e.name[length] = '\0';
        for (int i = length + 1; i < 10; ++i) 
            e.name[i] = '\0';
        if (e.num <= 0 || e.name[0] == '\0' || e.hours < 0.0) 
            in.setstate(std::ios::failbit);
    }
    return in;
}

ostream& operator<<(ostream& out, const employee& e) {
    out << "ID=" << e.num << " Name=" << e.name << " Hours=" << fixed << setprecision(2) << e.hours;
    return out;
}

istream& employee::readBin(istream& in) {
    in.read((char*)this, sizeof(employee));
    return in;
}

ostream& employee::writeBin(ostream& out) const {
    out.write((const char*)this, sizeof(employee));
    return out;
}

istream& employee::readBinAt(istream& in, streamoff index, employee& e) {
    in.seekg(index * sizeof(employee), std::ios::beg);
    return e.readBin(in);
}

ostream& employee::writeBinAt(ostream& out, streamoff index, const employee& e) {
    out.seekp(index * sizeof(employee), std::ios::beg);
    return e.writeBin(out);
}