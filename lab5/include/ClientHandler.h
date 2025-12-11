#pragma once
#include <string>
#include <map>
#include <mutex>
#include <condition_variable>
#include "employee.h"
#include "message.h"
#include "pipes.h"

using std::string;
using std::mutex;
using std::condition_variable;
using std::map;

class ClientHandler {
public:
    explicit ClientHandler(const string& filename);

    void lockRead(int id);
    void unlockRead(int id);
    void lockWrite(int id);
    void unlockWrite(int id);

    employee readRecord(int id);
    void modify(const employee& updated);

    void processRequest(HANDLE pipe);

private:
    struct RecordLock {
        mutex m;
        condition_variable cv;
        int readers = 0;
        bool writer = false;
    };

    string filename;
    map<int, RecordLock> locks;

    RecordLock& get_lock(int id);
};