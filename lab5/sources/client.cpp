#include <windows.h>
#include <iostream>
#include <string>
#include "employee.h"
#include "message.h"
#include "pipes.h"

using std::cin;
using std::cout;
using std::cerr;
using std::string;

const char* PIPE_NAME = R"(\\.\pipe\EmployeePipe)";

int main() {
    HANDLE hPipe = CreateFileA(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hPipe == INVALID_HANDLE_VALUE) {
        cerr << "Failed to connect to server pipe\n";
        return 1;
    }
    while (true) {
        cout << "\nChoose operation:\n" << "1 - modify record\n" << "2 - read record\n" << "3 - exit\n" << "Enter your choice: ";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1: {
            int id;
            cout << "Enter employee ID: ";
            cin >> id;
            Message req{};
            req.setCommand("modify");
            req.id = id;
            pipeWrite(hPipe, &req, sizeof(req));
            Message resp{};
            pipeRead(hPipe, &resp, sizeof(resp));
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
                pipeWrite(hPipe, &commit, sizeof(commit));
                pipeRead(hPipe, &resp, sizeof(resp));
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
                pipeWrite(hPipe, &rel, sizeof(rel));
                pipeRead(hPipe, &resp, sizeof(resp));
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
            pipeWrite(hPipe, &req, sizeof(req));
            Message resp{};
            pipeRead(hPipe, &resp, sizeof(resp));
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
                pipeWrite(hPipe, &rel, sizeof(rel));
                pipeRead(hPipe, &resp, sizeof(resp));
            }
            break;
        }
        case 3: {
            Message msg{};
            msg.setCommand("exit");
            pipeWrite(hPipe, &msg, sizeof(msg));
            CloseHandle(hPipe);
            return 0;
        }
        default:
            cerr << "Unknown choice\n";
            break;
        }
    }
}