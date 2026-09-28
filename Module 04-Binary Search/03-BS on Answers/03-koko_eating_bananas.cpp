#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Koko has n piles of bananas, arr[i] in the i-th pile, and h hours. Each
// hour she picks one pile and eats up to k bananas from it (if the pile has
// fewer, she finishes it and waits out the hour). Return the MINIMUM integer
// speed k that lets her eat every banana within h hours. (n <= h is given.)
//
// Example:
// arr = {3, 6, 7, 11},        h = 8   -> 4
// arr = {30, 11, 23, 4, 20},  h = 5   -> 30
// arr = {30, 11, 23, 4, 20},  h = 6   -> 23

/*
    Approach: Binary search on the answer (the speed)

    - At speed k, pile p takes ceil(p / k) hours, so the total time is
      hours(k) = sum of ceil(arr[i] / k). A faster speed never takes longer,
      so hours(k) is non-increasing in k, and the predicate
      "hours(k) <= h" is false for small k and true from the answer onward.
      We want the FIRST true.
    - The answer lies in 1..max(arr): speed max(arr) finishes every pile in
      one hour (n hours total, and n <= h), and eating faster than that
      gains nothing.
    - Probe mid:
        hours(mid) <= h  ->  mid works, but a slower speed might too:
                             high = mid (keep mid, it may be the answer).
        hours(mid) >  h  ->  mid is too slow: low = mid + 1.
    - The loop runs while low < high and keeps mid on the high = mid branch,
      so it converges on the smallest speed that works.

    Algorithm Steps
    ----------------
    1. Set low = 1, high = max(arr).
    2. While low < high, compute mid = low + (high - low) / 2.
    3. Compute hours = sum of ceil(arr[i] / mid).
    4. If hours <= h, high = mid.
    5. Else low = mid + 1.
    6. Return low.

    Time Complexity: O(n * log(max(arr))) - log(max) probes, each summing
                      over all n piles.
    Space Complexity: O(1)
*/
int minEatingSpeed(vector<int>& arr, int h) {
  int low = 1;
  int high = *max_element(begin(arr), end(arr));

  while (low < high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    long long count = 0;  // total hours at speed mid
    for (int i : arr) {
      count += ceil((double)i / mid);
    }

    if (count <= h) {
      high = mid;  // mid is fast enough, try slower
    } else {
      low = mid + 1;  // mid is too slow
    }
  }

  return low;
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{3, 6, 7, 11}, 8}, {{30, 11, 23, 4, 20}, 5}, {{30, 11, 23, 4, 20}, 6}};

  for (auto& [arr, h] : tests) {
    cout << "Piles: ";
    for (int v : arr) cout << v << " ";
    cout << " h = " << h << endl;
    cout << "  min speed: " << minEatingSpeed(arr, h) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of minEatingSpeed: arr = {3, 6, 7, 11}, h = 8
    (answer = 4)
    ======================================================================

      speed k:        1   2   3   4   5   6   7   8   9  10  11
      hours(k):      27  15  10   8   8   6   5   5   5   5   4
      hours <= 8:     F   F   F   T   T   T   T   T   T   T   T
                                  ^
                                  first T -> answer

    Initial state: low = 1, high = max(arr) = 11

    | iter | low | high | mid | ceil per pile  | hours | <= 8 ? | action    |
    |------|-----|------|-----|----------------|-------|--------|-----------|
    |  1   |  1  |  11  |  6  | 1 + 1 + 2 + 2  |   6   |  yes   | high = 6  |
    |  2   |  1  |  6   |  3  | 1 + 2 + 3 + 4  |  10   |  no    | low  = 4  |
    |  3   |  4  |  6   |  5  | 1 + 2 + 2 + 3  |   8   |  yes   | high = 5  |
    |  4   |  4  |  5   |  4  | 1 + 2 + 2 + 3  |   8   |  yes   | high = 4  |

      Loop ends at low = high = 4.  RETURN 4

    ======================================================================
    Notes
    ======================================================================

    Change from the original: int count -> long long count.
      With up to 10^4 piles of up to 10^9 bananas, speed 1 gives
      hours = sum(arr), up to 10^13 - far past INT_MAX (~2.1 * 10^9). The
      int overflowed, could wrap to a small or negative number, pass the
      count <= h test, and send the search the wrong way. The unused
      variable n was also removed.

    Integer ceil instead of the double:
      ceil((double)i / mid) is exact for these sizes, but the usual
      integer-only form avoids floating point entirely:
          count += (i + mid - 1) / mid;
      (i + mid - 1 stays below 2 * 10^9, so it fits in an int.)

    Optional early exit: break out of the for loop once count > h. The
      answer for this mid is already "too slow", and it also keeps count
      small - but with long long it is purely a speed-up.

    Why high = mid and not high = mid - 1:
      mid passed the test, so it might be the answer itself. Dropping it
      would lose it. low < high guarantees mid < high, so the range still
      shrinks every iteration.

    Edge cases:
      - h == n: every pile must go in one hour -> max(arr) (example 2).
      - single pile {p}, h hours: the answer is ceil(p / h).
*/
