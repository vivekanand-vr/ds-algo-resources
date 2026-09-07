#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Swap two integers without using a temporary variable
//
// Example:
// a = 5, b = 9
// After swap: a = 9, b = 5

/*
    Approach: XOR Swap

    - a ^ b produces a value that encodes exactly the bits where a and b
      differ, without losing either original value.
    - Re-XORing that combined value with b recovers the original a,
      and storing it into a gives us the new a.
    - XORing the combined value with the (now updated) a recovers the
      original b.
    - This works because XOR is its own inverse: x ^ y ^ y = x.

    Algorithm Steps
    ----------------
    1. a = a ^ b            (a now holds the XOR of both original values)
    2. b = a ^ b            (b becomes the original a)
    3. a = a ^ b            (a becomes the original b)

    Caution: if a and b are the same variable (same memory address),
    step 1 makes it 0, and the rest of the swap zeroes it out entirely.
    Guard against self-swap when a and b might alias the same location.

    Time Complexity: O(1) - three bitwise operations
    Space Complexity: O(1) - no temporary variable needed
*/
void swapNumbers(int &a, int &b) {
  if (&a == &b) return;   // guard against self-swap aliasing

  a = a ^ b;
  b = a ^ b;
  a = a ^ b;
}

int main() {
  int a = 5, b = 9;

  cout << "Before swap: a = " << a << ", b = " << b << endl;
  swapNumbers(a, b);
  cout << "After swap:  a = " << a << ", b = " << b << endl;

  return 0;
}
