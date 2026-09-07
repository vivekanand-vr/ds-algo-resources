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
