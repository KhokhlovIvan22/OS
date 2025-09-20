#include "pch.h"
#include "helpers.h"
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
using std::vector;
using std::wstring;
using std::ifstream;
using std::string;
using std::ostringstream;
using std::ios;
using std::ofstream;
using std::to_wstring;

TEST_CLASS(CreatorTests)//runs Creator.exe with a valid file name and number of records
{
public:

    TEST_METHOD(CREATOR_OK_RecordCount)//runs Creator.exe with a valid file name and number of records
    {
        const wstring bin = L"test_creator.bin";
        if (fileExists(bin)) deleteFile(bin);
        const int N = 3;
        ostringstream oss;
        for (int i = 1; i <= N; ++i) {
            oss << i << "\n" << "Emp" << i << "\n" << (i * 1.5) << "\n";
        }
        DWORD code = RunProcessInput(L"Creator.exe " + bin + L" " + to_wstring(N), oss.str());
        Assert::AreEqual<UINT>(0u, code);

        vector<employee> records;
        int rc = readFromBinary(string(bin.begin(), bin.end()), records);
        Assert::AreEqual(0, rc);
        Assert::AreEqual<size_t>((size_t)N, records.size());
    }

    TEST_METHOD(CREATOR_Error_ZeroCount)//runs Creator.exe with count 0, expects a nonzero exit code
    {
        const wstring bin = L"test0.bin";
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
        const wstring bin = L"test_reporter.bin";
        {
            ofstream out(bin, ios::binary | ios::trunc);
            employee a{ 1, "A", 2.0 };
            employee b{ 2, "B", 3.5 };
            out.write(reinterpret_cast<char*>(&a), sizeof(a));
            out.write(reinterpret_cast<char*>(&b), sizeof(b));
        }

        const wstring report = L"test_reporter.txt";
        if (fileExists(report)) deleteFile(report);

        DWORD code = RunProcess(L"Reporter.exe " + bin + L" " + report + L" 2.0");
        Assert::AreEqual<UINT>(0u, code);
        Assert::IsTrue(fileExists(report));

        auto lines = ReadTextLines(report);
        bool found = false;
        for (auto const& ln : lines) {
            if (ln.find(L"4.00") != wstring::npos) {
                found = true;
                break;
            }
        }
        Assert::IsTrue(found);
    }


    TEST_METHOD(REPORTER_Error_InvalidRate)//runs Reporter.exe with a negative rate, expects a nonzero code
    {
        const wstring bin = L"test_reporter.bin";
        {
            ofstream out(bin, ios::binary | ios::trunc);
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
        DWORD code = RunProcessInput(L"Main.exe", string("f.bin\n0\n"));
        Assert::IsTrue(code != 0);
    }

    TEST_METHOD(MAIN_Error_InvalidRate)//runs Main.exe with a negative rate, expects a nonzero code
    {
        int N = 1;
        ostringstream oss;
        oss << "f.bin\n" << N << "\n" << "1\nName\n2\n" << "out.txt\n" << "-2.5\n";

        DWORD code = RunProcessInput(L"Main.exe", oss.str());
        Assert::IsTrue(code != 0);
    }
};
