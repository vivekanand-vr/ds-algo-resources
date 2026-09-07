#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Divide two integers without using multiplication, division, or the
//    mod operator. Return the quotient after dividing dividend by divisor,
//    truncated toward zero.
//
// Example:
// dividend = 43, divisor = 5
// 43 / 5 = 8.6 -> truncated to 8

/*
    Approach: Exponential Search via Bit Shifting

    - Repeated subtraction of the divisor is correct but far too slow
      (O(dividend / divisor) steps).
    - Instead, at each step we find the largest value (divisor << k) that
      still fits inside what remains of the dividend. Left-shifting by k
      is the same as multiplying by 2^k, so we are effectively trying the
      biggest power-of-two multiple of the divisor that fits.
    - Subtract that value out, add 2^k to the running quotient, and
      repeat on what remains.
    - Work with the absolute values in a wider type (long long) and track
      the sign separately, since negating INT_MIN overflows a 32-bit int.
    - LeetCode's 32-bit constraint means INT_MIN / -1 overflows the valid
      range, so that one case is clamped to INT_MAX explicitly.

    Algorithm Steps
    ----------------
    1. Handle the overflow edge case: dividend == INT_MIN && divisor == -1
    2. Determine the sign of the result from the signs of the inputs
    3. Work with abs(dividend) and abs(divisor) as long long
    4. While the remaining dividend >= divisor:
         a. Double the divisor (via left shift) as many times as it still fits
         b. Subtract that doubled value from the remaining dividend
         c. Add the corresponding power of two to the quotient
    5. Apply the sign and return the quotient

    Time Complexity: O(log^2 n) - the inner doubling loop is bounded by
                      log(quotient) and runs across O(log(quotient)) outer steps
    Space Complexity: O(1) - only a few accumulator variables are used
*/
int divide(int dividend, int divisor) {
  if (dividend == INT_MIN && divisor == -1) return INT_MAX;  // overflow guard

  bool negative = (dividend < 0) ^ (divisor < 0);

  long long a = llabs((long long)dividend);
  long long b = llabs((long long)divisor);
  long long quotient = 0;

  while (a >= b) {
    long long temp = b, multiple = 1;
    while (a >= (temp << 1)) {
      temp <<= 1;
      multiple <<= 1;
    }
    a -= temp;
    quotient += multiple;
  }

  return negative ? (int)(-quotient) : (int)quotient;
}

int main() {
  int dividend = 43, divisor = 5;
  cout << dividend << " / " << divisor << " = " << divide(dividend, divisor)
       << endl;

  dividend = -43, divisor = 5;
  cout << dividend << " / " << divisor << " = " << divide(dividend, divisor)
       << endl;

  dividend = INT_MIN, divisor = -1;
  cout << dividend << " / " << divisor << " = " << divide(dividend, divisor)
       << endl;

  return 0;
}