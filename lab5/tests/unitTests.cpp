#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "employee.h"
#include "message.h"
#include "pipes.h"
#include "ClientHandler.h"

using std::ofstream;
using std::ifstream;
using std::ios;
using std::string;
using std::runtime_error;

// tests binary write and read of employee
TEST(EmployeeTest, WriteAndReadBinary) {
    employee e{ 1, "Ivan", 12.5 };
    ofstream fout("emp.bin", ios::binary);
    e.writeBin(fout);
    fout.close();

    ifstream fin("emp.bin", ios::binary);
    employee e2;
    ASSERT_TRUE(e2.readBin(fin));
    EXPECT_EQ(e2.num, 1);
    EXPECT_STREQ(e2.name, "Ivan");
    EXPECT_DOUBLE_EQ(e2.hours, 12.5);
}

// checks if validation rejects negative hours
TEST(EmployeeTest, ValidateRejectsNegativeHours) {
    employee e{ 2, "Test", -3.0 };
    string err;
    EXPECT_FALSE(e.validate(err));
    EXPECT_EQ(err, "Hours must be non-negative");
}

// message command set and check
TEST(MessageTest, CommandSetAndCheck) {
    Message m;
    m.setCommand("read");
    EXPECT_TRUE(m.isCommand("read"));
    EXPECT_FALSE(m.isCommand("modify"));
}

// ClientHandler throws on nonexistent record
TEST(ClientHandlerTest, ReadNonexistentNumThrows) {
    ofstream fout("file.bin", ios::binary);
    employee e{ 1, "Ivan", 5.0 };
    e.writeBin(fout);
    fout.close();

    ClientHandler handler("file.bin");
    handler.lockRead(999);
    EXPECT_THROW(handler.readRecord(999), runtime_error);
    handler.unlockRead(999);
}
