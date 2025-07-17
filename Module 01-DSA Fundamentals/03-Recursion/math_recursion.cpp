#include <iostream>
using namespace std;

int sumFirstN(int n) {
    if(n <= 0) return 0;
    return n + sumFirstN(n - 1);
}

long long factorial(int n) {
    if(n <= 1) return 1;
    return n * factorial(n - 1);
}

long long power(int base, int exp) {
    if(exp == 0) return 1;
    return base * power(base, exp - 1);
}

int sumOfDigits(int n) {
    if(n == 0) return 0;
    return (n % 10) + sumOfDigits(n / 10);
}

int main() {
    cout << "=== MATHEMATICAL RECURSION ===\n";
    cout << "Sum of first 5 numbers: " << sumFirstN(5) << endl;
    cout << "Factorial of 5: " << factorial(5) << endl;
    cout << "2^4: " << power(2, 4) << endl;
    cout << "Sum of digits of 123: " << sumOfDigits(123) << endl;
    return 0;
}