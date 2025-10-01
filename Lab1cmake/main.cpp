#include "employee.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using std::string;
using std::wstring;
using std::cin;
using std::cout;
using std::to_wstring;
using std::cerr;
using std::ifstream;
using std::ios;

int main() {
    string binFileName;
    int  recordCount;
    cout << "Enter binary file name: ";
    cin >> binFileName;
    cout << "Enter number of records: ";
    cin >> recordCount;
    wstring binFileW = wstring(binFileName.begin(), binFileName.end());
    wstring countW = to_wstring(recordCount);
    {
        wstring cmdCreator = L"Creator.exe " + binFileW + L" " + countW;
        LPWSTR cmd1 = &cmdCreator[0];

        STARTUPINFOW si1;
        PROCESS_INFORMATION pi1;
        ZeroMemory(&si1, sizeof(si1));
        si1.cb = sizeof(si1);
        ZeroMemory(&pi1, sizeof(pi1));

        if (!CreateProcessW(nullptr, cmd1, nullptr, nullptr, FALSE,  0,  nullptr, nullptr, &si1, &pi1))
        {
            cerr << "Error: failed to launch Creator (" << GetLastError() << ")\n";
            return 1;
        }

        WaitForSingleObject(pi1.hProcess, INFINITE);
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
    }

    {
        ifstream in(binFileName, ios::binary);
        if (!in) {
            cerr << "Error: cannot open \"" << binFileName << "\" for reading\n";
            return 2;
        }

        cout << "\nBinary file content:\n";
        employee e;
        while (in.read(reinterpret_cast<char*>(&e), sizeof(e))) {
            cout << e.num << "  "  << e.name << "  "  << e.hours << "\n";
        }
    }

    string reportFileName;
    double hourlyRate;
    cout << "\nEnter report file name: ";
    cin >> reportFileName;
    cout << "Enter hourly rate: ";
    cin >> hourlyRate;
    wstring reportFileW = wstring(reportFileName.begin(), reportFileName.end());
    wstring hourlyRateW = to_wstring(hourlyRate);
    {
        wstring cmdReporter = L"Reporter.exe " + binFileW + L" " + reportFileW + L" " + hourlyRateW;
        LPWSTR cmd2 = &cmdReporter[0];

        STARTUPINFOW si2;
        PROCESS_INFORMATION pi2;
        ZeroMemory(&si2, sizeof(si2));
        si2.cb = sizeof(si2);
        ZeroMemory(&pi2, sizeof(pi2));

        if (!CreateProcessW(nullptr,cmd2, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si2, &pi2))
        {
            cerr << "Error: failed to launch Reporter (" << GetLastError() << ")\n";
            return 3;
        }

        WaitForSingleObject(pi2.hProcess, INFINITE);
        CloseHandle(pi2.hProcess);
        CloseHandle(pi2.hThread);
    }

    {
        ifstream in(reportFileName);
        if (!in) {
            cerr << "Error: cannot open report \"" << reportFileName << "\"\n";
            return 4;
        }

        cout << "\nReport file contents:\n";
        string line;
        while (getline(in, line)) {
            cout << "  " << line << "\n";
        }
    }
    return 0;
}
