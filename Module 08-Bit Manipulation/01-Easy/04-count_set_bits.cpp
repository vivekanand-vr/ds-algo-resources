#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Count the number of set bits (1s) in the binary representation of n
//
// Example:
// n = 11 (binary 1011)
// Set bits: 3

/*
    Approach: Brian Kernighan's Algorithm

    - Checking every one of the 32 bit positions of n works, but wastes
      time on every 0 bit.
    - Kernighan's trick: n & (n - 1) always clears exactly the lowest
      set bit of n and leaves every other bit untouched.
        e.g. n = 1011 0 (=1100), well:
             n      = 1010 1100
             n - 1   = 1010 1011
             n & n-1 = 1010 1000   <- lowest set bit (the '100') is gone
    - So repeatedly applying n = n & (n - 1) removes one set bit per
      step. Counting how many steps it takes to reach 0 is exactly the
      number of set bits n started with.
    - This visits each SET bit exactly once, instead of every bit
      position, so it is faster whenever n is sparse (few 1s).

    Algorithm Steps
    ----------------
    1. Initialize count = 0
    2. While n is not 0:
         a. n = n & (n - 1)     -> clears the lowest set bit
         b. count++
    3. Return count

    Time Complexity: O(k) - k is the number of set bits in n (worst case O(log n))
    Space Complexity: O(1) - only a counter is used
*/
int countSetBits(int n) {
  int count = 0;
  while (n != 0) {
    n = n & (n - 1);   // clear the lowest set bit
    count++;
  }
  return count;
}

int main() {
  int n = 11;
  cout << "n = " << n << " (binary: " << bitset<8>(n) << ")" << endl;
  cout << "Number of set bits: " << countSetBits(n) << endl;

  n = 16;
  cout << "n = " << n << " (binary: " << bitset<8>(n) << ")" << endl;
  cout << "Number of set bits: " << countSetBits(n) << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: n = 11   (answer = 3)
    ======================================================================

      n = 11    binary:  0 1 0 1 1
                bit:     4 3 2 1 0

      the three set bits are bit 3 (8), bit 1 (2) and bit 0 (1), so the
      loop must run exactly 3 times

    Tracked state:
      n     - the remaining number; one set bit is stripped off it per
              iteration, always the LOWEST one still standing
      count - how many set bits have been stripped so far

    Initial state: n = 0 1 0 1 1 (11), count = 0

    ----------------------------------------------------------------------
    Iteration 1      loop test: n = 11 != 0 -> enter
      n         =  0 1 0 1 1    (11)
      n - 1     =  0 1 0 1 0    (10)      bit 0 paid the borrow directly
      n & n-1   =  0 1 0 1 0    (10)
      cleared   bit 0            count 0 -> 1
      state     n = 10, count = 1

    ----------------------------------------------------------------------
    Iteration 2      loop test: n = 10 != 0 -> enter
      n         =  0 1 0 1 0    (10)
      n - 1     =  0 1 0 0 1    ( 9)      bit 1 paid, bit 0 borrowed a 1
      n & n-1   =  0 1 0 0 0    ( 8)
      cleared   bit 1            count 1 -> 2
      state     n = 8, count = 2
                note the borrow created a 1 in column 0 of n - 1, but n
                has a 0 there, so the AND kills it - nothing is ever
                added back

    ----------------------------------------------------------------------
    Iteration 3      loop test: n = 8 != 0 -> enter
      n         =  0 1 0 0 0    ( 8)
      n - 1     =  0 0 1 1 1    ( 7)      bit 3 paid, bits 2,1,0 borrowed
      n & n-1   =  0 0 0 0 0    ( 0)
      cleared   bit 3            count 2 -> 3
      state     n = 0, count = 3

    ----------------------------------------------------------------------
    Iteration 4 does not happen: loop test n = 0 != 0 is false -> exit

    RETURN count = 3

    ----------------------------------------------------------------------
    Second call from main: n = 16
      n = 16    binary:  1 0 0 0 0
                bit:     4 3 2 1 0

      Iteration 1   n       =  1 0 0 0 0   (16)
                    n - 1   =  0 1 1 1 1   (15)
                    n & n-1 =  0 0 0 0 0   ( 0)   count 0 -> 1
      loop test     n = 0 -> exit
      RETURN 1      one set bit, one iteration - a power of two is the
                    cheapest possible input for this algorithm

    ======================================================================
    Summary table
    ======================================================================

    | iter | n before | binary | n - 1 binary | n after | binary | count |
    |------|----------|--------|--------------|---------|--------|-------|
    |  1   |    11    | 01011  |    01010     |   10    | 01010  |   1   |
    |  2   |    10    | 01010  |    01001     |    8    | 01000  |   2   |
    |  3   |     8    | 01000  |    00111     |    0    | 00000  |   3   |
    | exit |     0    | 00000  |      -       |    -    |   -    |   3   |

    Reading the binary column downward, the 1s disappear from the right:
        0 1 0 1 1  ->  0 1 0 1 0  ->  0 1 0 0 0  ->  0 0 0 0 0
    one 1 per row, never two, and no 1 ever reappears.

    Why n & (n-1) clears exactly the lowest set bit - the borrow chain:
      subtracting 1 borrows from bit 0 upward until it reaches a column
      holding a 1. That column becomes 0 and every column it borrowed
      through becomes 1. Iteration 3 is the clearest example:

          n     =  0 1 0 0 0     bit 0 is 0 -> borrow
                                 bit 1 is 0 -> borrow
                                 bit 2 is 0 -> borrow
                                 bit 3 is 1 -> pays, becomes 0
          n - 1 =  0 0 1 1 1     the three borrowed columns are now 1

      Above the lowest set bit the two numbers are identical, so those
      bits survive the AND untouched. At the lowest set bit and below,
      the two numbers disagree in every column (1 vs 0, then 0 vs 1), so
      the AND turns all of them into 0. Net effect: exactly one 1 is
      removed per step, and no other bit moves.

    Step count backing O(k):
      n = 11 has 3 set bits and took 3 iterations (plus the failing loop
      test) - not the 4 or 8 or 32 bit positions a per-position scan of
      an int would inspect. n = 16 took 1 iteration despite being the
      larger number: the cost tracks the number of 1s, not the size of n.
*/
