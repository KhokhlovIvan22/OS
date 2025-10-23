#include "palindrom.h"
#include <stdexcept>
#include <climits>
using std::overflow_error;

bool is_palindrome(long long x) {
    if (x < 0) return false;
    long long x1 = x, rev = 0;
    int d=0
    while (x > 0) {
        d = x % 10;
        rev = rev * 10 + d;
        x /= 10;
    }
    return orig == rev;
}