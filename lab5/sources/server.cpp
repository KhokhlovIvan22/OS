#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include "employee.h"
#include "ClientHandler.h"
#include "message.h"
#include "pipes.h"

using std::cin;
using std::cout;
using std::cerr;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::thread;

const char* PIPE_NAME = R"(\\.\pipe\EmployeePipe)";

static void printFile(const string& filename, const char* title) {
    cout << "\n" << title << "\n";
    ifstream fin(filename, std::ios::binary);
    if (!fin) {
        cerr << "Failed to open file\n";
        return;
    }
    employee e;
    while (e.readBin(fin)) 
        cout << e << "\n";
}

int main() {
    string filename;
    cout << "Enter binary file name: ";
    cin >> filename;
    int n;
    cout << "Enter number of employees: ";
    cin >> n;
    ofstream fout(filename, std::ios::binary);
    for (int i = 0; i < n; ++i) {
        employee e;
        cout << "Enter employee (ID Name Hours): ";
        cin >> e;
        string err;
        if (!e.validate(err)) {
            cerr << "Invalid employee: " << err << "\n";
            --i;
            continue;
        }
        e.writeBin(fout);
    }
    fout.close();
    printFile(filename, "Created file:");
    int clients;
    cout << "\nEnter number of client processes: ";
    cin >> clients;
    ClientHandler handler(filename);
    vector<thread> workers;
    for (int i = 0; i < clients; ++i) {
        HANDLE hPipe = CreateNamedPipeA(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, clients, sizeof(Message), sizeof(Message), 0, nullptr);
        if (hPipe == INVALID_HANDLE_VALUE) {
            cerr << "Failed to create named pipe. Error code: " << GetLastError() << "\n";
            continue;
        }
        STARTUPINFOA si{};
        PROCESS_INFORMATION pi{};
        si.cb = sizeof(si);
        if (!CreateProcessA("client.exe", nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            cerr << "Failed to start client process. Error code: " << GetLastError() << "\n";
            CloseHandle(hPipe);
            continue;
        }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        BOOL connected = ConnectNamedPipe(hPipe, nullptr);
        if (!connected && GetLastError() == ERROR_PIPE_CONNECTED)
            connected = TRUE;
        if (connected)
            workers.emplace_back([hPipe, &handler]() { handler.processRequest(hPipe); });
        else
            CloseHandle(hPipe);
    }
    for (auto& t : workers) 
        if (t.joinable())
            t.join();
    printFile(filename, "Modified file:");
    cout << "All clients finished. Server is shutting down.\n";
    return 0;
}