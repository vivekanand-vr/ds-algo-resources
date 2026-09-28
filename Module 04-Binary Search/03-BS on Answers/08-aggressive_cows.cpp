#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: (SPOJ AGGRCOW) A barn has n stalls along a straight line at positions
// stalls[0..n-1] (0 <= stalls[i] <= 10^9, in any order). Place k cows
// (2 <= k <= n) in the stalls so that the MINIMUM distance between any two
// cows is as LARGE as possible, and return that largest minimum distance.
//
// Example:
// stalls = {1, 2, 8, 4, 9},        k = 3   -> 3   (cows at 1, 4, 8)
// stalls = {10, 1, 2, 7, 5},       k = 3   -> 4   (cows at 1, 5, 10)
// stalls = {2, 12, 11, 3, 26, 7},  k = 5   -> 1

/*
    Approach: Binary search on the answer (the minimum distance)

    - Sort the stalls first; only their order along the line matters.
    - For a distance dist, canPlace(dist) asks: can k cows be placed with
      every pair at least dist apart? Greedily put the first cow in the
      first stall, then each next cow in the first stall that is >= dist
      past the previous cow. Taking the earliest valid stall is optimal -
      it leaves the most room for the cows still to come.
    - If the cows fit at distance dist they also fit at any smaller one, so
      canPlace is true and then false. We want the LAST true.
    - The answer lies in 1..(stalls[n-1] - stalls[0]): with distinct
      positions any two cows are at least 1 apart, and no two cows can be
      further apart than the two end stalls.
    - Probe mid:
        canPlace(mid)  ->  mid works, a larger gap might too: low = mid + 1.
        !canPlace(mid) ->  mid is too large: high = mid - 1.
      This is the same "last true" search as 01-sqrt.cpp, so the answer is
      high when the loop ends.

    Algorithm Steps
    ----------------
    1. Sort stalls.
    2. Set low = 1, high = stalls[n - 1] - stalls[0].
    3. While low <= high, compute mid = low + (high - low) / 2.
    4. canPlace(mid): cows = 1, last = stalls[0]; for each next stall, if
       stall - last >= mid, place a cow there (cows++, last = stall);
       true as soon as cows == k.
    5. If canPlace(mid), low = mid + 1.
    6. Else high = mid - 1.
    7. Return high.

    Time Complexity: O(n log n + n * log(range)) - the sort, then one O(n)
                      greedy check per probe, where range is
                      stalls[n-1] - stalls[0].
    Space Complexity: O(1) extra, apart from what the sort uses.
*/
bool canPlace(vector<int>& stalls, int dist, int k) {
  int cows = 1;  // the first cow always goes in the first stall
  int last = stalls[0];  // position of the most recently placed cow

  for (int i = 1; i < stalls.size(); i++) {
    if (stalls[i] - last >= dist) {
      cows++;  // far enough from the previous cow, place one here
      last = stalls[i];
      if (cows == k) return true;
    }
  }

  return false;
}

int aggressiveCows(vector<int>& stalls, int k) {
  sort(stalls.begin(), stalls.end());
  int n = stalls.size();
  int low = 1, high = stalls[n - 1] - stalls[0];

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (canPlace(stalls, mid, k)) {
      low = mid + 1;  // all k cows fit, try a larger gap
    } else {
      high = mid - 1;  // gap too large, not all cows fit
    }
  }

  return high;  // last distance at which all k cows fit
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{1, 2, 8, 4, 9}, 3}, {{10, 1, 2, 7, 5}, 3}, {{2, 12, 11, 3, 26, 7}, 5}};

  for (auto& [stalls, k] : tests) {
    cout << "Stalls: ";
    for (int v : stalls) cout << v << " ";
    cout << " k = " << k << endl;
    cout << "  largest min distance: " << aggressiveCows(stalls, k) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of aggressiveCows: stalls = {1, 2, 8, 4, 9}, k = 3
    (answer = 3)
    ======================================================================

    Sorted stalls:   1   2   4   8   9
    Initial state:   low = 1, high = 9 - 1 = 8

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 1, high = 8
      mid          1 + (8 - 1) / 2 = 4
      placing      cow at 1
                   2: 2 - 1 = 1 < 4    skip
                   4: 4 - 1 = 3 < 4    skip
                   8: 8 - 1 = 7 >= 4   cow at 8
                   9: 9 - 8 = 1 < 4    skip
      result       2 cows < 3  ->  gap 4 is too large
      update       high = 3

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 1, high = 3
      mid          1 + (3 - 1) / 2 = 2
      placing      cow at 1
                   2: 1 < 2            skip
                   4: 3 >= 2           cow at 4
                   8: 4 >= 2           cow at 8  -> 3 cows, stop
      result       fits  ->  try larger
      update       low = 3

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 3, high = 3
      mid          3
      placing      cow at 1
                   2: 1 < 3            skip
                   4: 3 >= 3           cow at 4
                   8: 4 >= 3           cow at 8  -> 3 cows, stop
      result       fits  ->  try larger
      update       low = 4

    ----------------------------------------------------------------------
    Loop ends with low = 4 > high = 3.  RETURN 3

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | cows placed at | fits k = 3 ? | action    |
    |------|-----|------|-----|----------------|--------------|-----------|
    |  1   |  1  |  8   |  4  | 1, 8           |  no          | high = 3  |
    |  2   |  1  |  3   |  2  | 1, 4, 8        |  yes         | low  = 3  |
    |  3   |  3  |  3   |  3  | 1, 4, 8        |  yes         | low  = 4  |

    ======================================================================
    Notes
    ======================================================================

    Why return high and not low:
      every distance <= high has been proven to fit all k cows and every
      distance >= low has been proven not to. At exit low = high + 1, so
      high is the largest distance that fits.

    Minimum vs maximum problems in this folder:
      03-koko, 04-bouquets, 05-divisor and 06-ship all want the FIRST value
      that works (high = mid on success). Here we want the LAST value that
      works, so success moves low up instead - same shape as 01-sqrt.cpp.

    Reading SPOJ input:
      the SPOJ version reads t test cases, each as n and k followed by n
      stall positions. To submit it there, replace main with:
          int t; cin >> t;
          while (t--) {
            int n, k; cin >> n >> k;
            vector<int> stalls(n);
            for (int& x : stalls) cin >> x;
            cout << aggressiveCows(stalls, k) << endl;
          }

    Overflow: positions are at most 10^9, so every difference and every
      mid fits in an int.

    Edge cases:
      - k == 2: the two cows go at the two ends -> stalls[n-1] - stalls[0].
      - k == n: every stall is used -> the smallest adjacent gap.
        Example 3 (k = 5, n = 6) also ends at 1: its sorted gaps are
        1 4 4 1 14, and only four cows fit at distance 2.
      - duplicate positions with k too large for distinct spots: high can
        end at 0, meaning cows must share a position.
*/
