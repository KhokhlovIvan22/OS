#pragma once
#include <windows.h>

// Write data to pipe, returns true if all bytes written
bool pipeWrite(HANDLE h, const void* data, int bytes);

// Read data from pipe, returns true if all bytes read
bool pipeRead(HANDLE h, void* data, int bytes);