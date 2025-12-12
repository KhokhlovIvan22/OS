#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "employee.h"
#include "message.h"
#include "pipes.h"

using std::cin;
using std::cout;
using std::cerr;
using std::string;
using std::exception;

const char* PIPE_NAME = R"(\\.\pipe\EmployeePipe)";

int main() {
    try {
        HANDLE hPipe = CreateFileA(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hPipe == INVALID_HANDLE_VALUE) {
            cerr << "Failed to connect to server pipe\n";
            return 1;
        }
        while (true) {
            cout << "\nChoose operation:\n" << "1 - modify record\n" << "2 - read record\n" << "3 - exit\n" << "Enter your choice: ";
            int choice;
            if (!(cin >> choice)) {
                cerr << "Invalid input\n";
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            switch (choice) {
            case 1: {
                int id;
                cout << "Enter employee ID: ";
                cin >> id;
                Message req{};
                req.setCommand("modify");
                req.id = id;
                if (!pipeWrite(hPipe, &req, sizeof(req))) {
                    cerr << "Pipe write failed\n";
                    break;
                }
                Message resp{};
                if (!pipeRead(hPipe, &resp, sizeof(resp))) {
                    cerr << "Pipe read failed\n";
                    break;
                }
                if (resp.isCommand("modify"))
                    cout << "Current record: " << resp.emp << "\n";
                else {
                    cerr << "Modify request failed\n";
                    break;
                }
                cout << "Enter new values (ID Name Hours): ";
                employee updated;
                cin >> updated;
                cout << "Type 'commit' to send changes: ";
                string cmd;
                cin >> cmd;
                if (cmd == "commit") {
                    Message commit{};
                    commit.setCommand("commit");
                    commit.id = id;
                    commit.emp = updated;
                    if (!pipeWrite(hPipe, &commit, sizeof(commit))) {
                        cerr << "Pipe write failed\n";
                        break;
                    }
                    if (!pipeRead(hPipe, &resp, sizeof(resp))) {
                        cerr << "Pipe read failed\n";
                        break;
                    }
                    if (resp.isCommand("commit"))
                        cout << "Modification saved\n";
                    else
                        cerr << "Commit failed\n";
                }
                cout << "Type 'release' to finish access: ";
                cin >> cmd;
                if (cmd == "release") {
                    Message rel{};
                    rel.setCommand("release");
                    rel.id = id;
                    if (!pipeWrite(hPipe, &rel, sizeof(rel))) {
                        cerr << "Pipe write failed\n";
                        break;
                    }
                    if (!pipeRead(hPipe, &resp, sizeof(resp))) {
                        cerr << "Pipe read failed\n";
                        break;
                    }
                }
                break;
            }
            case 2: {
                int id;
                cout << "Enter employee ID: ";
                cin >> id;
                Message req{};
                req.setCommand("read");
                req.id = id;
                if (!pipeWrite(hPipe, &req, sizeof(req))) {
                    cerr << "Pipe write failed\n";
                    break;
                }
                Message resp{};
                if (!pipeRead(hPipe, &resp, sizeof(resp))) {
                    cerr << "Pipe read failed\n";
                    break;
                }
                if (resp.isCommand("read"))
                    cout << "Record received: " << resp.emp << "\n";
                else
                    cerr << "Read failed\n";
                cout << "Type 'release' to finish access: ";
                string cmd;
                cin >> cmd;
                if (cmd == "release") {
                    Message rel{};
                    rel.setCommand("release");
                    rel.id = id;
                    if (!pipeWrite(hPipe, &rel, sizeof(rel))) {
                        cerr << "Pipe write failed\n";
                        break;
                    }
                    if (!pipeRead(hPipe, &resp, sizeof(resp))) {
                        cerr << "Pipe read failed\n";
                        break;
                    }
                }
                break;
            }
            case 3: {
                Message msg{};
                msg.setCommand("exit");
                if (!pipeWrite(hPipe, &msg, sizeof(msg)))
                    cerr << "Pipe write failed\n";
                CloseHandle(hPipe);
                return 0;
            }
            default:
                cerr << "Unknown choice\n";
                break;
            }
        }
    }
    catch (const exception& ex) {
        cerr << "Client error: " << ex.what() << "\n";
        return 2;
    }
}