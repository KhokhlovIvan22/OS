#include <gtest/gtest.h>
#include "filequeue.h"

using std::string;

TEST(SenderTest, PushesMessageIntoQueue) {
    FileQueue q("test_sender.bin", 3);
    q.push("FAMCS");
    string msg = q.pop();
    EXPECT_EQ(msg, "FAMCS");
}