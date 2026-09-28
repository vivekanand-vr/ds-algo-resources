#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a non-negative integer x, return the square root of x rounded DOWN
// to the nearest integer, without using any built-in pow / sqrt.
//
// Example:
// x = 4            -> 2
// x = 8            -> 2   (sqrt(8) = 2.828...)
// x = 0            -> 0
// x = 1            -> 1
// x = 2147483647   -> 46340

/*
    Approach: Binary search on the answer

    - The answer is the LARGEST r with r * r <= x. The predicate
      "mid * mid <= x" is true for every r up to the answer and false after
      it, so the candidates 1..x are split into a true block and a false
      block - exactly the shape binary search needs.
    - Probe mid and compare mid * mid with x:
        mid * mid == x  ->  perfect square, mid is the answer.
        mid * mid <  x  ->  mid is a valid floor, but a bigger one may exist:
                            low = mid + 1.
        mid * mid >  x  ->  mid is too big: high = mid - 1.
    - The loop runs while low <= high. When it ends, low = high + 1 and
      high sits on the last value whose square is <= x, so high is the floor.
    - mid * mid is computed in long long: mid can be ~10^9 on the first
      probe, and its square does not fit in an int.

    Algorithm Steps
    ----------------
    1. If x <= 1, return x.
    2. Set low = 1, high = x.
    3. While low <= high, compute mid = low + (high - low) / 2.
    4. If mid * mid == x, return mid.
    5. Else if mid * mid < x, low = mid + 1.
    6. Else high = mid - 1.
    7. Return high.

    Time Complexity: O(log x) - the range 1..x halves every iteration.
    Space Complexity: O(1)
*/
int mySqrt(int x) {
  if (x <= 1) return x;
  int low = 1, high = x;

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if ((long long)mid * mid == (long long)x) {
      return mid;  // perfect square
    } else if ((long long)mid * mid < (long long)x) {
      low = mid + 1;  // mid fits, look for a bigger one
    } else {
      high = mid - 1;  // mid is too big
    }
  }

  return high;  // last value whose square is still <= x
}

int main() {
  vector<int> tests = {4, 8, 0, 1, 2147483647};

  for (int x : tests) {
    cout << "sqrt(" << x << ") = " << mySqrt(x) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of mySqrt: x = 8
    (answer = 2, since 2 * 2 = 4 <= 8 < 9 = 3 * 3)
    ======================================================================

      candidate:   1   2   3   4   5   6   7   8
      r * r <= 8:  T   T   F   F   F   F   F   F
                       ^
                       last T -> answer

    Initial state: low = 1, high = 8

    | iter | low | high | mid | mid * mid | vs x = 8 | action      |
    |------|-----|------|-----|-----------|----------|-------------|
    |  1   |  1  |  8   |  4  |    16     |  > 8     | high = 3    |
    |  2   |  1  |  3   |  2  |     4     |  < 8     | low  = 3    |
    |  3   |  3  |  3   |  3  |     9     |  > 8     | high = 2    |

      Loop ends with low = 3 > high = 2.  RETURN high = 2

    ======================================================================
    Notes
    ======================================================================

    Why return high and not low:
      every value <= high has been proven to satisfy r * r <= x, and every
      value >= low has been proven to fail it. At exit low = high + 1, so
      high is the largest passing value (the floor) and low is the ceiling.

    Change from the original: (long) -> (long long).
      long is 64-bit on Linux (where LeetCode runs), but only 32-bit on
      Windows (MSVC and MinGW). For x = 2147483647 the first probe is
      mid = 1073741824, and mid * mid = 2^60 overflows a 32-bit long.
      long long is 64-bit everywhere.

    Optional tightening: high = x / 2.
      For x >= 4, sqrt(x) <= x / 2, so starting at high = x / 2 saves one
      iteration. It does not change the complexity.

    Edge cases:
      - x = 0 or x = 1: returned directly, the loop never runs.
      - x = 2 or x = 3: low = 1 fits, the loop ends with high = 1.
      - x = INT_MAX: the answer is 46340 (46340^2 = 2147395600 <= x).
*/
