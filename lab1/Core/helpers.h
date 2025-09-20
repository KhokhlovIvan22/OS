#pragma once
#include <string>
#include <vector>
#include <Windows.h>

using std::wstring;
using std::vector;
using std::string;
bool fileExists(const wstring& path);

void deleteFile(const wstring& path);

// reads all lines from text file
vector<wstring> ReadTextLines(const wstring& txtPath);

// starts the process without input and returns exit code
DWORD RunProcess(const wstring& commandLine);

// starts the process, feeds inputData to STDIN and returns exit code
DWORD RunProcessInput(const wstring& commandLine, const string& inputData);