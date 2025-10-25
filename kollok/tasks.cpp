#include "tasks.h"
#include "list.h"
#include <vector>
#include <stdexcept>
using std::vector;
using std::invalid_argument;
using std::overflow_error;

vector<unsigned long long> fibonacci(int n) {
    if (n <= 0)
        throw invalid_argument("Entered numbers amount must be > 0");
    vector<unsigned long long> numbers;
    numbers.reserve(n);
    unsigned long long a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        numbers.push_back(a);
        auto next = a + b;
        if (next < a) 
            throw overflow_error("Error! Overflow in calculating Fibonacci numbers");
        a = b;
        b = next;
    }
    return numbers;
}

bool isPalindrome(long long x) {
    if (x < 0)
        throw invalid_argument("Palindrome check is defined only for x >= 0");
    long long x_copy = x, rev = 0;
    int d = 0;
        while (x > 0) {
            d = x % 10;
            rev = rev * 10 + d;
            x /= 10;
        }
    return x_copy == rev;
}

List reverseList(const List& orig) {
    List reversed;
    auto current = orig.getHead();
    vector<int> values;
    while (current != nullptr) {
        values.push_back(current->value);
        current = current->next;
    }
    for (int i = values.size() - 1; i >= 0; --i)
        reversed.push_back(values[i]);
    return reversed;
}