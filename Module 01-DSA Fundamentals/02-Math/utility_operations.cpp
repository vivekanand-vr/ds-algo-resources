#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

/** Fast exponentiation algorithm */
long long power(int base, int exp) {
    long long result = 1, b = base;
    while (exp > 0) {
        if (exp % 2 == 1) result *= b;
        b *= b;
        exp /= 2;
    }
    return result;
}

/** Check if a number is perfect square */
bool isPerfectSquare(int n) {
    if (n < 0) return false;
    int root = sqrt(n);
    return root * root == n;
}

/** Check if a number is perfect cube */
bool isPerfectCube(int n) {
    if (n < 0) return false;
    int root = round(cbrt(n));
    return root * root * root == n;
}

/** Convert to binary string */
string toBinary(int n) {
    if (n == 0) return "0";
    string binary = "";
    while (n > 0) {
        binary = (n % 2 == 0 ? "0" : "1") + binary;
        n /= 2;
    }
    return binary;
}

/** Count 1s in binary representation */
int countSetBits(int n) {
    int count = 0;
    while (n) {
        count++;
        n &= (n - 1);
    }
    return count;
}

/** Find all divisors of a number */
vector<int> getDivisors(int n) {
    vector<int> divisors;
    for (int i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i * i != n) divisors.push_back(n / i);
        }
    }
    sort(divisors.begin(), divisors.end());
    return divisors;
}

int main() {
    int num = 12;
    cout << "=== UTILITY OPERATIONS ===\n";
    cout << "Number: " << num << endl;
    cout << "Binary: " << toBinary(num) << endl;
    cout << "Set Bits: " << countSetBits(num) << endl;
    cout << "Perfect Square: " << (isPerfectSquare(num) ? "Yes" : "No") << endl;
    cout << "Perfect Cube: " << (isPerfectCube(num) ? "Yes" : "No") << endl;
    cout << "Power 2^10: " << power(2, 10) << endl;
    cout << "Divisors of " << num << ": ";
    for (int d : getDivisors(num)) cout << d << " ";
    cout << endl;
    return 0;
}