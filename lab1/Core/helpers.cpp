#include "pch.h" 
#include "helpers.h"
#include <io.h>
#include <fstream>
#include <cassert>
using std::wstring;
using std::vector; 
using std::ifstream;
using std::string;

bool fileExists(const wstring& path) {
    return _waccess(path.c_str(), 0) == 0;
}

void deleteFile(const wstring& path) {
    _wremove(path.c_str());
}

vector<wstring> ReadTextLines(const wstring& txtPath) {
    vector<wstring> lines;
    ifstream in(txtPath);
    assert(in.is_open());
    string tmp;
    while (getline(in, tmp)) {
        lines.emplace_back(tmp.begin(), tmp.end());
    }
    return lines;
}

DWORD RunProcess(const wstring& commandLine) {
    wstring cmd = commandLine;
    cmd.push_back(L'\0');
    LPWSTR cmdPtr = &cmd[0];

    STARTUPINFO si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};

    BOOL ok = CreateProcess(nullptr, cmdPtr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    assert(ok);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return code;
}

DWORD RunProcessInput(const wstring& commandLine, const string& inputData) {
    SECURITY_ATTRIBUTES sa{ sizeof(sa), nullptr, TRUE };
    HANDLE hRead = nullptr, hWrite = nullptr;
    assert(CreatePipe(&hRead, &hWrite, &sa, 0));

    DWORD written = 0;
    WriteFile(hWrite, inputData.data(), (DWORD)inputData.size(), &written, nullptr);
    CloseHandle(hWrite);

    wstring cmd = commandLine;
    cmd.push_back(L'\0');
    LPWSTR cmdPtr = &cmd[0];

    STARTUPINFO si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hRead;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcess(nullptr, cmdPtr, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);
    assert(ok);
    CloseHandle(hRead);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return code;
}
