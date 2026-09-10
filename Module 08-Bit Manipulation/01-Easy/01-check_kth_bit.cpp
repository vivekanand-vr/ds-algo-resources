#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Check whether the k-th bit (0-indexed from the LSB) of a given number n is set (1) or not (0)
//
// Example:
// n = 5 (binary 101), k = 0
// Bit 0 of n is 1 -> set

/*
    Approach: Bit Masking

    - Build a mask that has only the k-th bit turned on: mask = 1 << k
    - AND n with this mask. Every other bit of n is wiped out by the 0s
      in the mask, so the result is non-zero if and only if the k-th
      bit of n was set.

    Algorithm Steps
    ----------------
    1. Left shift 1 by k to isolate the k-th bit position
    2. Bitwise AND the mask with n
    3. Return true if the result is non-zero, false otherwise

    Time Complexity: O(1) - a single shift and a single AND
    Space Complexity: O(1) - no extra memory used
*/
bool checkKthBit(int n, int k) {
  return (n & (1 << k)) != 0;
}

int main() {
  int n = 5, k = 0;

  cout << "n = " << n << " (binary: " << bitset<8>(n) << "), k = " << k << endl;
  cout << "Is bit " << k << " set? " << (checkKthBit(n, k) ? "Yes" : "No") << endl;

  k = 1;
  cout << "Is bit " << k << " set? " << (checkKthBit(n, k) ? "Yes" : "No") << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: n = 5, k = 0 (answer = true)  then k = 1 (answer = false)
    ======================================================================

      n = 5     binary:  1 0 1
                bit:     2 1 0

    Tracked state:
      mask     - a value with a single 1 sitting at position k, built as
                 1 << k
      n & mask - every bit of n except bit k is ANDed against a 0 and so
                 is wiped out; what survives is bit k of n, still in its
                 original column

    There is no loop here - the function is one expression, so each call
    is a single evaluation.

    ----------------------------------------------------------------------
    Call 1: checkKthBit(5, 0)          <-- the TRUE case
      shift      1 << 0    =   0 0 1     (1)
      operands   n         =   1 0 1     (5)
                 mask      =   0 0 1     (1)
                 n & mask  =   0 0 1     (1)
      test       1 != 0 -> true
      RETURN true            main prints "Is bit 0 set? Yes"

    ----------------------------------------------------------------------
    Call 2: checkKthBit(5, 1)          <-- the FALSE case
      shift      1 << 1    =   0 1 0     (2)
      operands   n         =   1 0 1     (5)
                 mask      =   0 1 0     (2)
                 n & mask  =   0 0 0     (0)
      test       0 != 0 -> false
      RETURN false           main prints "Is bit 1 set? No"

    ======================================================================
    Summary table
    ======================================================================

    | k | mask = 1 << k | n binary | n & mask | decimal | result |
    |---|---------------|----------|----------|---------|--------|
    | 0 |     001       |   101    |   001    |    1    | true   |
    | 1 |     010       |   101    |   000    |    0    | false  |
    | 2 |     100       |   101    |   100    |    4    | true   |

    (the k = 2 row is not called by main, it is there so all three bits
     of 101 can be read straight off the table)

    Why the identity holds:
      AND is bitwise: result bit i = n bit i AND mask bit i. mask has a 1
      only at position i = k, and x AND 0 = 0, x AND 1 = x. So the result
      is 0 in every column except column k, where it is exactly n's bit.
      That means the whole value is non-zero if and only if bit k was 1.
      Note the survivor keeps its place value (2^k, here 4 for k = 2),
      which is why the code compares against 0 rather than against 1.

    Step count backing O(1):
      one shift, one AND, one comparison per call - 3 operations, and the
      count does not grow with n or k.
*/
