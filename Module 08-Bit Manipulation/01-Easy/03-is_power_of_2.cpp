#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Check if a given positive integer n is a power of 2
//
// Example:
// n = 16 (binary 10000) -> true
// n = 18 (binary 10010) -> false

/*
    Approach: n & (n - 1) Trick

    - A power of two has exactly one set bit (e.g. 8 = 1000).
    - Subtracting 1 flips every bit up to and including that lowest
      set bit (1000 - 1 = 0111).
    - ANDing the two together clears that single set bit, producing 0
      exactly when n had only one bit set, i.e. when n is a power of 2.
    - n must be strictly positive: the check is meaningless for 0 or
      negative numbers (0 has no set bits at all).

    Algorithm Steps
    ----------------
    1. If n <= 0, return false
    2. Compute n & (n - 1)
    3. Return true if the result is 0, false otherwise

    Time Complexity: O(1) - a subtraction and a bitwise AND
    Space Complexity: O(1) - no extra memory used
*/
bool isPowerOfTwo(int n) {
  return n > 0 && (n & (n - 1)) == 0;
}

int main() {
  int n = 16;
  cout << n << " is power of 2? " << (isPowerOfTwo(n) ? "Yes" : "No") << endl;

  n = 18;
  cout << n << " is power of 2? " << (isPowerOfTwo(n) ? "Yes" : "No") << endl;

  n = 1;
  cout << n << " is power of 2? " << (isPowerOfTwo(n) ? "Yes" : "No") << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: n = 16 (answer = Yes), n = 18 (No), n = 1 (Yes)
    ======================================================================

      n = 16    binary:  1 0 0 0 0        n = 18   binary:  1 0 0 1 0
                bit:     4 3 2 1 0                 bit:     4 3 2 1 0

    Tracked state:
      n - 1     - flips the lowest set bit of n to 0 and turns every 0
                  below it into a 1 (a borrow chain)
      n & (n-1) - therefore equals n with its lowest set bit cleared; it
                  is 0 only when that bit was the ONLY set bit

    There is no loop here - the function is one short-circuit expression,
    so each call is a single evaluation.

    ----------------------------------------------------------------------
    Call 1: isPowerOfTwo(16)           <-- the TRUE case
      guard      16 > 0 -> true, so the AND is evaluated
      operands   n       =  1 0 0 0 0    (16)
                 n - 1   =  0 1 1 1 1    (15)
                 n & n-1 =  0 0 0 0 0    (0)
      test       0 == 0 -> true
      RETURN true            main prints "16 is power of 2? Yes"
                 nothing overlaps: the one bit of 16 and the four bits of
                 15 sit in completely different columns

    ----------------------------------------------------------------------
    Call 2: isPowerOfTwo(18)           <-- the FALSE case
      guard      18 > 0 -> true
      operands   n       =  1 0 0 1 0    (18)
                 n - 1   =  1 0 0 0 1    (17)
                 n & n-1 =  1 0 0 0 0    (16)
      test       16 == 0 -> false
      RETURN false           main prints "18 is power of 2? No"
                 bit 1 (the lowest set bit) was cleared as always, but
                 bit 4 SURVIVED - that leftover 1 is the proof that 18
                 had more than one set bit

    ----------------------------------------------------------------------
    Call 3: isPowerOfTwo(1)            <-- smallest true case, 2^0
      guard      1 > 0 -> true
      operands   n       =  0 0 0 0 1    (1)
                 n - 1   =  0 0 0 0 0    (0)
                 n & n-1 =  0 0 0 0 0    (0)
      test       0 == 0 -> true
      RETURN true            main prints "1 is power of 2? Yes"

    ======================================================================
    Summary table
    ======================================================================

    |  n | n binary | n-1 binary | n & (n-1) | decimal | power of 2? |
    |----|----------|------------|-----------|---------|-------------|
    | 16 |  10000   |   01111    |   00000   |    0    | yes         |
    | 18 |  10010   |   10001    |   10000   |   16    | no          |
    |  1 |  00001   |   00000    |   00000   |    0    | yes         |
    | 12 |  01100   |   01011    |   01000   |    8    | no          |
    |  0 |  00000   |     -      |     -     |    -    | no (guard)  |

    (the 12 row is not called by main; the 0 row never reaches the AND at
     all because n > 0 short-circuits first - which also means n - 1 is
     never computed on 0, avoiding the all-ones value that would give)

    Why n & (n-1) clears exactly the lowest set bit - the borrow chain:
      subtracting 1 borrows from column 0 upward until it finds a 1 to
      take. Watch 12 do it, bit by bit:

          12 =  0 1 1 0 0
                        ^ bit 0 is 0, cannot give up a 1 -> borrow
                      ^   bit 1 is 0, cannot either      -> borrow
                    ^     bit 2 is 1  -> it pays: becomes 0, and every
                                         borrowed column below becomes 1
          11 =  0 1 0 1 1

      So n and n - 1 agree on every bit ABOVE the lowest set bit, and
      disagree on the lowest set bit and everything under it:

          n     =  0 1 1 0 0
          n - 1 =  0 1 0 1 1
                   = =           agree above  -> survive the AND
                       x x x     disagree     -> all become 0
          AND   =  0 1 0 0 0     (8, the untouched high bits of 12)

      For 16 there are no bits above the lowest set bit, so nothing can
      survive and the AND is 0. That is the whole test: a power of two is
      precisely a number with nothing left over after this clearing.

    Step count backing O(1):
      one comparison, one subtraction, one AND, one comparison per call -
      4 operations regardless of how big n is, versus the O(log n) loop a
      "divide by 2 repeatedly" version would need.
*/
