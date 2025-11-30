#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "filequeue.h"

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::cin;
using std::runtime_error;
using std::exception;

static PROCESS_INFORMATION spawnSender(const string& exe, const string& fileName) {
    string cmd = "\"" + exe + "\" \"" + fileName + "\"";
    STARTUPINFOA si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};
    vector<char> buf(cmd.begin(), cmd.end());
    buf.push_back('\0');
    DWORD creationFlags = CREATE_NEW_CONSOLE;
    if (!CreateProcessA(nullptr, buf.data(), nullptr, nullptr, FALSE, creationFlags, nullptr, nullptr, &si, &pi))
        throw runtime_error("CreateProcess(sender) failed");
    return pi;
}

int main() {
    try {
        cout << "           Receiver\n";
        cout << "Enter binary file name: ";
        string fileName;
        getline(cin, fileName);
        cout << "Enter records count (capacity): ";
        string tmp;
        getline(cin, tmp);
        int binFileCapacity = stoi(tmp);
        cout << "Enter Sender processes count: ";
        string tmp2;
        getline(cin, tmp2);
        int senderCount = stoi(tmp2);
        FileQueue q(fileName, binFileCapacity);
        vector<PROCESS_INFORMATION> children;
        children.reserve(senderCount);
        string exeName = ".\\sender.exe";
        for (int i = 0; i < senderCount; ++i) {
            auto pi = spawnSender(exeName, fileName);
            children.push_back(pi);
        }
        cout << "\n";
        cout << "Waiting for all Senders to be ready...\n";
        q.waitForAllSenders(senderCount);
        cout << "All Senders ready\n";
        while (true) {
            cout << "[Receiver] Enter command (read/quit): ";
            string cmd;
            if (!getline(cin, cmd)) 
                break;
            if (cmd == "read") {
                string msg = q.pop();
                if (msg.empty())
                    cout << "[Receiver] No messages\n";
                else 
                    cout << "[Receiver] Message: " << msg << "\n";
            }
            else if (cmd == "quit") {
                cout << "[Receiver] Finishing.\n";
                break;
            }
            else 
                cout << "Unknown command.\n";
        }
        for (auto& pi : children) {
            TerminateProcess(pi.hProcess, 0);
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
        }
        return 0;
    }
    catch (const exception& ex) {
        cerr << "Runtime error: " << ex.what() << "\n";
        return 1;
    }
}