#include <iostream>
#include <string>
#include <stdexcept>
#include "filequeue.h"

using std::string;
using std::cout;
using std::cerr;
using std::cin;
using std::exception;

int main(int argc, char* argv[]) {
    try {
        cout << "               Sender\n";
        if (argc < 2) {
            cerr << "[Sender] No binary file name provided.\n" << "Please start the program as: sender.exe <fileName>\n";
            return 1;
        }
        string fileName = argv[1];
        FileQueue q(fileName);
        q.markSenderReady();
        cout << "[Sender] Ready\n";
        while (true) {
            cout << "[Sender] Command (send/quit): ";
            string cmd;
            if (!getline(cin, cmd)) 
                break;
            if (cmd == "send") {
                cout << "Input message (<20 symbols): ";
                string msg;
                if (!getline(cin, msg)) 
                    break;
                if (msg.size() >= 20) {
                    cout << "[Sender] Message is too long.\n";
                    continue;
                }
                q.push(msg);
                cout << "[Sender] Message sent.\n";
            }
            else if (cmd == "quit") {
                cout << "[Sender] Quitting.\n";
                break;
            }
            else 
                cout << "Unknown command.\n";
        }
        return 0;
    }
    catch (const exception& ex) {
        cerr << "Runtime error: " << ex.what() << "\n";
        return 1;
    }
}
