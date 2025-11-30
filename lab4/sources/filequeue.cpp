#include "filequeue.h"

using std::string;
using std::runtime_error;

static HANDLE createOrOpenMutex(const string& name) {
    HANDLE h = CreateMutexA(nullptr, FALSE, name.c_str());
    if (!h) 
        throw runtime_error("Failed to create/open mutex");
    return h;
}

static HANDLE createOrOpenSemaphore(const string& name, int initial, int max) {
    HANDLE h = CreateSemaphoreA(nullptr, initial, max, name.c_str());
    if (!h) 
        throw runtime_error("Failed to create/open semaphore");
    return h;
}

FileQueue::FileQueue(const string& path, unsigned int capacity) : path_(path) {
    if (capacity == 0) 
        throw runtime_error("Capacity must be greater than 0");
    hFile_ = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile_ == INVALID_HANDLE_VALUE) 
        throw runtime_error("Failed to create file");
    LARGE_INTEGER size;
    size.QuadPart = sizeof(FileHeader) + static_cast<LONGLONG>(capacity) * sizeof(Entry);
    SetFilePointerEx(hFile_, size, nullptr, FILE_BEGIN);
    SetEndOfFile(hFile_);
    hdr_.capacity = capacity;
    hdr_.head = 0;
    hdr_.tail = 0;
    hdr_.count = 0;
    hdr_.readyCount = 0;
    writeHeader();
    Entry e{};
    for (unsigned int i = 0; i < capacity; ++i) 
        writeEntry(i, e);
    hMutex_ = createOrOpenMutex("Global\\MQ_Mutex_" + path);
    hSemaphorMsg_ = createOrOpenSemaphore("Global\\MQ_SemaphorMsg_" + path, 0, capacity);
    hSemaphorSpace_ = createOrOpenSemaphore("Global\\MQ_SemaphorSpace_" + path, capacity, capacity);
}

FileQueue::FileQueue(const string& path) : path_(path) {
    hFile_ = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile_ == INVALID_HANDLE_VALUE) 
        throw runtime_error("Failed to open file");
    readHeader();
    hMutex_ = createOrOpenMutex("Global\\MQ_Mutex_" + path);
    hSemaphorMsg_ = createOrOpenSemaphore("Global\\MQ_SemaphorMsg_" + path, 0, hdr_.capacity);
    hSemaphorSpace_ = createOrOpenSemaphore("Global\\MQ_SemaphorSpace_" + path, hdr_.capacity, hdr_.capacity);
}

FileQueue::~FileQueue() {
    if (hFile_) 
        CloseHandle(hFile_);
    if (hMutex_) 
        CloseHandle(hMutex_);
    if (hSemaphorMsg_) 
        CloseHandle(hSemaphorMsg_);
    if (hSemaphorSpace_) 
        CloseHandle(hSemaphorSpace_);
}

void FileQueue::writeHeader() {
    DWORD written;
    SetFilePointer(hFile_, 0, nullptr, FILE_BEGIN);
    WriteFile(hFile_, &hdr_, sizeof(hdr_), &written, nullptr);
}

void FileQueue::readHeader() {
    DWORD read;
    SetFilePointer(hFile_, 0, nullptr, FILE_BEGIN);
    ReadFile(hFile_, &hdr_, sizeof(hdr_), &read, nullptr);
}

void FileQueue::writeEntry(unsigned int index, const Entry& e) {
    DWORD written;
    SetFilePointer(hFile_, sizeof(FileHeader) + index * sizeof(Entry), nullptr, FILE_BEGIN);
    WriteFile(hFile_, &e, sizeof(e), &written, nullptr);
}

Entry FileQueue::readEntry(unsigned int index) {
    Entry e{};
    DWORD read;
    SetFilePointer(hFile_, sizeof(FileHeader) + index * sizeof(Entry), nullptr, FILE_BEGIN);
    ReadFile(hFile_, &e, sizeof(e), &read, nullptr);
    return e;
}

void FileQueue::lock() {
    WaitForSingleObject(hMutex_, INFINITE);
}
void FileQueue::unlock() {
    ReleaseMutex(hMutex_);
}

void FileQueue::push(const string& msg) {
    WaitForSingleObject(hSemaphorSpace_, INFINITE);
    lock();
    readHeader();
    Entry e{};
    e.state = 1;
    strncpy_s(e.message, sizeof(e.message), msg.c_str(), _TRUNCATE);
    writeEntry(hdr_.tail, e);
    hdr_.tail = (hdr_.tail + 1) % hdr_.capacity;
    hdr_.count++;
    writeHeader();
    unlock();
    ReleaseSemaphore(hSemaphorMsg_, 1, nullptr);
}

string FileQueue::pop() {
    WaitForSingleObject(hSemaphorMsg_, INFINITE);
    lock();
    readHeader();
    Entry e = readEntry(hdr_.head);
    hdr_.head = (hdr_.head + 1) % hdr_.capacity;
    hdr_.count--;
    writeHeader();
    unlock();
    ReleaseSemaphore(hSemaphorSpace_, 1, nullptr);
    return string(e.message);
}

void FileQueue::markSenderReady() {
    lock();
    readHeader();
    hdr_.readyCount++;
    writeHeader();
    unlock();
}

void FileQueue::waitForAllSenders(int expectedCount) {
    for (;;) {
        lock();
        readHeader();
        int ready = hdr_.readyCount;
        unlock();
        if (ready >= expectedCount) 
            break;
        Sleep(sleepTime);
    }
}
