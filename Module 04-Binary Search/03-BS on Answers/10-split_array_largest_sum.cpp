#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: (LeetCode 410) Given an integer array arr (0 <= arr[i] <= 10^6) and an
// integer k (1 <= k <= min(50, n)), split arr into k NON-EMPTY CONTIGUOUS
// subarrays so that the LARGEST subarray sum is as SMALL as possible, and
// return that minimised largest sum.
//
// Example:
// arr = {7, 2, 5, 10, 8},  k = 2   -> 18   ({7, 2, 5} and {10, 8})
// arr = {1, 2, 3, 4, 5},   k = 2   -> 9    ({1, 2, 3} and {4, 5})
// arr = {1, 4, 4},         k = 3   -> 4    ({1} {4} {4})

/*
    Approach: Binary search on the answer (the largest subarray sum)

    - For a limit mid, count how many subarrays are needed greedily: keep
      adding elements to the current subarray while its sum stays <= mid,
      and start a new subarray when the next element does not fit. A bigger
      limit never needs more subarrays, so the predicate
      "subarrays needed <= k" is false and then true. We want the FIRST true.
    - The answer lies in max(arr)..sum(arr):
        below max(arr) the largest element fits in no subarray;
        at sum(arr) the whole array is one subarray.
    - Needing FEWER than k subarrays is still fine: any subarray of two or
      more elements can be cut further without raising the largest sum, and
      k <= n guarantees there are enough elements to reach exactly k.
    - Probe mid:
        subarrays <= k  ->  mid works, a smaller limit might too: high = mid.
        subarrays >  k  ->  mid is too small: low = mid + 1.
      This is exactly 09-allocate_minimum_pages.cpp with subarrays in place
      of students (and no -1 case, since k <= n is guaranteed here).

    Algorithm Steps
    ----------------
    1. Set low = max(arr), high = sum(arr).
    2. While low < high, compute mid = low + (high - low) / 2.
    3. Simulate: subarrays = 1, remaining = mid. For each num, if it fits,
       remaining -= num; else subarrays++, remaining = mid - num.
    4. If subarrays <= k, high = mid.
    5. Else low = mid + 1.
    6. Return low (== high).

    Time Complexity: O(n * log(sum - max)) - one O(n) simulation per probe.
    Space Complexity: O(1)
*/
int splitArray(vector<int>& arr, int k) {
  long long low = *max_element(arr.begin(), arr.end());
  long long high = accumulate(arr.begin(), arr.end(), 0LL);

  while (low < high) {
    long long mid = low + (high - low) / 2;  // to prevent overflow

    int subarrays = 1;          // subarrays used so far
    long long remaining = mid;  // room left in the current subarray

    for (int num : arr) {
      if (num <= remaining) {
        remaining -= num;  // num fits in the current subarray
      } else {
        subarrays++;  // start a new subarray with num in it
        remaining = mid - num;
      }
    }

    if (subarrays <= k) {
      high = mid;  // mid is enough, try a smaller largest sum
    } else {
      low = mid + 1;  // limit too small, needs too many subarrays
    }
  }

  return low;  // smallest largest-sum achievable with k subarrays
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{7, 2, 5, 10, 8}, 2}, {{1, 2, 3, 4, 5}, 2}, {{1, 4, 4}, 3}};

  for (auto& [arr, k] : tests) {
    cout << "Array: ";
    for (int v : arr) cout << v << " ";
    cout << " k = " << k << endl;
    cout << "  minimised largest sum: " << splitArray(arr, k) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of splitArray: arr = {7, 2, 5, 10, 8}, k = 2
    (answer = 18)
    ======================================================================

    Initial state:   low = max = 10, high = 7 + 2 + 5 + 10 + 8 = 32

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 10, high = 32
      mid          10 + (32 - 10) / 2 = 21
      splitting    part 1: 7, 2, 5            (14, 7 left)
                   10 > 7   ->  part 2: 10, 8  (18, 3 left)
      result       2 parts <= 2  ->  fits
      update       high = 21

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 10, high = 21
      mid          10 + (21 - 10) / 2 = 15
      splitting    part 1: 7, 2, 5            (14, 1 left)
                   10 > 1   ->  part 2: 10     (5 left)
                   8 > 5    ->  part 3: 8
      result       3 parts > 2  ->  too small
      update       low = 16

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 16, high = 21
      mid          16 + (21 - 16) / 2 = 18
      splitting    part 1: 7, 2, 5            (14, 4 left)
                   10 > 4   ->  part 2: 10, 8  (18, 0 left)
      result       2 parts  ->  fits
      update       high = 18

    ----------------------------------------------------------------------
    Iteration 4
      bounds       low = 16, high = 18
      mid          16 + (18 - 16) / 2 = 17
      splitting    part 1: 7, 2, 5            (14, 3 left)
                   10 > 3   ->  part 2: 10     (7 left)
                   8 > 7    ->  part 3: 8
      result       3 parts  ->  too small
      update       low = 18

    ----------------------------------------------------------------------
    Loop ends with low = high = 18.  RETURN 18

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | split                 | parts | action    |
    |------|-----|------|-----|-----------------------|-------|-----------|
    |  1   |  10 |  32  |  21 | {7,2,5} {10,8}        |   2   | high = 21 |
    |  2   |  10 |  21  |  15 | {7,2,5} {10} {8}      |   3   | low  = 16 |
    |  3   |  16 |  21  |  18 | {7,2,5} {10,8}        |   2   | high = 18 |
    |  4   |  16 |  18  |  17 | {7,2,5} {10} {8}      |   3   | low  = 18 |

    ======================================================================
    Notes
    ======================================================================

    Why return low:
      every limit >= high has been proven to work and every limit < low has
      been proven not to. The loop runs while low < high, so at exit
      low == high is the smallest limit that works.

    Why low starts at max(arr):
      since mid >= every element, an element that does not fit in the
      current subarray always fits in a fresh one, so remaining = mid - num
      never goes negative.

    Same problem, different names:
      09-allocate_minimum_pages.cpp (books -> students),
      11-painters_partition.cpp (boards -> painters) and
      06-ship_within_d_days.cpp
      (packages -> days) are all this search.

    Overflow: with n <= 1000 and arr[i] <= 10^6 the sum stays under 10^9,
      so int would fit here; long long keeps it safe for larger inputs.

    Edge cases:
      - k == n: every element is its own subarray -> max(arr) (example 3).
      - k == 1: the whole array is one subarray -> sum(arr).
      - zeros in arr: they fit in any subarray (0 <= remaining), so they never
        force an extra split.
*/
