#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

/** Greatest Common Divisor using Euclidean algorithm */
int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/** Least Common Multiple */
int lcm(int a, int b) {
    return abs(a * b) / gcd(a, b);
}

/** GCD of multiple numbers */
int gcdMultiple(vector<int> numbers) {
    if (numbers.empty()) return 0;
    int result = numbers[0];
    for (int i = 1; i < numbers.size(); i++) {
        result = gcd(result, numbers[i]);
    }
    return result;
}

/** LCM of multiple numbers */
int lcmMultiple(vector<int> numbers) {
    if (numbers.empty()) return 0;
    int result = numbers[0];
    for (int i = 1; i < numbers.size(); i++) {
        result = lcm(result, numbers[i]);
    }
    return result;
}

int main() {
    cout << "=== GCD AND LCM OPERATIONS ===\n";
    int a = 48, b = 18;
    cout << "GCD of " << a << " and " << b << ": " << gcd(b, a) << endl;
    cout << "LCM of " << a << " and " << b << ": " << lcm(a, b) << endl;
    vector<int> numbers = {12, 18, 24};
    cout << "GCD of {12, 18, 24}: " << gcdMultiple(numbers) << endl;
    cout << "LCM of {12, 18, 24}: " << lcmMultiple(numbers) << endl;
    return 0;
}