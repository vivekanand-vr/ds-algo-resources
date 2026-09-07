#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given two integers start and goal, return the minimum number of bit
//    flips required to convert start into goal
//
// Example:
// start = 10 (1010), goal = 7 (0111)
// Flipping bits 0, 1 and 3 of start turns it into goal -> 3 flips

/*
    Approach: XOR + Brian Kernighan's Algorithm

    - XORing start and goal produces a 1 in exactly the bit positions
      where the two numbers differ, and a 0 everywhere they already
      agree - flipping start into goal means flipping precisely those
      differing positions.
    - So the answer is just the number of set bits in (start xor goal).
    - Counting those set bits with Kernighan's trick, n & (n - 1)
      repeatedly clears the lowest set bit, so the number of iterations
      until n reaches 0 is exactly the popcount.

    Algorithm Steps
    ----------------
    1. Compute diff = start xor goal
    2. Repeatedly clear the lowest set bit of diff (diff = diff & (diff - 1)),
       counting how many times this is done
    3. Return that count

    Time Complexity: O(k) - k is the number of differing bits (worst case O(log n))
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

int minBitFlips(int start, int goal) {
  int diff = start ^ goal;   // 1s mark exactly the differing bit positions
  return countSetBits(diff);
}

int main() {
  int start = 10, goal = 7;
  cout << "start = " << start << " (binary: " << bitset<8>(start) << ")" << endl;
  cout << "goal  = " << goal << " (binary: " << bitset<8>(goal) << ")" << endl;
  cout << "Minimum bit flips: " << minBitFlips(start, goal) << endl;

  start = 3, goal = 4;
  cout << "\nstart = " << start << " (binary: " << bitset<8>(start) << ")" << endl;
  cout << "goal  = " << goal << " (binary: " << bitset<8>(goal) << ")" << endl;
  cout << "Minimum bit flips: " << minBitFlips(start, goal) << endl;

  return 0;
}
