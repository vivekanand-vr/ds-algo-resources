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
