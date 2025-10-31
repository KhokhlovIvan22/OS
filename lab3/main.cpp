#include "Array.h"
#include "Marker.h"
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>

using std::cout;
using std::cin;
using std::make_shared;
using std::unique_ptr;
using std::vector;
using std::make_unique;
using std::invalid_argument;
using std::runtime_error;
using std::exception;
using std::cerr;

int main() {
    try {
        int size;
        cout << "Enter array size: ";
        cin >> size;
        auto arr = make_shared<Array>(size);
        int markerCount = 0;
        cout << "Enter number of markers: ";
        if (!(cin >> markerCount) || markerCount <= 0)
            throw invalid_argument("Number of markers must be positive");
        vector<unique_ptr<Marker>> markers;
        markers.reserve(markerCount);
        for (int i = 1; i <= markerCount; ++i) {
            markers.push_back(make_unique<Marker>(i, arr));
            markers.back()->start(cout);
        }
        int active = markerCount;
        while (active > 0) {
            bool allBlocked = false;
            while (!allBlocked) {
                allBlocked = true;
                for (auto& m : markers)
                    if (m && !m->isFinished() && !m->isBlocked()) {
                        allBlocked = false;
                        break;
                    }
            }
            cout << "\nCurrent array state:\n";
            arr->print(cout);
            int terminateId = -1;
            cout << "\nEnter marker ID to terminate: ";
            if (!(cin >> terminateId))
                throw runtime_error("Invalid input for marker ID");
            bool found = false;
            for (auto& m : markers)
                if (m && m->id() == terminateId) {
                    m->finish();
                    m->join();
                    --active;
                    found = true;
                    break;
                }
            if (!found)
                cout << "No marker with ID " << terminateId << " found\n";
            else {
                cout << "\nArray after termination:\n";
                arr->print(cout);
                for (auto& m: markers)
                    if (m && !m->isFinished())
                        m->unblock();
            }
        }
        cout << "\n\nAll markers finished. Final array state:\n";
        arr->print(cout);
    }
    catch (const exception& e)
    {
        cerr << "Error! " << e.what() << "\n";
    }
    return 0;
}