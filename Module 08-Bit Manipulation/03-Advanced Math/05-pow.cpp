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

/*
    ======================================================================
    DRY RUN: x = 2.0, n = 10   (answer = 1024.0)
    ======================================================================

      the exponent being decomposed:
        n = 10 = 1010 in binary
        bit position : bit3 bit2 bit1 bit0
        bit value    :  1    0    1    0
        weight       :  8    4    2    1
        so 10 = 8 + 2   and   2^10 = 2^8 * 2^2 = 256 * 4 = 1024

      the loop consumes those bits from the RIGHT (bit0 first).

    Tracked state:
      N      - the remaining exponent; shifted right one bit per step,
               so its lowest bit is the bit currently being examined
      x      - the current repeated-squaring power: x^1, x^2, x^4, x^8...
      result - the product of exactly those powers whose bit was set

    Setup:
      N = 10 (widened to long long), N < 0 is false -> no reciprocal,
      no sign flip. result = 1.0, x = 2.0

    Initial state: N = 1010 (10), x = 2.0, result = 1.0

    ----------------------------------------------------------------------
    step 1     N = 1010 (10),  x = 2.0 = x^1
      bit set?   N & 1 = 1010 & 0001 = 0000 -> bit0 is 0, DO NOT multiply
      result     stays 1.0
      square     x = 2.0 * 2.0 = 4.0            (now x^2)
      halve      N = 1010 >> 1 = 101 (5)

    ----------------------------------------------------------------------
    step 2     N = 101 (5),  x = 4.0 = x^2
      bit set?   N & 1 = 101 & 001 = 001 -> bit is 1, MULTIPLY
      result     1.0 * 4.0 = 4.0                (x^2 folded in)
      square     x = 4.0 * 4.0 = 16.0           (now x^4)
      halve      N = 101 >> 1 = 10 (2)

    ----------------------------------------------------------------------
    step 3     N = 10 (2),  x = 16.0 = x^4
      bit set?   N & 1 = 10 & 01 = 0 -> bit is 0, DO NOT multiply
      result     stays 4.0
      square     x = 16.0 * 16.0 = 256.0        (now x^8)
      halve      N = 10 >> 1 = 1

    ----------------------------------------------------------------------
    step 4     N = 1 (1),  x = 256.0 = x^8
      bit set?   N & 1 = 1 -> MULTIPLY
      result     4.0 * 256.0 = 1024.0           (x^8 folded in)
      square     x = 256.0 * 256.0 = 65536.0    (x^16 - computed but
                                                 never used; the code
                                                 squares unconditionally
                                                 even on the last pass)
      halve      N = 1 >> 1 = 0  ->  while (N > 0) fails, loop ends

    ----------------------------------------------------------------------
    RETURN result = 1024.0

    ======================================================================
    Summary table
    ======================================================================

    x = 2.0, n = 10 (binary 1010):

    | step | N before | N binary | bit set? | x in  | result after | N after |
    |------|----------|----------|----------|-------|--------------|---------|
    |  1   |    10    |   1010   |    no    |   2.0 |      1.0     |    5    |
    |  2   |     5    |    101   |   yes    |   4.0 |      4.0     |    2    |
    |  3   |     2    |     10   |    no    |  16.0 |      4.0     |    1    |
    |  4   |     1    |      1   |   yes    | 256.0 |   1024.0     |    0    |

      the two "yes" rows contributed x^2 and x^8, exactly the two set
      bits of 1010, and 2 + 8 = 10.

    The negative case from main(), x = 2.0, n = -2:
      setup   N = -2 < 0 -> x = 1 / 2.0 = 0.5, N = 2 (binary 10)

    | step | N before | N binary | bit set? | x in   | result after | N after |
    |------|----------|----------|----------|--------|--------------|---------|
    |  1   |     2    |     10   |    no    |  0.5   |     1.0      |    1    |
    |  2   |     1    |      1   |   yes    | 0.25   |     0.25     |    0    |

      RETURN 0.25 = 1/4 = 2^-2   (check)

    The n = 0 case: N = 0, the while body never runs, result stays 1.0.

    The identity that makes it work:
      write n in binary as a sum of distinct powers of two,
      n = 2^a1 + 2^a2 + ... ; then
          x^n = x^(2^a1) * x^(2^a2) * ...
      and the repeated squaring x -> x^2 -> x^4 -> x^8 walks through
      every x^(2^k) in order, so one pass over the bits of n suffices.

    Step count backing O(log n):
      n = 10 needed 4 iterations - the number of bits in 1010 - with one
      squaring and at most one extra multiply each, so 7 multiplications
      total instead of the 10 a naive loop would use. The gap is the
      whole point at scale: n = 1,000,000,000 takes 30 iterations
      (about 59 multiplications) rather than a billion.
*/
