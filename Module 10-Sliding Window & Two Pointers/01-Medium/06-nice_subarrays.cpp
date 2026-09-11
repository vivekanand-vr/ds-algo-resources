#include <bits/stdc++.h>
using namespace std;

// Q: An array is called "nice" if it contains exactly k odd numbers. Given
//    an integer array arr and an integer k, return the number of nice
//    SUBARRAYS (contiguous).
//
// Example:
// arr = [1,1,2,1,1], k = 3 -> 2   ([1,1,2,1] at 0..3 and [1,2,1,1] at 1..4)
// arr = [2,4,6],     k = 1 -> 0   (no odd numbers at all)
// arr = [2,2,2,1,2,2,1,2,2,2], k = 2 -> 16

/*
    Approach: Two pointers, counting "at most k" twice

    - Replace every element mentally by 1 if it is odd and 0 if it is even.
      "Exactly k odd numbers" then reads as "sum exactly k" over a binary
      array - this problem IS 05-binary_subarray_with_sum in disguise, and
      the code is the same shape with `arr[r] & 1` in place of `arr[r]`.
    - A single window cannot count exact matches, because "odd count == k"
      is not a monotone condition - shrinking the window does not reliably
      move you towards satisfying it, so no one window sweeps every valid
      subarray. The standard fix:

          exactly(k)  =  atMost(k)  -  atMost(k - 1)

      subarrays with at most k odds, minus those with at most k-1 odds,
      leaves precisely those with exactly k.
    - solve(k) counts subarrays with at most k odds in one linear pass.
      Grow by arr[r]; while the odd count exceeds k, shrink from the left.
      Then add (r - l + 1): arr[l..r] is the longest valid subarray ending
      at r, and every suffix of it - arr[l+1..r], ... , arr[r..r] - has no
      more odds than it does, so all of them are valid too. Counting by
      right endpoint visits every subarray exactly once.
    - `arr[r] & 1` tests the low bit, which is 1 exactly for odd numbers.
      Prefer it over `% 2` when negatives are possible: in C++ `-3 % 2` is
      -1, not 1, so a naive `% 2 == 1` test would miss negative odds while
      the bit test handles them correctly on two's-complement hardware.
    - The `if (k < 0) return 0` guard covers the k = 0 call, where the
      second pass is solve(-1): no subarray can contain fewer than zero
      odd numbers, and without the guard the shrink loop would run the
      left pointer off the end.

    Algorithm Steps
    ----------------
    solve(arr, k)  -> number of subarrays with AT MOST k odd numbers
      1. If k < 0, return 0 (impossible - a count is never negative).
      2. Set l = 0, r = 0, odd = 0, c = 0.
      3. While r < n:
           a. If arr[r] is odd, odd++          (grow the window)
           b. While odd > k:
                - if arr[l] is odd, odd--
                - l++                          (shrink from the left)
           c. c += (r - l + 1)                 (all subarrays ending at r)
           d. r++.
      4. Return c.

    numberOfSubarrays(arr, k)
      1. Return solve(arr, k) - solve(arr, k - 1).

    Time Complexity: O(n) - two linear passes; within each, l and r only
                      move forward, so the nested loop is amortised O(1).
    Space Complexity: O(1) - one counter and two pointers, no map.
*/
int solve(vector<int>& arr, int k) {
  if (k < 0) return 0;              // a subarray can't hold < 0 odd numbers

  int n = arr.size();
  int l = 0, r = 0, c = 0, odd = 0;

  while (r < n) {
    if (arr[r] & 1) odd++;          // grow: arr[r] joins the window

    while (odd > k) {               // too many odds inside
      if (arr[l] & 1) {
        odd--;                      // the element leaving was odd
      }
      l++;                          // shrink from the left
    }

    c += (r - l + 1);               // every subarray ending at r is valid
    r++;
  }

  return c;
}

int numberOfSubarrays(vector<int>& arr, int k) {
  return solve(arr, k) - solve(arr, k - 1);   // exactly(k) = atMost gap
}

