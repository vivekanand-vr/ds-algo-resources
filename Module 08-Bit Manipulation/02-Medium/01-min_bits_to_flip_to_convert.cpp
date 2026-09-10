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

/*
    ======================================================================
    DRY RUN: start = 10, goal = 7   (answer = 3 flips)
    ======================================================================

      bit position :   3   2   1   0
      start = 10   :   1   0   1   0
      goal  =  7   :   0   1   1   1
      differ?      :   Y   Y   n   Y    <-- bits 3, 2, 0 disagree

    Tracked state:
      diff  - start ^ goal; a 1 marks a position that must be flipped
      n     - the working copy of diff inside countSetBits; it loses its
              lowest set bit on every pass
      count - how many set bits have been cleared so far (= flip count)

    Step 0 - build diff (inside minBitFlips):
      xor        1010 ^ 0111 = 1101   ->   diff = 13
                 bit0: 0^1=1   bit1: 1^1=0   bit2: 0^1=1   bit3: 1^0=1

    Initial state: n = 1101 (13), count = 0

    ----------------------------------------------------------------------
    pass 1
      n           1101   (13)
      n - 1       1100   (12)     the borrow turns the low 1 into 0
      mask        1101 & 1100 = 1100   (12)    <-- bit0 cleared
      n becomes   1100   (12)
      count       0 -> 1

    ----------------------------------------------------------------------
    pass 2
      n           1100   (12)
      n - 1       1011   (11)     borrow ripples across the two low 0s
      mask        1100 & 1011 = 1000   (8)     <-- bit2 cleared
      n becomes   1000   (8)
      count       1 -> 2

    ----------------------------------------------------------------------
    pass 3
      n           1000   (8)
      n - 1       0111   (7)
      mask        1000 & 0111 = 0000   (0)     <-- bit3 cleared
      n becomes   0000   (0)
      count       2 -> 3
      loop test   n == 0  ->  exit the while loop

    ----------------------------------------------------------------------
    RETURN count = 3

    ======================================================================
    Summary table
    ======================================================================

    | pass | n before    | n - 1       | n & (n-1)   | cleared | count |
    |------|-------------|-------------|-------------|---------|-------|
    |  1   | 1101 (13)   | 1100 (12)   | 1100 (12)   |  bit0   |   1   |
    |  2   | 1100 (12)   | 1011 (11)   | 1000  (8)   |  bit2   |   2   |
    |  3   | 1000  (8)   | 0111  (7)   | 0000  (0)   |  bit3   |   3   |

    Why the trick works:
      subtracting 1 flips the lowest set bit to 0 and sets every bit
      BELOW it to 1, while leaving every bit above it untouched. ANDing
      the two therefore keeps the high part unchanged and wipes out the
      whole low run - exactly one set bit disappears per pass.

    Step count backing O(k):
      diff = 1101 has 3 set bits, and the loop ran exactly 3 times - not
      once per bit of the word (a naive shift-and-test loop would need 4
      passes here, or 32 for a full int).

    Note on the Example header:
      the positions that actually differ are bits 0, 2 and 3 (diff =
      1101), which is what the trace above follows. The count of 3 is
      the same either way.
*/
