#include <gtest/gtest.h>
#include <fstream>
#include "employee.h"
#include "ClientHandler.h"

using std::ofstream;
using std::ifstream;
using std::ios;
using std::runtime_error;

// tests modify → commit → release scenario
TEST(AppScenarioTest, ModifyCommitRelease) {
    ofstream fout("scenario1.bin", ios::binary);
    employee e{ 1, "Ivan", 5.0 };
    e.writeBin(fout);
    fout.close();
    ClientHandler handler("scenario1.bin");
    handler.lockWrite(1);
    employee updated{ 1, "Updated", 7.5 };
    handler.modify(updated);
    handler.unlockWrite(1);
    ifstream fin("scenario1.bin", ios::binary);
    employee e2;
    ASSERT_TRUE(e2.readBin(fin));
    EXPECT_EQ(e2.num, 1);
    EXPECT_STREQ(e2.name, "Updated");
    EXPECT_DOUBLE_EQ(e2.hours, 7.5);
}

// reads existing record
TEST(AppScenarioTest, ReadExistingRecord) {
    ofstream fout("scenario2.bin", ios::binary);
    employee e{ 2, "Baranova", 10.0 };
    e.writeBin(fout);
    fout.close();
    ClientHandler handler("scenario2.bin");
    handler.lockRead(2);
    employee e2 = handler.readRecord(2);
    handler.unlockRead(2);
    EXPECT_EQ(e2.num, 2);
    EXPECT_STREQ(e2.name, "Baranova");
    EXPECT_DOUBLE_EQ(e2.hours, 10.0);
}

// reads nonexistent record
TEST(AppScenarioTest, ReadNonexistentRecord) {
    ofstream fout("scenario3.bin", ios::binary);
    employee e{ 3, "Lohov", 12.0 };
    e.writeBin(fout);
    fout.close();
    ClientHandler handler("scenario3.bin");
    handler.lockRead(999);
    EXPECT_THROW(handler.readRecord(999), runtime_error);
    handler.unlockRead(999);
}