#include "Marker.h"
#include <iostream>
#include <thread>
#include <chrono>

using std::move;
using std::unique_ptr;
using std::thread;
using std::lock_guard;
using std::mutex;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::ostream;
using std::unique_lock;

Marker::Marker(int id, shared_ptr<Array> arr): id_(id), arr(move(arr)) {}


Marker::~Marker() {
    if (th.joinable()) 
        th.join();
}

void Marker::run(ostream& out) {
    srand(id_);
    int counter = 0;
    while (true) {
        int index = rand() % arr->size();
        if (arr->tryMark(index, id_)) {
            ++counter;
            sleep_for(milliseconds(sleepTime));
            continue;
        }
        out << "\nMarker " << id_ << " blocked\n" << "Marked " << counter << " elements\n" << "Failed to mark element at " << index << "\n";
        unique_lock<mutex> lk(mtx);
        blocked = true;
        cv.wait(lk, [this] { return !blocked || finished; });
        if (finished) {
            out << "Marker " << id_ << " cleaning up...\n";
            arr->reset(id_);
            out << "Marker " << id_ << " finished.\n";
            break;
        }
    }
}

void Marker::start() {th = std::thread(&Marker::run, this);}

void Marker::join() {
    if (th.joinable()) 
        th.join();
}

void Marker::unblock() {
    {
        lock_guard<mutex> lk(mtx);
        blocked = false;
    }
    cv.notify_one();
}

void Marker::finish() {
    {
        lock_guard<mutex> lk(mtx);
        finished = true;
        blocked = false;
    }
    cv.notify_one();
}

int  Marker::id() const { return id_; }
