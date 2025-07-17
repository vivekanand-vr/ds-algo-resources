#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

int reverseNumber(int n) {
    int reversed = 0, original = abs(n);
    while (original > 0) {
        reversed = reversed * 10 + original % 10;
        original /= 10;
    }
    return (n < 0) ? -reversed : reversed;
}

int countDigits(int n) {
    if (n == 0) return 1;
    int count = 0;
    n = abs(n);
    while (n > 0) {
        count++;
        n /= 10;
    }
    return count;
}

/** Check if number reads same forwards/backwards */
bool isPalindrome(int n) {
    return n == reverseNumber(n);
}

/** Check if sum of digits^(digit count) equals number */
bool isArmstrong(int n) {
    int original = n, digits = countDigits(n), sum = 0;
    while (n > 0) {
        int digit = n % 10;
        sum += pow(digit, digits);
        n /= 10;
    }
    return sum == original;
}

/** Check if sum of proper divisors equals number */
bool isPerfect(int n) {
    if (n <= 1) return false;
    int sum = 1;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum += i;
            if (i * i != n) sum += n / i;
        }
    }
    return sum == n;
}

/** Check if repeated sum of squares of digits leads to 1 */
bool isHappy(int n) {
    vector<int> seen;
    while (n != 1 && find(seen.begin(), seen.end(), n) == seen.end()) {
        seen.push_back(n);
        int sum = 0;
        while (n > 0) {
            int digit = n % 10;
            sum += digit * digit;
            n /= 10;
        }
        n = sum;
    }
    return n == 1;
}

int main() {
    vector<int> testNumbers = {153, 6, 28, 7, 121};
    cout << "=== SPECIAL NUMBER CHECKS ===\n";
    for (int num : testNumbers) {
        cout << "\nNumber: " << num << endl;
        cout << "Armstrong: " << (isArmstrong(num) ? "Yes" : "No") << endl;
        cout << "Perfect: " << (isPerfect(num) ? "Yes" : "No") << endl;
        cout << "Happy: " << (isHappy(num) ? "Yes" : "No") << endl;
        cout << "Palindrome: " << (isPalindrome(num) ? "Yes" : "No") << endl;
    }
    return 0;
}