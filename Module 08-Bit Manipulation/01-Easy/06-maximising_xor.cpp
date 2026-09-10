#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given two integers l and r, find the maximum value of (a xor b)
//    over all pairs a, b such that l <= a <= b <= r
//
// Example:
// l = 10, r = 15
// Checking pairs, the best is 8 (1000) xor 15 (1111) = 7 (0111)
// Maximum value = 7

/*
    Approach: Highest Differing Bit

    - a xor b is maximised by making as many high-order bits differ as
      possible, and every bit above the highest bit where l and r differ
      is forced to be identical in any a, b within [l, r] - only the bits
      from that point downward are free to vary.
    - So compute diff = l xor r. Its highest set bit marks the first
      position where the range allows a and b to disagree.
    - Once that position is known, the best possible xor simply sets
      that bit and every bit below it to 1, since we can always find an
      a, b pair in [l, r] achieving exactly that pattern.
    - This means the answer is (1 << (highestBitPosition + 1)) - 1,
      i.e. all 1s up to and including that bit.
    - The highest set bit is found by hand: keep left-shifting a mask
      until shifting it once more would pass diff, instead of relying
      on a compiler bit-scan builtin.

    Algorithm Steps
    ----------------
    1. Compute diff = l xor r
    2. If diff == 0, l == r, so the only possible xor is 0
    3. Starting from mask = 1, keep doubling mask while (mask << 1) <= diff
       to walk mask up to the highest set bit of diff
    4. Return (mask << 1) - 1, an all-ones value up to that bit

    Time Complexity: O(log(diff)) - one shift per bit position of diff
    Space Complexity: O(1) - no extra memory used
*/
int maximizingXor(int l, int r) {
  int diff = l ^ r;
  if (diff == 0) return 0;

  int mask = 1;
  while ((mask << 1) <= diff) {
    mask <<= 1;   // walk mask up to the highest set bit of diff
  }

  return (mask << 1) - 1;
}

int main() {
  int l = 10, r = 15;
  cout << "l = " << l << ", r = " << r << endl;
  cout << "Maximum XOR value: " << maximizingXor(l, r) << endl;

  l = 8, r = 20;
  cout << "l = " << l << ", r = " << r << endl;
  cout << "Maximum XOR value: " << maximizingXor(l, r) << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: l = 10, r = 15   (answer = 7)
    ======================================================================

      l = 10    binary:  1 0 1 0
      r = 15    binary:  1 1 1 1
                bit:     3 2 1 0

      diff = l ^ r    =  0 1 0 1    (5)
                           ^
                           highest set bit of diff is bit 2

      bit 3 is 1 in both l and r, so EVERY value in [10, 15] has bit 3
      set and no pair can ever make bit 3 differ - the free columns start
      at bit 2

    Tracked state:
      diff - l ^ r, a 1 in each column where l and r disagree
      mask - a single-bit value walked upward until it lands on the
             highest set bit of diff; the loop guard is (mask << 1),
             i.e. "would the NEXT position still be inside diff?"

    Initial state: diff = 0 1 0 1 (5), mask = 0 0 0 1 (1)

    ----------------------------------------------------------------------
    Zero check:  diff == 0 ?   no, diff = 5 -> continue

    ----------------------------------------------------------------------
    Iteration 1      guard: (mask << 1) <= diff
      mask       =  0 0 0 1    (1)
      mask << 1  =  0 0 1 0    (2)
      test       2 <= 5 -> true, enter
      mask <<= 1 =  0 0 1 0    (2)         mask moved bit 0 -> bit 1

    ----------------------------------------------------------------------
    Iteration 2      guard: (mask << 1) <= diff
      mask       =  0 0 1 0    (2)
      mask << 1  =  0 1 0 0    (4)
      test       4 <= 5 -> true, enter
      mask <<= 1 =  0 1 0 0    (4)         mask moved bit 1 -> bit 2

    ----------------------------------------------------------------------
    Iteration 3 does not happen
      mask       =  0 1 0 0    (4)
      mask << 1  =  1 0 0 0    (8)
      test       8 <= 5 -> false, exit
                 mask has stopped exactly on bit 2, the highest set bit
                 of diff - one position further would overshoot diff

    ----------------------------------------------------------------------
    Return value
      mask       =  0 1 0 0    (4)
      mask << 1  =  1 0 0 0    (8)
      minus 1    =  0 1 1 1    (7)         all 1s from bit 2 downward

    RETURN 7        main prints "Maximum XOR value: 7"
                    achieved by a = 11 (1011), b = 12 (1100), both in
                    [10, 15]:  1011 ^ 1100 = 0111 = 7

    ----------------------------------------------------------------------
    Second call from main: l = 8, r = 20

      l = 8     binary:  0 1 0 0 0
      r = 20    binary:  1 0 1 0 0
                bit:     4 3 2 1 0

      diff = l ^ r    =  1 1 1 0 0    (28)     highest set bit is bit 4

      | iter | mask before | binary | mask << 1 | binary | <= 28? |
      |------|-------------|--------|-----------|--------|--------|
      |  1   |      1      | 00001  |     2     | 00010  | yes    |
      |  2   |      2      | 00010  |     4     | 00100  | yes    |
      |  3   |      4      | 00100  |     8     | 01000  | yes    |
      |  4   |      8      | 01000  |    16     | 10000  | yes    |
      | exit |     16      | 10000  |    32     |100000  | no     |

      return   (16 << 1) - 1 = 32 - 1 = 31 = 1 1 1 1 1
      RETURN 31       achieved by a = 11 (01011), b = 20 (10100):
                      01011 ^ 10100 = 11111 = 31, both in [8, 20]

      Note the low bits: diff = 11100 has 0s in bits 1 and 0, yet the
      answer still sets them. Once bit 4 is free to differ, the range is
      wide enough that the lower columns can be chosen freely too - which
      is exactly why the code returns an all-ones value rather than diff
      itself. (Returning diff here would give 28, and 31 > 28.)

    ======================================================================
    Summary table (first call, l = 10, r = 15)
    ======================================================================

    | iter | mask before | binary | mask << 1 | binary | <= 5? | action |
    |------|-------------|--------|-----------|--------|-------|--------|
    |  1   |      1      |  0001  |     2     |  0010  | yes   | shift  |
    |  2   |      2      |  0010  |     4     |  0100  | yes   | shift  |
    | exit |      4      |  0100  |     8     |  1000  | no    | stop   |

    | result | binary | decimal |
    |--------|--------|---------|
    | mask   |  0100  |    4    |
    | mask<<1|  1000  |    8    |
    | answer |  0111  |    7    |

    The bit-level identity that makes the trick work:
      for any single bit position p,  (1 << (p+1)) - 1  is all 1s in
      columns p..0, by the same borrow chain that powers n & (n-1):

          1 << 3  =  1 0 0 0     bit 0 is 0 -> borrow
                                 bit 1 is 0 -> borrow
                                 bit 2 is 0 -> borrow
                                 bit 3 is 1 -> pays, becomes 0
          minus 1 =  0 1 1 1     every borrowed column is now 1

      So the code never has to build the answer bit by bit; finding the
      position p of the highest differing bit is enough, and one
      subtraction fills in everything below it.

    Step count backing O(log diff):
      diff = 5 needed 2 shifts plus 3 guard tests (bit 2 is position 2).
      diff = 28 needed 4 shifts plus 5 guard tests (bit 4). The work is
      the bit LENGTH of diff, not its magnitude - and certainly not the
      O((r-l)^2) pair enumeration the problem statement hints at, which
      for l = 8, r = 20 would test 91 pairs instead of 5.
*/
