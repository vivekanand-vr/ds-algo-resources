#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Implement pow(x, n), which calculates x raised to the power n (x^n)
//
// Example:
// x = 2.0, n = 10
// 2^10 = 1024.0

/*
    Approach: Binary (Fast) Exponentiation

    - Multiplying x by itself n times is O(n) - too slow when n is
      large (up to ~2 * 10^9 in magnitude here).
    - Instead, write n in binary. x^n can be built from repeated
      squaring: x, x^2, x^4, x^8, ... and n's binary representation
      says exactly which of these squared powers to multiply into the
      answer.
        e.g. n = 13 = 1101 in binary = 8 + 4 + 1
             x^13 = x^8 * x^4 * x^1
    - At each step, check the lowest bit of the exponent: if it is 1,
      fold the current squared value into the running result. Then
      square the base and shift the exponent right by one bit,
      exactly like long multiplication in binary.
    - Negative n means x^n = 1 / x^(-n), so the base is inverted and
      the exponent's sign is flipped up front, then the same loop
      handles the (now positive) magnitude.
    - n is widened to a 64-bit type before negating, since -INT_MIN
      overflows a 32-bit int (INT_MIN has no positive counterpart in
      the same width).

    Algorithm Steps
    ----------------
    1. Widen n into a 64-bit signed integer N
    2. If N < 0, set x = 1 / x and N = -N
    3. Initialize result = 1.0
    4. While N > 0:
         a. If the lowest bit of N is 1, multiply result by x
         b. Square x (x = x * x)
         c. Right-shift N by 1 bit
    5. Return result

    Time Complexity: O(log n) - one loop iteration per bit of n
    Space Complexity: O(1) - only a few variables are used
*/
double myPow(double x, int n) {
  long long N = n;

  if (N < 0) {
    x = 1 / x;
    N = -N;
  }

  double result = 1.0;
  while (N > 0) {
    if (N & 1) result *= x;   // this bit is set - fold in the current power of x
    x *= x;                  // move to the next power of two: x, x^2, x^4, ...
    N >>= 1;
  }

  return result;
}

int main() {
  double x = 2.0;
  int n = 10;
  cout << x << "^" << n << " = " << myPow(x, n) << endl;

  x = 2.0, n = -2;
  cout << x << "^" << n << " = " << myPow(x, n) << endl;

  x = 2.0, n = 0;
  cout << x << "^" << n << " = " << myPow(x, n) << endl;

  return 0;
}
