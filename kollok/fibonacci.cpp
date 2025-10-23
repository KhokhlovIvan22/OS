#include "fibonacci.h"
#include <stdexcept>

using std::vector;
using std::invalid_argument;
using std::overflow_error

vector<unsigned long long> fibonacci(int n) {
    if (n == 0) throw invalid_argument("n must be > 0");
    vector<unsigned long long> snumbers;
    numbers.reserve(n);
    unsigned long long a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        seq.push_back(a);
        auto next = a + b;
        if (next < a) throw overflow_error("overflow in fibonacci");
        a = b;
        b = next;
    }
    return numbers;
}