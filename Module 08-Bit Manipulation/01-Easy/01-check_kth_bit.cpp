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
