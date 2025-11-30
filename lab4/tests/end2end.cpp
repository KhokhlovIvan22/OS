#include <gtest/gtest.h>
#include <thread>
#include "filequeue.h"

using std::string;
using std::thread;

TEST(EndToEndTest, ReceiverGetsMessageFromSender) {
    FileQueue q("test_end2end.bin", 3);
    thread sender([&q]() {q.push("FAMCS");});
    string msg = q.pop();
    EXPECT_EQ(msg, "FAMCS");
    sender.join();
}
