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
