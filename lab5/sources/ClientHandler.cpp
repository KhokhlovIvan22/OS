#include "ClientHandler.h"
#include "message.h"
#include <fstream>

using std::ios;
using std::unique_lock;
using std::streampos;
using std::runtime_error;
using std::exception;
using std::fstream;
using std::ifstream;

ClientHandler::ClientHandler(const string& filename) : filename(filename) {}

ClientHandler::RecordLock& ClientHandler::get_lock(int id) {
    return locks[id];
}

void ClientHandler::lockRead(int id) {
    RecordLock& lock_ = get_lock(id);
    unique_lock<mutex> lk(lock_.m);
    while (lock_.writer)
        lock_.cv.wait(lk);
    lock_.readers++;
}

void ClientHandler::unlockRead(int id) {
    RecordLock& lock_ = get_lock(id);
    unique_lock<mutex> lk(lock_.m);
    lock_.readers--;
    if (lock_.readers == 0) 
        lock_.cv.notify_all();
}

void ClientHandler::lockWrite(int id) {
    RecordLock& lock_ = get_lock(id);
    unique_lock<mutex> lk(lock_.m);
    while (lock_.writer || lock_.readers > 0) 
        lock_.cv.wait(lk);
    lock_.writer = true;
}

void ClientHandler::unlockWrite(int id) {
    RecordLock& lock_ = get_lock(id);
    unique_lock<mutex> lk(lock_.m);
    lock_.writer = false;
    lock_.cv.notify_all();
}

employee ClientHandler::readRecord(int id) {
    ifstream fin(filename, ios::binary);
    if (!fin) 
        throw runtime_error("Failed to open file for reading");
    while (true) {
        employee e;
        e.readBin(fin);
        if (!fin) 
            break;
        if (e.num == id) 
            return e;
    }
    throw runtime_error("Record with this ID not found");
}

void ClientHandler::modify(const employee& updated) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) 
        throw runtime_error("Failed to open file for modification");
    while (true) {
        streampos pos = file.tellg();
        employee e;
        e.readBin(file);
        if (!file) 
            break;
        if (e.num == updated.num) {
            file.seekp(pos);
            updated.writeBin(file);
            return;
        }
    }
    throw runtime_error("Record for modification not found");
}

void ClientHandler::processRequest(HANDLE pipe) {
    while (true) {
        Message req{};
        if (!pipeRead(pipe, &req, sizeof(req))) 
            break;
        Message resp{};
        resp.id = req.id;
        try {
            if (req.isCommand("read")) {
                lockRead(req.id);
                employee e = readRecord(req.id);
                resp.setCommand("read");
                resp.emp = e;
                pipeWrite(pipe, &resp, sizeof(resp));
            }
            else if (req.isCommand("release")) {
                unlockRead(req.id);
                unlockWrite(req.id);
                resp.setCommand("release");
                pipeWrite(pipe, &resp, sizeof(resp));
            }
            else if (req.isCommand("modify")) {
                lockWrite(req.id);
                employee e = readRecord(req.id);
                resp.setCommand("modify");
                resp.emp = e;
                pipeWrite(pipe, &resp, sizeof(resp));
            }
            else if (req.isCommand("commit")) {
                modify(req.emp);
                resp.setCommand("commit");
                resp.emp = req.emp;
                pipeWrite(pipe, &resp, sizeof(resp));
            }
            else if (req.isCommand("exit")) 
                break;
            else {
                resp.setCommand("error");
                pipeWrite(pipe, &resp, sizeof(resp));
            }
        }
        catch (const exception& ex) {
            resp.setCommand("error");
            pipeWrite(pipe, &resp, sizeof(resp));
        }
    }
    FlushFileBuffers(pipe);
    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);
}