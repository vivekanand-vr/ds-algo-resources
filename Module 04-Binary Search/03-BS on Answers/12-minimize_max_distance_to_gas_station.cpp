#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: (LeetCode 774) On a horizontal number line there are gas stations at
// positions stations[0..n-1] (strictly increasing, 0 <= stations[i] <= 10^8).
// Add k more gas stations (anywhere, not necessarily at integer positions)
// so that D, the MAXIMUM distance between adjacent gas stations, is as SMALL
// as possible. Return that smallest D (answers within 10^-6 are accepted).
//
// Example:
// stations = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},            k = 9  -> 0.5
// stations = {23, 24, 36, 39, 46, 56, 57, 65, 84, 98},   k = 1  -> 14.0
// stations = {1, 7},                                     k = 2  -> 2.0
//                                        (new stations at 3 and 5)

/*
    Approach: Binary search on the answer (the maximum gap D), over doubles

    - For a target gap D, stationsNeeded(D) counts how many new stations
      bring every gap down to <= D. A gap of length g is split into
      ceil(g / D) equal pieces, which takes ceil(g / D) - 1 new stations.
      Gaps are independent, so the total is the sum over all gaps.
    - A larger D never needs more stations, so the predicate
      "stationsNeeded(D) <= k" is false and then true. We want the FIRST
      true - the smallest D that k stations can achieve.
    - The answer lies in 0..maxGap: adding stations can only shrink gaps,
      and with no new stations the largest gap is maxGap.
    - D is a real number, so there is no mid + 1 / mid - 1. Instead the
      search keeps halving [low, high] until it is narrower than 10^-6:
        stationsNeeded(mid) <= k  ->  mid works, try smaller: high = mid.
        stationsNeeded(mid) >  k  ->  mid is too small:       low = mid.
      Same "first true" shape as 03-koko_eating_bananas.cpp, just on a
      continuous range (like 02-nth_root_of_m.cpp with real roots).

    Algorithm Steps
    ----------------
    1. low = 0, high = the largest gap between adjacent stations.
    2. While high - low > 1e-6, compute mid = low + (high - low) / 2.
    3. stationsNeeded(mid): for every gap g, add ceil(g / mid) - 1.
    4. If stationsNeeded(mid) <= k, high = mid.
    5. Else low = mid.
    6. Return high.

    Time Complexity: O(n * log(maxGap / 1e-6)) - one O(n) count per probe,
                      and about log2(maxGap / 1e-6) probes (~47 for the
                      largest possible gap of 10^8).
    Space Complexity: O(1)
*/
long long stationsNeeded(vector<int>& stations, double dist) {
  long long needed = 0;

  for (int i = 1; i < stations.size(); i++) {
    double gap = stations[i] - stations[i - 1];
    needed += (long long)ceil(gap / dist) - 1;  // pieces - 1 new stations
  }

  return needed;
}

double minmaxGasDist(vector<int>& stations, int k) {
  double low = 0, high = 0;
  for (int i = 1; i < stations.size(); i++) {
    high = max(high, (double)(stations[i] - stations[i - 1]));  // largest gap
  }

  while (high - low > 1e-6) {
    double mid = low + (high - low) / 2;

    if (stationsNeeded(stations, mid) <= k) {
      high = mid;  // k stations are enough, try a smaller max gap
    } else {
      low = mid;  // gap too small, needs more than k stations
    }
  }

  return high;  // smallest max gap reachable, within 1e-6
}

