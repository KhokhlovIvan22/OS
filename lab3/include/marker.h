#pragma once
#include <thread>
#include <condition_variable>
#include <memory>
#include <ostream>
#include "Array.h"

using std::shared_ptr;
using std::thread;
using std::mutex;
using std::condition_variable;
using std::ostream;

class Marker {
private:
    int id_;
    shared_ptr<Array> arr;
    thread th;
    bool blocked = false;
    bool finished = false;
    mutex mtx;
    static mutex io;
    condition_variable cv;
    const int sleepTime = 5;
    void run(ostream&out);
public:
    Marker(int id, shared_ptr<Array> arr);
    ~Marker();
    void start(ostream&out);
    void join();
    void unblock();
    void finish();
    int id() const;
    bool isFinished() const;
    bool isBlocked() const;
};
