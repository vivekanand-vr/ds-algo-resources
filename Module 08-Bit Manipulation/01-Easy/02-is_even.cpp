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
