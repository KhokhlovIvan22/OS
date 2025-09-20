#include "employee.h"
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
using std::stoi;
using std::cerr;
using std::out_of_range;
using std::exception;
using std::runtime_error;
using std::ios;
using std::string;
using std::ofstream;
using std::cout;
using std::cin;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Error: invalid startup format\n";
        return 1;
    }

    string binFileName = argv[1];
    int recordCount = 0;
    try {
        recordCount = stoi(argv[2]);
        if (recordCount <= 0)
            throw out_of_range("Error: amount of records must be greater than zero");
    }
    catch (const exception& ex) {
        cerr << ex.what() << "\n";
        return 2;
    }

    try {
        ofstream outFile(binFileName, ios::binary | ios::trunc);
        if (!outFile.is_open()) {
            throw runtime_error("Error: сould not open file for writing");
        }
        employee e;
        string tmpName;
        for (int i = 0; i < recordCount; ++i) {
            cout << "\n\n";
            cout <<"Record No. " << (i + 1) << "\n";
            cin >> e;
            if (!cin)
                throw runtime_error("Input error");
            outFile.write(reinterpret_cast<const char*>(&e), sizeof(e));
            if (!outFile)
                throw runtime_error("Write error");
        }
        outFile.close();
    }
    catch (const exception& ex) {
        cerr << ex.what() << "\n";
        return 3;
    }

    cout << "Data successfully written to file \"" << binFileName << " \"\n";
    return 0;
}
