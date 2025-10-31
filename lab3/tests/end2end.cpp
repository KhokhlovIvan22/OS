#include <gtest/gtest.h>
#include "Array.h"
#include "Marker.h"
#include <sstream>
#include <thread>
#include <vector>
#include <memory>
#include <chrono>

using std::ostringstream;
using std::string;
using std::thread;
using std::unique_ptr;
using std::make_unique;
using std::make_shared;
using std::shared_ptr;
using std::vector;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

TEST(EndToEnd, MainScenario) {
    auto arr = make_shared<Array>(5);
    ostringstream output;
    vector<unique_ptr<Marker>> markers;
    for (int i = 1; i <= 3; ++i) {
        markers.push_back(make_unique<Marker>(i, arr));
        markers.back()->start(output);
    }
    sleep_for(milliseconds(200));
    for (auto& m: markers) 
        m->finish();
    for (auto& m: markers) 
        m->join();
    for (int i = 0; i < arr->size(); ++i) {
        EXPECT_EQ((*arr)[i], 0);
    }
    string out = output.str();
    EXPECT_NE(out.find("finished"), string::npos);
}