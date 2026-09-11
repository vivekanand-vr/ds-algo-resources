#include <bits/stdc++.h>
using namespace std;

// Q: Given a binary array arr and an integer goal k, return the number of
//    non-empty SUBARRAYS (contiguous) whose sum is exactly k.
//
// Example:
// arr = [1,0,1,0,1], k = 2 -> 4   (the subarrays [1,0,1] at 0..2,
//                                  [1,0,1,0] at 0..3, [0,1,0,1] at 1..4,
//                                  [1,0,1] at 2..4)
// arr = [0,0,0,0,0], k = 0 -> 15  (every one of the 5*6/2 subarrays sums 0)
// arr = [1,1,1],     k = 2 -> 2   ([1,1] at 0..1 and [1,1] at 1..2)
//
// NOTE: this is solution II of two. See 05-binary_subarray_with_sum.cpp for
//       the prefix-sum + hash map version, which handles negative numbers
//       too but costs O(n) extra space.

/*
    Approach II: Two pointers, counting "at most k" twice

    - A plain sliding window CANNOT count subarrays summing to exactly k.
      The window condition has to be monotone - "sum <= k" is (shrinking
      always helps), but "sum == k" is not, so there is no single window
      that sweeps every exact-sum subarray. The fix is a classic trick:

          exactly(k)  =  atMost(k)  -  atMost(k - 1)

      every subarray with sum <= k, minus every subarray with sum <= k-1,
      leaves precisely those with sum == k.
    - solve(k) counts subarrays with sum <= k in one linear pass. Grow the
      window by arr[r]; while the sum exceeds k, shrink from the left. Then
      add (r - l + 1) to the count: that is the number of valid subarrays
      ENDING AT r, because if arr[l..r] has sum <= k then so does every
      suffix of it - arr[l+1..r], arr[l+2..r], ... , arr[r..r]. Counting by
      right endpoint this way visits every subarray exactly once.
    - This shrink-until-valid logic depends on the sum only ever DECREASING
      as the left edge advances, i.e. on the values being non-negative.
      With a negative number present the window is not monotone and this
      whole approach silently breaks - use solution I there.
    - The `if (k < 0) return 0` guard is essential, not cosmetic. When the
      caller asks for k = 0 the second call is solve(-1); no subarray of a
      non-negative array can sum below zero, and without the guard the
      shrink loop would run the left pointer past r on the first element.

    Algorithm Steps
    ----------------
    solve(arr, k)  -> number of subarrays with sum <= k
      1. If k < 0, return 0 (impossible for a non-negative array).
      2. Set l = 0, r = 0, sum = 0, cnt = 0.
      3. While r < n:
           a. sum += arr[r]                     (grow the window)
           b. While sum > k: sum -= arr[l], l++ (shrink from the left)
           c. cnt += (r - l + 1)                (all subarrays ending at r)
           d. r++.
      4. Return cnt.

    numSubarraysWithSum(arr, k)
      1. Return solve(arr, k) - solve(arr, k - 1).

    Time Complexity: O(n) - two linear passes; in each one l and r only
                      move forward, so the nested loop is amortised O(1).
    Space Complexity: O(1) - no map, just counters and pointers.
*/
int solve(vector<int>& arr, int k) {
  if (k < 0) return 0;                // no non-negative subarray sums below 0

  int n = arr.size();
  int l = 0, r = 0, sum = 0, cnt = 0;

  while (r < n) {
    sum += arr[r];                    // grow: arr[r] joins the window

    while (sum > k) {                 // window is over the cap
      sum -= arr[l];
      l++;                            // shrink from the left
    }

    cnt += (r - l + 1);               // every subarray ending at r is valid
    r++;
  }

  return cnt;
}

int numSubarraysWithSum(vector<int>& arr, int k) {
  return solve(arr, k) - solve(arr, k - 1);   // exactly(k) = atMost gap
}