int main() {
  vector<pair<vector<int>, int>> tests = {
      {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 9}, {{23, 24, 36, 39, 46, 56, 57, 65, 84, 98}, 1}, {{1, 7}, 2}};

  cout << fixed << setprecision(5);
  for (auto& [stations, k] : tests) {
    cout << "Stations: ";
    for (int v : stations) cout << v << " ";
    cout << " k = " << k << endl;
    cout << "  smallest max distance: " << minmaxGasDist(stations, k) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of minmaxGasDist: stations = {1, 7}, k = 2
    (answer = 2.0)
    ======================================================================

    Gaps:            7 - 1 = 6
    Initial state:   low = 0, high = 6

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 6
      mid          3
      needed       ceil(6 / 3) - 1 = 2 - 1 = 1
      result       1 <= 2  ->  fits
      update       high = 3

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 0, high = 3
      mid          1.5
      needed       ceil(6 / 1.5) - 1 = 4 - 1 = 3
      result       3 > 2  ->  gap too small
      update       low = 1.5

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 1.5, high = 3
      mid          2.25
      needed       ceil(2.67) - 1 = 3 - 1 = 2
      result       2 <= 2  ->  fits
      update       high = 2.25

    ----------------------------------------------------------------------
    Iteration 4
      bounds       low = 1.5, high = 2.25
      mid          1.875
      needed       ceil(3.2) - 1 = 4 - 1 = 3
      result       too small
      update       low = 1.875

    ----------------------------------------------------------------------
    Iteration 5
      bounds       low = 1.875, high = 2.25
      mid          2.0625
      needed       ceil(2.91) - 1 = 2
      result       fits
      update       high = 2.0625

    ... every probe above 2 needs 2 stations (fits) and every probe below
    2 needs 3 (too small), so [low, high] keeps closing in on 2 from both
    sides. After 23 iterations the width 6 / 2^23 is below 1e-6.

    RETURN high ~ 2.000000  (new stations at 3 and 5, gaps 2 2 2)

    ======================================================================
    Summary table
    ======================================================================

    | iter |  low   |  high  |  mid   | needed | fits k = 2 ? | action        |
    |------|--------|--------|--------|--------|--------------|---------------|
    |  1   | 0      | 6      | 3      |   1    |  yes         | high = 3      |
    |  2   | 0      | 3      | 1.5    |   3    |  no          | low  = 1.5    |
    |  3   | 1.5    | 3      | 2.25   |   2    |  yes         | high = 2.25   |
    |  4   | 1.5    | 2.25   | 1.875  |   3    |  no          | low  = 1.875  |
    |  5   | 1.875  | 2.25   | 2.0625 |   2    |  yes         | high = 2.0625 |
    |  ... |        |        |        |        |              | -> 2.0        |

    ======================================================================
    Notes
    ======================================================================

    Why ceil(g / D) - 1 stations for one gap:
      to make every piece <= D, a gap g needs at least ceil(g / D) pieces,
      and p pieces need p - 1 new stations between them. When g is an exact
      multiple of D (g = 6, D = 2) this gives 3 - 1 = 2, not 3.

    Why low = mid and high = mid (no +1 / -1):
      D is continuous, so there is no "next" value to skip to. The loop
      instead stops on a tolerance. A fixed count such as 100 iterations
      works just as well and sidesteps any floating-point worries about the
      stop condition.

    Why return high:
      high is always a D that has been shown to fit k stations (or the
      starting maxGap, which needs none), so it is a valid answer, and it
      is within 1e-6 of the true minimum.

    No division by zero: the loop only runs while high - low > 1e-6, so
      mid > 0 on every probe.

    Why the count is long long: a gap of 10^8 probed at a tiny D can need
      far more than 2^31 stations before the search moves low up.

    Other approaches:
      - Greedy with a max-heap: place the k stations one at a time, each in
        the gap whose current piece length (g / (pieces)) is largest.
        O(k log n) - fine for small k, but k can be 10^6 here.
      - The binary search above is independent of k, which is why it is
        the usual answer.

    Edge cases:
      - evenly spaced stations with k a multiple of n - 1: every gap gets
        the same number of stations (example 1: 9 gaps of 1, one station
        each -> 0.5).
      - the answer can be an untouched original gap: in example 2 the one
        new station splits the largest gap 65..84 (19) into 9.5 + 9.5, and
        the next largest gap 84..98 (14) becomes the maximum -> 14.0.
*/
