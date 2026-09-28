#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an array of positive integers nums and an integer threshold, pick
// a positive integer divisor, divide every element by it rounding UP, and sum
// the results. Return the SMALLEST divisor whose sum is <= threshold.
// (nums.length <= threshold is given, so an answer always exists.)
//
// Example:
// nums = {1, 2, 5, 9},             threshold = 6         -> 5
// nums = {44, 22, 33, 11, 1},      threshold = 5         -> 44
// nums = {2, 3, 5, 7, 11},         threshold = 11        -> 3
// nums = {21212, 10101, 12121},    threshold = 1000000   -> 1

/*
    Approach: Binary search on the answer (the divisor)

    - For a divisor d, sum(d) = sum of ceil(nums[i] / d). A bigger divisor
      never makes any quotient bigger, so sum(d) is non-increasing in d and
      the predicate "sum(d) <= threshold" is false for small d and true from
      the answer onward. We want the FIRST true.
    - The answer lies in 1..max(nums): at d = max(nums) every quotient is 1,
      the sum is n, and n <= threshold. A larger divisor cannot go lower.
    - This is the same search as 03-koko_eating_bananas.cpp, with the
      divisor in place of the eating speed and threshold in place of h.
    - Probe mid:
        sum(mid) <= threshold  ->  mid works, a smaller one might too:
                                   high = mid (keep mid, it may be the answer).
        sum(mid) >  threshold  ->  mid is too small: low = mid + 1.

    Algorithm Steps
    ----------------
    1. Set low = 1, high = max(nums).
    2. While low < high, compute mid = low + (high - low) / 2.
    3. Compute sum = sum of ceil(nums[i] / mid).
    4. If sum <= threshold, high = mid.
    5. Else low = mid + 1.
    6. Return low.

    Time Complexity: O(n * log(max(nums))) - log(max) probes, each summing
                      over all n elements.
    Space Complexity: O(1)
*/
int smallestDivisor(vector<int>& nums, int threshold) {
  int low = 1, high = *max_element(begin(nums), end(nums));

  while (low < high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    long long sum = 0;  // sum of rounded-up quotients for divisor mid
    for (int num : nums) {
      sum += ceil((double)num / mid);
    }

    if (sum <= threshold) {
      high = mid;  // mid is big enough, try a smaller divisor
    } else {
      low = mid + 1;  // mid is too small, the sum is over threshold
    }
  }

  return low;
}

int main() {
  vector<pair<vector<int>, int>> tests = {
      {{1, 2, 5, 9}, 6}, {{44, 22, 33, 11, 1}, 5}, {{2, 3, 5, 7, 11}, 11}, {{21212, 10101, 12121}, 1000000}};

  for (auto& [nums, threshold] : tests) {
    cout << "Array: ";
    for (int v : nums) cout << v << " ";
    cout << " threshold = " << threshold << endl;
    cout << "  smallest divisor: " << smallestDivisor(nums, threshold) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of smallestDivisor: nums = {1, 2, 5, 9}, threshold = 6
    (answer = 5)
    ======================================================================

      divisor d:      1   2   3   4   5   6   7   8   9
      sum(d):        17  10   7   7   5   5   5   5   4
      sum <= 6:       F   F   F   F   T   T   T   T   T
                                      ^
                                      first T -> answer

    Initial state: low = 1, high = max(nums) = 9

    | iter | low | high | mid | ceil per element | sum | <= 6 ? | action    |
    |------|-----|------|-----|------------------|-----|--------|-----------|
    |  1   |  1  |  9   |  5  | 1 + 1 + 1 + 2    |  5  |  yes   | high = 5  |
    |  2   |  1  |  5   |  3  | 1 + 1 + 2 + 3    |  7  |  no    | low  = 4  |
    |  3   |  4  |  5   |  4  | 1 + 1 + 2 + 3    |  7  |  no    | low  = 5  |

      Loop ends at low = high = 5.  RETURN 5

    ======================================================================
    Notes
    ======================================================================

    Changes from the original:
      - int sum -> long long sum. With up to 5 * 10^4 elements of up to
        10^6, divisor 1 gives a sum of up to 5 * 10^10, past INT_MAX
        (~2.1 * 10^9). The wrapped int could look small, pass the
        sum <= threshold test, and send the search the wrong way.
      - Renamed arr -> nums, t -> threshold and the loop variable i -> num,
        so the names match the problem statement and i no longer looks
        like an index.

    Integer ceil instead of the double:
      ceil((double)num / mid) is exact for these sizes, but the
      integer-only form avoids floating point entirely:
          sum += (num + mid - 1) / mid;

    Optional early exit: break out of the for loop once sum > threshold -
      mid is already known to be too small.

    Edge cases:
      - threshold == n: every quotient must be 1 -> max(nums) (example 2).
      - threshold >= sum(nums): divisor 1 already works -> 1 (example 4).
      - single element {x}: the answer is ceil(x / threshold).
*/
