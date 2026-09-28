#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: arr[i] is the day the i-th flower in a row blooms. A bouquet needs k
// ADJACENT bloomed flowers, and each flower can be in only one bouquet.
// Return the minimum number of days to wait to make m bouquets, or -1 if it
// is impossible.
//
// Example:
// arr = {1, 10, 3, 10, 2},       m = 3, k = 1   -> 3
// arr = {1, 10, 3, 10, 2},       m = 3, k = 2   -> -1  (needs 6 flowers, has 5)
// arr = {7, 7, 7, 7, 12, 7, 7},  m = 2, k = 3   -> 12

/*
    Approach: Binary search on the answer (the day)

    - If m * k > n there are not enough flowers at all: return -1. Otherwise
      on day max(arr) every flower has bloomed, the row gives n / k >= m
      bouquets, so an answer always exists in 1..max(arr).
    - Waiting longer only blooms more flowers, so bouquets(day) never
      decreases. The predicate "bouquets(day) >= m" is false and then true,
      and we want the FIRST true day.
    - bouquets(day) is one left-to-right scan with a run counter c:
        arr[i] <= day  ->  bloomed, c++. When c reaches k, that is a
                           bouquet: b++ and start a new run (c = 0).
        arr[i] >  day  ->  not bloomed, it breaks adjacency: c = 0.
      Taking a bouquet as soon as k flowers line up is optimal - using the
      earliest flowers never leaves fewer options for later ones.
    - Probe mid:
        b >= m  ->  mid is enough, an earlier day might be too: high = mid.
        b <  m  ->  too early: low = mid + 1.

    Algorithm Steps
    ----------------
    1. If m * k > n, return -1.
    2. Set low = 1, high = max(arr).
    3. While low < high, compute mid = low + (high - low) / 2.
    4. Scan arr, counting bouquets b of k adjacent flowers with arr[i] <= mid.
    5. If b >= m, high = mid.
    6. Else low = mid + 1.
    7. Return low.

    Time Complexity: O(n * log(max(arr))) - log(max) probes, each an O(n)
                      scan.
    Space Complexity: O(1)
*/
int minDays(vector<int>& arr, int m, int k) {
  int n = arr.size();
  if ((long long)m * k > n) {
    return -1;  // not enough flowers even if all bloom
  }

  int low = 1, high = *max_element(begin(arr), end(arr));

  while (low < high) {
    int mid = low + (high - low) / 2;  // to prevent overflow
    int c = 0;  // consecutive bloomed flowers in the current run
    int b = 0;  // bouquets made by day mid

    for (int i : arr) {
      if (i <= mid) {
        c++;
        if (c == k) {
          b++;  // k in a row -> one bouquet, start a fresh run
          c = 0;
        }
      } else {
        c = 0;  // an unbloomed flower breaks adjacency
      }
    }

    if (b >= m) {
      high = mid;  // enough bouquets, try an earlier day
    } else {
      low = mid + 1;  // too early
    }
  }

  return low;
}

int main() {
  struct Test {
    vector<int> arr;
    int m, k;
  };
  vector<Test> tests = {{{1, 10, 3, 10, 2}, 3, 1}, {{1, 10, 3, 10, 2}, 3, 2}, {{7, 7, 7, 7, 12, 7, 7}, 2, 3}};

  for (auto& t : tests) {
    cout << "Bloom days: ";
    for (int v : t.arr) cout << v << " ";
    cout << " m = " << t.m << ", k = " << t.k << endl;
    cout << "  min days: " << minDays(t.arr, t.m, t.k) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of minDays: arr = {7, 7, 7, 7, 12, 7, 7}, m = 2, k = 3
    (answer = 12)
    ======================================================================

    m * k = 6 <= n = 7, so an answer exists.
    Initial state: low = 1, high = max(arr) = 12

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 1, high = 12
      mid          1 + (12 - 1) / 2 = 6
      bloomed      arr[i] <= 6 ?   .  .  .  .  .  .  .   (none)
      bouquets     b = 0 < 2  ->  too early
      update       low = 7

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 7, high = 12
      mid          7 + (12 - 7) / 2 = 9
      bloomed      arr[i] <= 9 ?   B  B  B  B  .  B  B
      scan         c: 1 2 3 -> b = 1, c = 0
                   c: 1           (4th flower)
                   12 > 9 -> c = 0
                   c: 1 2         (run of 2 < k at the end)
      bouquets     b = 1 < 2  ->  too early
      update       low = 10

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 10, high = 12
      mid          10 + (12 - 10) / 2 = 11
      bloomed      same as day 9 (nothing blooms on days 8..11)
      bouquets     b = 1 < 2  ->  low = 12

    ----------------------------------------------------------------------
    Loop ends at low = high = 12.  RETURN 12
      (on day 12 all 7 flowers bloom: B B B | B B B | B -> 2 bouquets)

    ======================================================================
    Summary table: arr = {1, 10, 3, 10, 2}, m = 3, k = 1
    ======================================================================

    | iter | low | high | mid | bloomed (<= mid) | b | >= 3 ? | action    |
    |------|-----|------|-----|------------------|---|--------|-----------|
    |  1   |  1  |  10  |  5  | 1, 3, 2          | 3 |  yes   | high = 5  |
    |  2   |  1  |  5   |  3  | 1, 3, 2          | 3 |  yes   | high = 3  |
    |  3   |  1  |  3   |  2  | 1, 2             | 2 |  no    | low  = 3  |

      Loop ends at low = high = 3.  RETURN 3

    ======================================================================
    Notes
    ======================================================================

    Change from the original: (long) -> (long long) in the m * k check.
      With m up to 10^6 and k up to 10^5, m * k reaches 10^11. long is
      64-bit on Linux but only 32-bit on Windows, where the product would
      overflow and the impossible case could slip past the check.

    Why the m * k > n check has to come first:
      without it the search still ends at some day in 1..max(arr) and
      returns it, even though no day ever gives m bouquets. The binary
      search assumes the answer exists; the check guarantees that.

    Optional tightening: low = min(arr).
      No flower blooms before day min(arr), so every mid below it gives
      b = 0. Starting at low = 1 is still correct, just a few extra probes.
      The same single pass can find both min and max.

    Edge cases:
      - k = 1: every bloomed flower is its own bouquet; the answer is the
        m-th smallest bloom day.
      - m * k == n: every flower is needed, so the answer is max(arr).
*/
