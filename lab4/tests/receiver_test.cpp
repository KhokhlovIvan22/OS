#include <gtest/gtest.h>
#include "filequeue.h"

using std::string;

TEST(ReceiverTest, ReceivesMessage) {
    FileQueue q("test_receiver2.bin", 3);
    q.push("FAMCS");
    string msg = q.pop();
    EXPECT_EQ(msg, "FAMCS");
}