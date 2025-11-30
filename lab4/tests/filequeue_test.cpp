#include <gtest/gtest.h>
#include "filequeue.h"

using std::string;
using std::runtime_error;

TEST(FileQueueTest, PushPopSingleMessage) {
    FileQueue q("test1.bin", 3);
    q.push("FAMCS");
    string msg = q.pop();
    EXPECT_EQ(msg, "FAMCS");
}

TEST(FileQueueTest, MultipleMessages) {
    FileQueue q("test2.bin", 5);
    q.push("FAMCS");
    q.push("OS");
    EXPECT_EQ(q.pop(), "FAMCS");
    EXPECT_EQ(q.pop(), "OS");
}