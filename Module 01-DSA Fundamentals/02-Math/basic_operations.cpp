#include <iostream>
#include <cmath>
using namespace std;

/** Reverse digits of a number */
int reverseNumber(int n) {
    int reversed = 0, original = abs(n);
    while (original > 0) {
        reversed = reversed * 10 + original % 10;
        original /= 10;
    }
    return (n < 0) ? -reversed : reversed;
}

/** Count total digits */
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

/** Sum all digits */
int sumOfDigits(int n) {
    int sum = 0;
    n = abs(n);
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

/** Multiply all digits */
int productOfDigits(int n) {
    int product = 1;
    n = abs(n);
    if (n == 0) return 0;
    while (n > 0) {
        product *= n % 10;
        n /= 10;
    }
    return product;
}

int main() {
    int num = 12345;
    cout << "=== BASIC OPERATIONS ===\n";
    cout << "Number: " << num << endl;
    cout << "Reversed: " << reverseNumber(num) << endl;
    cout << "Digit Count: " << countDigits(num) << endl;
    cout << "Sum of Digits: " << sumOfDigits(num) << endl;
    cout << "Product of Digits: " << productOfDigits(num) << endl;
    return 0;
}