int main() {
  vector<pair<vector<int>, int>> tests = {
      {{1, 1, 2, 1, 1}, 3},
      {{2, 4, 6}, 1},
      {{2, 2, 2, 1, 2, 2, 1, 2, 2, 2}, 2}};

  for (auto& t : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < t.first.size(); ++i) {
      cout << t.first[i] << (i + 1 < t.first.size() ? "," : "");
    }
    cout << "], k = " << t.second << endl;
    cout << "Output: " << numberOfSubarrays(t.first, t.second) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [1,1,2,1,1], k = 3   (n = 5, answer = 2)
    ==========================================================================

      index:   0  1  2  3  4
      value:   1  1  2  1  1
      odd?:    Y  Y  n  Y  Y     <- as a binary array: 1 1 0 1 1

    The answer is computed as  solve(3) - solve(2)  =  14 - 12  =  2.
    Both passes are traced below.

    Tracked state in solve():
      odd   - how many ODD numbers are inside the current window arr[l..r]
      l, r  - the window bounds, width = r - l + 1
      c     - subarrays with at most k odds counted so far

    The key line is  c += (r - l + 1).  Read it as: "arr[l..r] is the
    LONGEST subarray ending at r with at most k odds, and every shorter
    suffix of it has at most as many odds, so there are exactly
    (r - l + 1) valid subarrays ending here."

    ==========================================================================
    PASS 1: solve(arr, 3)  - subarrays with at most 3 odd numbers
    ==========================================================================

    Initial state: l = 0, r = 0, odd = 0, c = 0

    --------------------------------------------------------------------------
    r = 0, arr[r] = 1 (odd)
      grow         odd = 1
      shrink?      1 <= 3 -> no
      window       arr[0..0] = [1]
      count        c += (0 - 0 + 1) = 1     -> c = 1
                   ending at 0: [1]
    --------------------------------------------------------------------------
    r = 1, arr[r] = 1 (odd)
      grow         odd = 2
      shrink?      2 <= 3 -> no
      window       arr[0..1] = [1,1]
      count        c += 2                   -> c = 3
                   ending at 1: [1,1], [1]
    --------------------------------------------------------------------------
    r = 2, arr[r] = 2 (even)
      grow         odd stays 2              (evens are free - they widen the
                                             window without costing budget)
      shrink?      2 <= 3 -> no
      window       arr[0..2] = [1,1,2]
      count        c += 3                   -> c = 6
                   ending at 2: [1,1,2], [1,2], [2]
    --------------------------------------------------------------------------
    r = 3, arr[r] = 1 (odd)
      grow         odd = 3
      shrink?      3 <= 3 -> no, exactly at the cap
      window       arr[0..3] = [1,1,2,1]
      count        c += 4                   -> c = 10
                   ending at 3: [1,1,2,1], [1,2,1], [2,1], [1]
    --------------------------------------------------------------------------
    r = 4, arr[r] = 1 (odd)
      grow         odd = 4                  <-- over the cap
      shrink       step 1: drop arr[l=0] = 1, odd -> 3, l -> 1
                           re-check: 3 <= 3 -> stop
      window       arr[1..4] = [1,2,1,1], odd = 3
      count        c += (4 - 1 + 1) = 4     -> c = 14
                   ending at 4: [1,2,1,1], [2,1,1], [1,1], [1]
                   arr[0..4] has 4 odds and was correctly NOT counted
    --------------------------------------------------------------------------
    solve(3) = 14

    ==========================================================================
    PASS 2: solve(arr, 2)  - subarrays with at most 2 odd numbers
    ==========================================================================

    Initial state: l = 0, r = 0, odd = 0, c = 0

    --------------------------------------------------------------------------
    r = 0, arr[r] = 1
      grow         odd = 1,  1 <= 2 -> no shrink
      count        c += 1                   -> c = 1
    --------------------------------------------------------------------------
    r = 1, arr[r] = 1
      grow         odd = 2,  2 <= 2 -> no shrink
      window       arr[0..1]
      count        c += 2                   -> c = 3
    --------------------------------------------------------------------------
    r = 2, arr[r] = 2 (even)
      grow         odd stays 2, no shrink
      window       arr[0..2] = [1,1,2]
      count        c += 3                   -> c = 6
    --------------------------------------------------------------------------
    r = 3, arr[r] = 1
      grow         odd = 3                  <-- over the cap
      shrink       step 1: drop arr[l=0] = 1, odd -> 2, l -> 1, stop
      window       arr[1..3] = [1,2,1], odd = 2
      count        c += (3 - 1 + 1) = 3     -> c = 9
    --------------------------------------------------------------------------
    r = 4, arr[r] = 1
      grow         odd = 3                  <-- over the cap
      shrink       step 1: drop arr[l=1] = 1, odd -> 2, l -> 2, stop
      window       arr[2..4] = [2,1,1], odd = 2
      count        c += (4 - 2 + 1) = 3     -> c = 12
    --------------------------------------------------------------------------
    solve(2) = 12

    ==========================================================================
    RESULT:  solve(3) - solve(2)  =  14 - 12  =  2
    ==========================================================================

    Summary table

    | r | arr[r] | -------- solve(3) -------- | -------- solve(2) -------- |
    |   |        | odd | l | added |   c      | odd | l | added |   c      |
    |---|--------|-----|---|-------|----------|-----|---|-------|----------|
    | 0 |   1    |  1  | 0 |   1   |    1     |  1  | 0 |   1   |    1     |
    | 1 |   1    |  2  | 0 |   2   |    3     |  2  | 0 |   2   |    3     |
    | 2 |   2    |  2  | 0 |   3   |    6     |  2  | 0 |   3   |    6     |
    | 3 |   1    |  3  | 0 |   4   |   10     |  2  | 1 |   3   |    9     |
    | 4 |   1    |  3  | 1 |   4   |   14     |  2  | 2 |   3   |   12     |

    The subtraction cancels every subarray holding 0, 1 or 2 odds (counted
    in both passes) and leaves the two nice ones:
        [1,1,2,1] indices 0..3      [1,2,1,1] indices 1..4
    both containing exactly three odd numbers.

    ==========================================================================
    Notes
    ==========================================================================

    Why the k < 0 guard is load-bearing:
      for k = 0 the second call is solve(arr, -1). The true answer is 0 -
      no subarray holds a negative number of odds - but without the guard
      the condition odd > -1 is true even when odd = 0, so the shrink loop
      would never stop and l would run past the end of the array. The
      guard returns 0 immediately instead.

    Why evens are "free":
      an even element raises the window width without raising odd, so it
      always widens the count at no cost. That is visible at r = 2 in both
      passes, where c jumped by 3 with no shrink at all.

    The same trick elsewhere:
      atMost(k) - atMost(k-1) is the standard recipe whenever an "exactly
      k" count is wanted and the underlying condition is monotone. It
      appears in 05-binary_subarray_with_sum_II.cpp (sum exactly k) and in
      "subarrays with exactly k distinct integers", where atMost counts
      distinct values with a map instead of odds with a counter.
*/
