#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given two integers n and m, return the integer r such that r^n == m.
// If m has no integer n-th root, return -1.
//
// Example:
// n = 2, m = 9     -> 3
// n = 3, m = 27    -> 3
// n = 3, m = 9     -> -1  (2^3 = 8 < 9 < 27 = 3^3)
// n = 1, m = 14    -> 14
// n = 4, m = 0     -> 0

/*
    Approach: Binary search on the answer, with an overflow-safe power

    - r^n grows with r, so over the candidates 1..m the value mid^n is
      monotonic: compare it with m to decide which half to drop.
        mid^n == m  ->  found the root.
        mid^n >  m  ->  mid is too big: high = mid - 1.
        mid^n <  m  ->  mid is too small: low = mid + 1.
    - Computing mid^n directly overflows fast (e.g. 500000^3). Instead the
      power is built one multiplication at a time, and BEFORE each multiply
      it checks power > m / mid. For positive integers that is exactly
      "power * mid > m", so as soon as the product would pass m the loop
      stops and marks power as m + 1 - all that matters is that it is > m.
      Because of the guard power never exceeds m, so every multiply is safe.
    - If the search empties without an exact match, m is not a perfect
      n-th power: return -1.

    Algorithm Steps
    ----------------
    1. If m == 0, return 0.
    2. Set low = 1, high = m.
    3. While low <= high, compute mid = low + (high - low) / 2.
    4. Build power = mid^n, stopping with power = m + 1 if it would pass m.
    5. If power == m, return mid.
    6. Else if power > m, high = mid - 1.
    7. Else low = mid + 1.
    8. Return -1.

    Time Complexity: O(n * log m) - log m probes, each doing up to n
                      multiplications.
    Space Complexity: O(1)
*/
int nthRoot(int n, int m) {
  if (m == 0) return 0;
  int low = 1, high = m;

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    long long power = 1;
    for (int i = 1; i <= n; ++i) {
      if (power > m / mid) {
        power = (long long)m + 1;  // power * mid would pass m, mark as greater than m
        break;
      }
      power *= mid;
    }

    if (power == m) {
      return mid;  // exact root
    } else if (power > m) {
      high = mid - 1;  // mid^n is too big
    } else {
      low = mid + 1;  // mid^n is too small
    }
  }

  return -1;  // no integer root
}

int main() {
  vector<pair<int, int>> tests = {{2, 9}, {3, 27}, {3, 9}, {1, 14}, {4, 0}};

  for (auto& [n, m] : tests) {
    cout << "n = " << n << ", m = " << m << "  ->  " << nthRoot(n, m) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of nthRoot: n = 3, m = 27
    (answer = 3)
    ======================================================================

    Initial state: low = 1, high = 27

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 1, high = 27
      mid          1 + (27 - 1) / 2 = 14
      power        m / mid = 27 / 14 = 1
                   i = 1: power = 1  > 1 ?  no   ->  power = 14
                   i = 2: power = 14 > 1 ?  YES  ->  power = 28, stop
      compare      28 > 27  ->  14 is too big
      update       high = 13

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 1, high = 13
      mid          1 + (13 - 1) / 2 = 7
      power        m / mid = 27 / 7 = 3
                   i = 1: 1 > 3 ?  no   ->  power = 7
                   i = 2: 7 > 3 ?  YES  ->  power = 28, stop
      compare      28 > 27  ->  high = 6

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 1, high = 6
      mid          1 + (6 - 1) / 2 = 3
      power        m / mid = 27 / 3 = 9
                   i = 1: 1 > 9 ?  no  ->  power = 3
                   i = 2: 3 > 9 ?  no  ->  power = 9
                   i = 3: 9 > 9 ?  no  ->  power = 27
      compare      27 == 27  ->  RETURN 3

    ======================================================================
    Summary table: n = 3, m = 9 (no root)
    ======================================================================

    | iter | low | high | mid | m / mid | power      | vs m = 9 | action    |
    |------|-----|------|-----|---------|------------|----------|-----------|
    |  1   |  1  |  9   |  5  |    1    | 10 (stop)  |  > 9     | high = 4  |
    |  2   |  1  |  4   |  2  |    4    |  8         |  < 9     | low  = 3  |
    |  3   |  3  |  4   |  3  |    3    | 10 (stop)  |  > 9     | high = 2  |

      Loop ends with low = 3 > high = 2.  RETURN -1

    ======================================================================
    Notes
    ======================================================================

    Why "power > m / mid" is exact and not an approximation:
      m / mid is integer division, i.e. floor(m / mid). For a positive
      integer power, power * mid > m  <=>  power > m / mid (real)
      <=>  power > floor(m / mid). So the guard never stops too early or
      too late - it is not a heuristic.

    Changes from the original:
      - power = m + 1 became (long long)m + 1. m + 1 is computed in int
        before it is stored, so m = INT_MAX would overflow. The cast makes
        the addition happen in 64 bits.
      - The m == 0 check moved above the variable setup and the
        "// Code here" placeholder was removed. Behaviour is unchanged.

    Optional tightening: high = m / 2 for n >= 2 and m >= 4, since the root
      is at most sqrt(m) <= m / 2. Like in 01-sqrt.cpp, it only saves a
      single iteration.

    Edge cases:
      - n = 1: power = mid, so the search is a plain search for m -> m.
      - m = 1: low = high = 1, 1^n = 1 -> 1.
      - m = 0: returned directly (0^n = 0 for n >= 1).
*/
