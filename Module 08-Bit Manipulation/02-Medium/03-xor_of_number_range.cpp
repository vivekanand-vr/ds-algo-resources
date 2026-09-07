#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given two integers l and r, find the XOR of every integer in the
//    inclusive range [l, r]
//
// Example:
// l = 2, r = 5
// 2 xor 3 xor 4 xor 5 = 0

/*
    Approach: Prefix XOR with a Period-4 Pattern

    - XORing the whole range one element at a time is O(r - l), too
      slow when the range is large.
    - Just like prefix sums let range sums be computed as a subtraction,
      prefix XORs let range XORs be computed the same way, because XOR
      is its own inverse: x ^ x = 0.
          xor(l..r) = prefixXor(r) ^ prefixXor(l - 1)
      (XORing the two prefixes cancels everything from 0 to l - 1,
      leaving only l..r.)
    - prefixXor(n) = 0 ^ 1 ^ 2 ^ ... ^ n follows a simple repeating
      pattern with period 4, which can be verified by tracing the
      running XOR for n = 0, 1, 2, 3, 4, 5, ...:
          n % 4 == 0  ->  prefixXor(n) = n
          n % 4 == 1  ->  prefixXor(n) = 1
          n % 4 == 2  ->  prefixXor(n) = n + 1
          n % 4 == 3  ->  prefixXor(n) = 0
      so no loop is required at all.

    Algorithm Steps
    ----------------
    1. Write a helper prefixXor(n) that returns 0^1^...^n in O(1)
       using the period-4 pattern above
    2. Return prefixXor(r) ^ prefixXor(l - 1)

    Time Complexity: O(1) - constant-time pattern lookup, no loop
    Space Complexity: O(1) - no extra memory used
*/
int prefixXor(int n) {
  if (n < 0) return 0;   // prefixXor of an empty range (used when l = 0)

  switch (n % 4) {
    case 0: return n;
    case 1: return 1;
    case 2: return n + 1;
    default: return 0;   // n % 4 == 3
  }
}

int xorOfRange(int l, int r) {
  return prefixXor(r) ^ prefixXor(l - 1);
}

int main() {
  int l = 2, r = 5;
  cout << "l = " << l << ", r = " << r << endl;
  cout << "XOR of range: " << xorOfRange(l, r) << endl;

  l = 4, r = 8;
  cout << "\nl = " << l << ", r = " << r << endl;
  cout << "XOR of range: " << xorOfRange(l, r) << endl;

  return 0;
}
