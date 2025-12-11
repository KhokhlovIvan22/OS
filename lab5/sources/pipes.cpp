#include "pipes.h"

bool pipeWrite(HANDLE h, const void* data, int bytes) {
    DWORD written = 0;
    return WriteFile(h, data, static_cast<DWORD>(bytes), &written, nullptr) && static_cast<int>(written) == bytes;
}

bool pipeRead(HANDLE h, void* data, int bytes) {
    DWORD read = 0;
    return ReadFile(h, data, static_cast<DWORD>(bytes), &read, nullptr) && static_cast<int>(read) == bytes;
}