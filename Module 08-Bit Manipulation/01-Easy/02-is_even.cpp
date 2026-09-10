#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Check whether a given integer n is even or odd
//
// Example:
// n = 4 -> Even
// n = 7 -> Odd

/*
    Approach: Bitwise AND with 1

    - The least significant bit (LSB) of any even number is always 0,
      and the LSB of any odd number is always 1.
    - n & 1 isolates just that bit, so it is 0 exactly when n is even.
    - This avoids the % operator, which internally is more expensive
      than a single bitwise AND.

    Algorithm Steps
    ----------------
    1. Compute n & 1
    2. If the result is 0, n is even; otherwise n is odd

    Time Complexity: O(1) - a single bitwise operation
    Space Complexity: O(1) - no extra memory used
*/
bool isEven(int n) {
  return (n & 1) == 0;
}

int main() {
  int n = 4;
  cout << n << " is " << (isEven(n) ? "Even" : "Odd") << endl;

  n = 7;
  cout << n << " is " << (isEven(n) ? "Even" : "Odd") << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: n = 4 (answer = Even)  then n = 7 (answer = Odd)
    ======================================================================

      n = 4     binary:  1 0 0        n = 7     binary:  1 1 1
                bit:     2 1 0                  bit:     2 1 0

    Tracked state:
      n & 1 - the mask 1 is 0 0 1, so this keeps bit 0 of n and clears
              everything above it; the answer is that single bit

    There is no loop here - the function is one expression, so each call
    is a single evaluation.

    ----------------------------------------------------------------------
    Call 1: isEven(4)                  <-- the EVEN case
      operands   n         =   1 0 0     (4)
                 mask 1    =   0 0 1     (1)
                 n & 1     =   0 0 0     (0)
      test       0 == 0 -> true
      RETURN true            main prints "4 is Even"

    ----------------------------------------------------------------------
    Call 2: isEven(7)                  <-- the ODD case
      operands   n         =   1 1 1     (7)
                 mask 1    =   0 0 1     (1)
                 n & 1     =   0 0 1     (1)
      test       1 == 0 -> false
      RETURN false           main prints "7 is Odd"

    ======================================================================
    Summary table
    ======================================================================

    | n | n binary | n & 1 | decimal | isEven |
    |---|----------|-------|---------|--------|
    | 4 |   100    |  000  |    0    | true   |
    | 7 |   111    |  001  |    1    | false  |
    | 0 |   000    |  000  |    0    | true   |
    | 1 |   001    |  001  |    1    | false  |

    (the 0 and 1 rows are not called by main; they show the two smallest
     cases behave correctly with no special handling)

    Why the identity holds:
      every binary number is a sum of powers of two, and every power of
      two except 2^0 is itself even:
          1 0 0  =  4
          1 1 1  =  4 + 2 + 1
      bits 1 and above contribute 2, 4, 8, ... which are all divisible by
      2, so the parity of the whole number is decided by bit 0 alone.
      n & 1 reads exactly that bit, which is why it agrees with n % 2 for
      every non-negative n. (For negative n the two differ in C++ only in
      sign, never in parity: -7 & 1 = 1 and -7 % 2 = -1, both odd.)

    Step count backing O(1):
      one AND and one comparison per call - 2 operations, independent of
      how large n is.
*/
