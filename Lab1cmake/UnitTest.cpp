#include <gtest/gtest.h>
#include "employee.h"
#include "helpers.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
using std::vector;
using std::string;
using std::wstring;
using std::ifstream;
using std::ofstream;
using std::ostringstream;
using std::ios;
using std::to_wstring;

class FileTest : public ::testing::Test {
protected:
    vector<wstring> tempFiles;
    void TearDown() override {
        for (auto& f : tempFiles) {
            if (fileExists(f)) deleteFile(f);
        }
    }
    void track(const wstring& f) { tempFiles.push_back(f); 
    }
};

//checks the writing of structures into bin and reading
TEST_F(FileTest, ReadWriteBinary) {
    const string bin = "test_employees.bin";
    track(wstring(bin.begin(), bin.end()));
    { 
        ofstream out(bin, ios::binary | ios::trunc);
        employee a{ 1, "Alice", 2.0 };
        employee b{ 2, "Bob", 3.5 };
        out.write(reinterpret_cast<char*>(&a), sizeof(a));
        out.write(reinterpret_cast<char*>(&b), sizeof(b));
    }
    vector<employee> records;
    int rc = readFromBinary(bin, records);
    EXPECT_EQ(rc, 0);
    ASSERT_EQ(records.size(), 2u);
    EXPECT_EQ(records[0].num, 1);
    EXPECT_STREQ(records[0].name, "Alice");
    EXPECT_DOUBLE_EQ(records[1].hours, 3.5);
}

//checks the correctness of the data in the report
TEST_F(FileTest, WriteReportContainsSalary) {
    const string bin = "test_report.bin";
    const string txt = "test_report.txt";
    track(wstring(bin.begin(), bin.end()));
    track(wstring(txt.begin(), txt.end()));
    { 
        ofstream out(bin, ios::binary | ios::trunc);
        employee e{ 1, "X", 4.0 };
        out.write(reinterpret_cast<char*>(&e), sizeof(e));
    }
    vector<employee> emps;
    readFromBinary(bin, emps);
    int rc = writeReport(txt, bin, emps, 2.0);
    EXPECT_EQ(rc, 0);
    auto lines = ReadTextLines(wstring(txt.begin(), txt.end()));
    bool found = false;
    for (auto& l : lines) {
        if (l.find(L"8.00") != wstring::npos) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

//launches Creator.exe with zero employees
TEST_F(FileTest, CreatorRejectsZeroCount) {
    const wstring bin = L"test0.bin";
    track(bin);
    DWORD code = RunProcess(L"Creator.exe " + bin + L" 0");
    EXPECT_NE(code, 0u);
    EXPECT_FALSE(fileExists(bin));
}

//launches Reporter.exe with a negative rate
TEST_F(FileTest, ReporterRejectsNegativeRate) {
    const wstring bin = L"test_reporter.bin";
    track(bin);
    { 
        ofstream out(bin, ios::binary | ios::trunc);
        employee a{ 1, "X", 1.0 };
        out.write(reinterpret_cast<char*>(&a), sizeof(a));
    }
    DWORD code = RunProcess(L"Reporter.exe " + bin + L" report.txt -5");
    EXPECT_NE(code, 0u);
}
