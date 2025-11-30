#pragma once
#include <windows.h>
#include <string>
#include <stdexcept>

using std::string;
using std::runtime_error;

const int sleepTime = 50;

struct Entry {
    unsigned int state;
    char message[20];
};

struct FileHeader {
    unsigned int capacity;
    unsigned int head;
    unsigned int tail;
    unsigned int count;
    int readyCount;
};

class FileQueue {
public:
    FileQueue(const string& path, unsigned int capacity);
    FileQueue(const string& path);
    void push(const string& msg);
    string pop();
    void markSenderReady();
    void waitForAllSenders(int expectedCount);
    ~FileQueue();

private:
    HANDLE hFile_{ nullptr };
    HANDLE hMutex_{ nullptr };
    HANDLE hSemaphorMsg_{ nullptr };
    HANDLE hSemaphorSpace_{ nullptr };
    string path_;
    FileHeader hdr_{};
    void writeHeader();
    void readHeader();
    void writeEntry(unsigned int index, const Entry& e);
    Entry readEntry(unsigned int index);
    void lock();
    void unlock();
};