int main() {
  vector<pair<vector<int>, int>> tests = {
      {{1, 0, 1, 0, 1}, 2}, {{0, 0, 0, 0, 0}, 0}, {{1, 1, 1}, 2}};

  for (auto& t : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < t.first.size(); ++i) {
      cout << t.first[i] << (i + 1 < t.first.size() ? "," : "");
    }
    cout << "], k = " << t.second << endl;
    cout << "Output: " << numSubarraysWithSum(t.first, t.second) << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [1,0,1,0,1], k = 2   (n = 5, answer = 4)
    ==========================================================================

      index:  0  1  2  3  4
      value:  1  0  1  0  1

    The answer is computed as  solve(2) - solve(1)  =  14 - 10  =  4.
    Both passes are traced below.

    Tracked state in solve():
      sum   - the sum of the current window arr[l..r]
      l, r  - the window bounds, width = r - l + 1
      cnt   - subarrays with sum <= k counted so far

    The key line is  cnt += (r - l + 1).  Read it as: "arr[l..r] is the
    LONGEST valid subarray ending at r, and every shorter suffix of it is
    valid too, so there are exactly (r - l + 1) of them ending here."

    ==========================================================================
    PASS 1: solve(arr, 2)  - count subarrays with sum <= 2
    ==========================================================================

    Initial state: l = 0, r = 0, sum = 0, cnt = 0

    --------------------------------------------------------------------------
    r = 0, arr[r] = 1
      grow         sum = 0 + 1 = 1
      shrink?      1 <= 2 -> no
      window       arr[0..0] = [1]
      count        cnt += (0 - 0 + 1) = 1      -> cnt = 1
                   the subarrays ending at 0: [1]
    --------------------------------------------------------------------------
    r = 1, arr[r] = 0
      grow         sum = 1 + 0 = 1
      shrink?      1 <= 2 -> no
      window       arr[0..1] = [1,0]
      count        cnt += 2                    -> cnt = 3
                   ending at 1: [1,0], [0]
    --------------------------------------------------------------------------
    r = 2, arr[r] = 1
      grow         sum = 1 + 1 = 2
      shrink?      2 <= 2 -> no, exactly at the cap
      window       arr[0..2] = [1,0,1]
      count        cnt += 3                    -> cnt = 6
                   ending at 2: [1,0,1], [0,1], [1]
    --------------------------------------------------------------------------
    r = 3, arr[r] = 0
      grow         sum = 2 + 0 = 2
      shrink?      2 <= 2 -> no
      window       arr[0..3] = [1,0,1,0]
      count        cnt += 4                    -> cnt = 10
                   ending at 3: [1,0,1,0], [0,1,0], [1,0], [0]
    --------------------------------------------------------------------------
    r = 4, arr[r] = 1
      grow         sum = 2 + 1 = 3             <-- over the cap
      shrink       step 1: sum -= arr[0] = 1 -> sum = 2, l -> 1
                           re-check: 2 <= 2 -> stop
      window       arr[1..4] = [0,1,0,1], sum = 2
      count        cnt += (4 - 1 + 1) = 4      -> cnt = 14
                   ending at 4: [0,1,0,1], [1,0,1], [0,1], [1]
                   note arr[0..4] (sum 3) was correctly NOT counted
    --------------------------------------------------------------------------
    solve(2) = 14

    ==========================================================================
    PASS 2: solve(arr, 1)  - count subarrays with sum <= 1
    ==========================================================================

    Initial state: l = 0, r = 0, sum = 0, cnt = 0

    --------------------------------------------------------------------------
    r = 0, arr[r] = 1
      grow         sum = 1,  1 <= 1 -> no shrink
      window       arr[0..0]
      count        cnt += 1                    -> cnt = 1
    --------------------------------------------------------------------------
    r = 1, arr[r] = 0
      grow         sum = 1,  no shrink
      window       arr[0..1] = [1,0]
      count        cnt += 2                    -> cnt = 3
    --------------------------------------------------------------------------
    r = 2, arr[r] = 1
      grow         sum = 2                     <-- over the cap
      shrink       step 1: sum -= arr[0] = 1 -> sum = 1, l -> 1, stop
      window       arr[1..2] = [0,1], sum = 1
      count        cnt += (2 - 1 + 1) = 2      -> cnt = 5
    --------------------------------------------------------------------------
    r = 3, arr[r] = 0
      grow         sum = 1,  no shrink
      window       arr[1..3] = [0,1,0]
      count        cnt += (3 - 1 + 1) = 3      -> cnt = 8
    --------------------------------------------------------------------------
    r = 4, arr[r] = 1
      grow         sum = 2                     <-- over the cap
      shrink       step 1: sum -= arr[1] = 0 -> sum = 2, l -> 2
                           re-check: 2 > 1, keep going
                   step 2: sum -= arr[2] = 1 -> sum = 1, l -> 3
                           re-check: 1 <= 1 -> stop
                   (the zero at index 1 had to be dropped even though it
                    contributed nothing - the window cannot skip over it)
      window       arr[3..4] = [0,1], sum = 1
      count        cnt += (4 - 3 + 1) = 2      -> cnt = 10
    --------------------------------------------------------------------------
    solve(1) = 10

    ==========================================================================
    RESULT:  solve(2) - solve(1)  =  14 - 10  =  4
    ==========================================================================

    Summary table

    | r | arr[r] | -------- solve(2) -------- | -------- solve(1) -------- |
    |   |        | sum | l | added | cnt      | sum | l | added | cnt      |
    |---|--------|-----|---|-------|----------|-----|---|-------|----------|
    | 0 |   1    |  1  | 0 |   1   |    1     |  1  | 0 |   1   |    1     |
    | 1 |   0    |  1  | 0 |   2   |    3     |  1  | 0 |   2   |    3     |
    | 2 |   1    |  2  | 0 |   3   |    6     |  1  | 1 |   2   |    5     |
    | 3 |   0    |  2  | 0 |   4   |   10     |  1  | 1 |   3   |    8     |
    | 4 |   1    |  2  | 1 |   4   |   14     |  1  | 3 |   2   |   10     |

    The subtraction cancels every subarray with sum 0 or 1 (counted in both
    passes) and leaves the four with sum exactly 2:
        [1,0,1] 0..2, [1,0,1,0] 0..3, [0,1,0,1] 1..4, [1,0,1] 2..4

    ==========================================================================
    Notes
    ==========================================================================

    Why the k < 0 guard is load-bearing:
      for k = 0 the second call is solve(arr, -1). Any subarray of a
      non-negative array has sum >= 0, so the true answer is 0 - but
      without the guard, the first element would send sum > -1, the shrink
      loop would run l past r, and at l = r+1 the count (r - l + 1) = 0
      while `sum -= arr[l]` would have already read out of bounds on the
      next iteration. The guard returns 0 immediately instead.

    Why a plain "sum == k" window does not work:
      with arr = [1,0,0,1] and k = 1 there are several valid subarrays of
      different lengths ending at the same index ([1,0,0] and [1,0] and
      [1] all sum to 1 ending at index 2). A single window has one left
      edge, so it can report only one of them. atMost sidesteps this by
      counting ALL suffixes of the maximal window at once.

    Choosing between the two solutions:
      - II (this file): O(1) space, two clean passes, but REQUIRES
        non-negative values. Best for binary / non-negative arrays, and the
        same atMost trick powers 06-nice_subarrays.cpp and "subarrays with
        exactly k distinct".
      - I (05-binary_subarray_with_sum.cpp): O(n) space, but handles
        negatives and is the general LeetCode-560 template.
*/
