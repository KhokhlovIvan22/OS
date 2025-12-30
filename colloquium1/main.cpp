#include <iostream>
#include <stdexcept>
#include "tasks.h"
#include "list.h"
using std::cin;
using std::cout;
using std::exception;

int main() {
    try {
        cout << "Task 1: Fibonacci numbers\n";
        cout << "Enter wanted number of Fibonacci numbers: ";
        int n = 0;
        cin >> n; 
        auto numbers = fibonacci(n);
        cout << "First " << n << " Fibonacci numbers: ";
        for (auto x: numbers) cout << x << " ";
        cout << "\n\n";
    }
    catch (const exception& e) {
        cout << "[Fibonacci] Error: " << e.what() << "\n";
    }

    try {
        cout << "Task 2: palindrome\n";
        cout << "Enter a number to check palindrome: ";
        long long x = 0;
        cin >> x;
        cout << x << (isPalindrome(x) ? " is" : " is not") << " a palindrome\n";
        cout << "\n";
    }
    catch (const exception& e) {
        cout << "[Palindrome] Error: " << e.what() << "\n";
    }

    try {
        cout << "Task 3: list reversing\n";
        cout << "Enter number of list elements: ";
        int n = 0;
        cin >> n;
        cout << "Enter " << n << " elements: ";
        List orig;
        orig.read(cin, n);
        List rev = reverseList(orig);
        cout << "Original list: ";
        orig.print(cout);
        cout << "Reversed list: ";
        rev.print(cout);
        cout << "\n";
    }
    catch (const exception& e) {
        cout << "[List] Error: " << e.what() << "\n";
    }
    return 0;
}