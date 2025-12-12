#include "employee.h"
#include <cstring>
#include <iomanip>
#include <stdexcept>

using std::istream;
using std::ostream;
using std::string;
using std::streamoff;
using std::memcpy;
using std::setprecision;
using std::fixed;
using std::ios;
using std::runtime_error;

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
    if (std::strlen(name) >= 10) {
        err = "Name too long (max 9 chars)";
        return false;
    }
    if (hours < 0.0) {
        err = "Hours must be non-negative";
        return false;
    }
    return true;
}

istream& operator>>(istream& in, employee& e) {
    string tmpName;
    if (in >> e.num >> tmpName >> e.hours) {
        if (tmpName.empty()) {
            in.setstate(ios::failbit);
            return in;
        }
        int length;
        if (tmpName.length() < 9)
            length = tmpName.length();
        else
            length = 9;
        memcpy(e.name, tmpName.data(), length);
        e.name[length] = '\0';
        for (int i = length + 1; i < 10; ++i)
            e.name[i] = '\0';
        if (e.num <= 0 || e.name[0] == '\0' || e.hours < 0.0)
            in.setstate(ios::failbit);
    }
    return in;
}

ostream& operator<<(ostream& out, const employee& e) {
    out << "ID:" << e.num << " Name:" << e.name << " Hours:" << fixed << setprecision(2) << e.hours;
    return out;
}

istream& employee::readBin(istream& in) {
    in.read((char*)this, sizeof(employee));
    if (!in) 
        in.setstate(ios::failbit);
    return in;
}

ostream& employee::writeBin(ostream& out) const {
    out.write((const char*)this, sizeof(employee));
    if (!out) 
        throw runtime_error("Failed to write employee");
    return out;
}

istream& employee::readBinAt(istream& in, streamoff index, employee& e) {
    in.seekg(index * sizeof(employee), ios::beg);
    if (!in) {
        in.setstate(ios::failbit);
        return in;
    }
    return e.readBin(in);
}

ostream& employee::writeBinAt(ostream& out, streamoff index, const employee& e) {
    out.seekp(index * sizeof(employee), ios::beg);
    if (!out) 
        throw runtime_error("Failed to seek in file for write");
    return e.writeBin(out);
}