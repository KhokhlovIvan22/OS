#include "pch.h"
#include "CppUnitTest.h"
#include "employee.h"
#include <windows.h>
#include <fstream>
#include <io.h>       
#include <cstdio>     
#include <string>
#include <vector>
#include <set>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static bool fileExists(const std::wstring& path) {
    return _waccess(path.c_str(), 0) == 0;
}

static void deleteFile(const std::wstring& path) {
    _wremove(path.c_str());
}

// reads all data from binary file
static std::vector<employee> ReadEmployees(const std::wstring& binPath) {
    std::vector<employee> v;
    std::ifstream in(binPath, std::ios::binary);
    Assert::IsTrue(in.is_open());
    employee e;
    while (in.read(reinterpret_cast<char*>(&e), sizeof(e))) 
        v.push_back(e);
    return v;
}

// reads all lines from text file
static std::vector<std::wstring> ReadTextLines(const std::wstring& txtPath) {
    std::vector<std::wstring> lines;
    std::ifstream in(txtPath);
    Assert::IsTrue(in.is_open());
    std::string tmp;
    while (std::getline(in, tmp)) {
        lines.emplace_back(tmp.begin(), tmp.end());
    }
    return lines;
}

// starts the process without input and returns exit code
static DWORD RunProcess(const std::wstring& commandLine)
{
    std::wstring cmd = commandLine;
    cmd.push_back(L'\0');
    LPWSTR cmdPtr = &cmd[0];

    STARTUPINFO si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};

    BOOL ok = CreateProcess(nullptr, cmdPtr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
    Assert::IsTrue(ok);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return code;
}

// starts the process, feeds inputData to STDIN and returns exit code
static DWORD RunProcessInput(const std::wstring& commandLine, const std::string& inputData)
{
    SECURITY_ATTRIBUTES sa{sizeof(sa), nullptr, TRUE};
    HANDLE hRead = nullptr, hWrite = nullptr;
    Assert::IsTrue(CreatePipe(&hRead, &hWrite, &sa, 0));

    DWORD written = 0;
    WriteFile(hWrite, inputData.data(),(DWORD)inputData.size(), &written, nullptr);
    CloseHandle(hWrite);

    std::wstring cmd = commandLine;
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
    Assert::IsTrue(ok);
    CloseHandle(hRead);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return code;
}

TEST_CLASS(CreatorTests)//runs Creator.exe with a valid file name and number of records
{
public:

    TEST_METHOD(CREATOR_OK_RecordCount)
    {
        const std::wstring bin = L"test_creator.bin";
        if (fileExists(bin)) deleteFile(bin);
        const int N = 3;
        std::ostringstream oss;
        for (int i = 1; i <= N; ++i) {
            oss << i << "\n" << "Emp" << i << "\n"<< (i * 1.5) << "\n";
        }
        DWORD code = RunProcessInput(L"Creator.exe " + bin + L" " + std::to_wstring(N),oss.str());
        Assert::AreEqual<UINT>(0u, code);

        auto records = ReadEmployees(bin);
        Assert::AreEqual<size_t>((size_t)N, records.size());
    }

    TEST_METHOD(CREATOR_Error_ZeroCount)//runs Creator.exe with count 0, expects a nonzero exit code
    {
        const std::wstring bin = L"test0.bin";
        if (fileExists(bin)) deleteFile(bin);

        DWORD code = RunProcess(L"Creator.exe " + bin + L" 0");
        Assert::IsTrue(code != 0);
        Assert::IsFalse(fileExists(bin));
    }

    TEST_METHOD(CREATOR_Error_NonNumericCount)//runs Creator.exe with a non-numeric count argument, expects a nonzero exit code
    {
        DWORD code = RunProcess(L"Creator.exe test.bin abc");
        Assert::IsTrue(code != 0);
    }
};

TEST_CLASS(ReporterTests)
{
public:

    TEST_METHOD(REPORTER_Calc)//runs Reporter.exe with valid rate, checks data accuracy in report
    {
        const std::wstring bin = L"test_reporter.bin";
        {
            std::ofstream out(bin, std::ios::binary | std::ios::trunc);
            employee a{ 1, "A", 2.0 };
            employee b{ 2, "B", 3.5 };
            out.write(reinterpret_cast<char*>(&a), sizeof(a));
            out.write(reinterpret_cast<char*>(&b), sizeof(b));
        }

        const std::wstring report = L"test_reporter.txt";
        if (fileExists(report)) deleteFile(report);

        DWORD code = RunProcess(L"Reporter.exe " + bin + L" " + report + L" 2.0");
        Assert::AreEqual<UINT>(0u, code);
        Assert::IsTrue(fileExists(report));

        auto lines = ReadTextLines(report);
        bool found = false;
        for (auto const& ln : lines) {
            if (ln.find(L"4.00") != std::wstring::npos) {
                found = true;
                break;
            }
        }
        Assert::IsTrue(found);
    }

    TEST_METHOD(REPORTER_Error_InvalidRate)//runs Reporter.exe with a negative rate, expects a nonzero code
    {
        const std::wstring bin = L"test_reporter.bin";
        {
            std::ofstream out(bin, std::ios::binary | std::ios::trunc);
            employee a{ 1, "X", 1.0 };
            out.write(reinterpret_cast<char*>(&a), sizeof(a));
        }

        DWORD code = RunProcess(L"Reporter.exe " + bin + L" report.txt -5");
        Assert::IsTrue(code != 0);
    }
};

TEST_CLASS(MainTests)
{
public:

    TEST_METHOD(MAIN_Error_InvalidCount)//runs Main.exe with a 0-record count, expects a nonzero code
    {
        DWORD code = RunProcessInput(L"Main.exe", std::string("f.bin\n0\n"));
        Assert::IsTrue(code != 0);
    }

    TEST_METHOD(MAIN_Error_InvalidRate)//runs Main.exe with a negative rate, expects a nonzero code
    {
        int N = 1;
        std::ostringstream oss;
        oss << "f.bin\n" << N << "\n" << "1\nName\n2\n" << "out.txt\n" << "-2.5\n";

        DWORD code = RunProcessInput(L"Main.exe", oss.str());
        Assert::IsTrue(code != 0);
    }
};
