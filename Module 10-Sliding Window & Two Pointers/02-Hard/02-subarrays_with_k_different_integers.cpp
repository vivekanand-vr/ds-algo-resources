#include <bits/stdc++.h>
using namespace std;

// Q: Given an integer array nums and an integer k, return the number of
//    "good" subarrays of nums - a good subarray is one whose number of
//    distinct integers is EXACTLY k.
//
// Example:
// nums = {1, 2, 1, 2, 3}, k = 2 -> 7
//        {1,2}, {2,1}, {1,2}, {1,2,1}, {2,1,2}, {1,2,1,2}, {2,3}
// nums = {1, 2, 1, 3, 4}, k = 3 -> 3
//        {1,2,1,3}, {2,1,3}, {1,3,4}

/*
    Approach: exactly(k) = atMost(k) - atMost(k - 1)

    - Counting subarrays with EXACTLY k distinct values in one sliding
      window does not work: the window cannot decide how far left to
      start, because both ends have to satisfy an equality rather than
      an inequality.
    - Counting subarrays with AT MOST k distinct values, however, is a
      textbook window. Keep [l, r] within budget by shrinking whenever
      the distinct count exceeds k. Then for the current r, EVERY
      subarray ending at r and starting at any index in [l, r] has at
      most k distinct values - and there are exactly (r - l + 1) of
      them. Summing that over all r counts them all, each exactly once,
      because each subarray has a unique right endpoint.
    - Finally, a subarray with at most k distinct values either has
      exactly k, or at most k - 1. So subtracting the two counts leaves
      precisely the subarrays with exactly k:
          exactly(k) = atMost(k) - atMost(k - 1)
    - The helper is called `atMostK` rather than `solve` to make that
      identity readable at the call site.

    Algorithm Steps
    ----------------
    atMostK(arr, k):
      1. Initialize l = 0, r = 0, count = 0 and an empty count map.
      2. While r < n:
           a. Increment the count of arr[r] (grow the window).
           b. While the number of distinct values exceeds k:
                - decrement count[arr[l]], erase the key if it hits 0
                - l++ (shrink from the left)
           c. count += (r - l + 1) - all subarrays ending at r that
              start inside the window are within budget.
           d. r++.
      3. Return count.

    subarraysWithKDistinct(nums, k):
      4. Return atMostK(nums, k) - atMostK(nums, k - 1).

    Time Complexity: O(n log k) - two linear window passes, each map
                      operation O(log k). unordered_map makes it O(n).
    Space Complexity: O(k) - the map never holds more than k + 1 keys
*/
int atMostK(vector<int>& arr, int k) {
  int n = arr.size();
  int l = 0, r = 0, c = 0;
  map<int, int> mp;

  while (r < n) {
    mp[arr[r]]++;                        // grow the window to include arr[r]

    while ((int)mp.size() > k) {         // over budget -> shrink from the left
      mp[arr[l]]--;
      if (mp[arr[l]] == 0) {
        mp.erase(arr[l]);                // drop keys at zero so size() = distinct count
      }
      l++;
    }

    c += (r - l + 1);                    // every start in [l, r] is within budget
    r++;
  }

  return c;
}

int subarraysWithKDistinct(vector<int>& nums, int k) {
  return atMostK(nums, k) - atMostK(nums, k - 1);
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{1, 2, 1, 2, 3}, 2}, {{1, 2, 1, 3, 4}, 3}};

  for (auto& [nums, k] : tests) {
    vector<int> arr = nums;
    cout << "nums = [";
    for (size_t i = 0; i < arr.size(); i++) cout << arr[i] << (i + 1 < arr.size() ? ", " : "");
    cout << "], k = " << k << " -> " << subarraysWithKDistinct(arr, k) << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = {1,2,1,2,3}, k = 2   (n = 5, answer = 7)
    ==========================================================================

      index:   0   1   2   3   4
      value:   1   2   1   2   3

    Tracked state (inside atMostK):
      mp        - value -> how many times it occurs inside nums[l..r]
      mp.size() - the DISTINCT value count of the window (keys at zero are
                  erased, which is what keeps size() honest)
      l, r      - window bounds; window is nums[l..r], width = r - l + 1
      c         - running COUNT of qualifying subarrays, not a width

    The identity being computed:
      exactly(2) = atMostK(nums, 2) - atMostK(nums, 1)

    Why `c += (r - l + 1)` counts rather than measures:
      after the shrink, l is the SMALLEST index with distinct(nums[l..r]) <= k.
      Pushing the start further right only shrinks the window, which can
      never raise the distinct count - so the valid starts for this r are
      exactly l, l+1, ..., r, which is (r - l + 1) of them. Every subarray
      has one unique right endpoint, so summing over r counts each once.

    ==========================================================================
    CALL 1 of 2:  atMostK(nums, k = 2)
    ==========================================================================
    Initial state: l = 0, r = 0, c = 0, mp = {}

    --------------------------------------------------------------------------
    r = 0, nums[r] = 1
      add          mp[1]++              -> mp = {1:1}
      size         1, not > 2           -> no shrink
      count        c += (0 - 0 + 1) = 1 -> c = 1
      advance      l = 0, r -> 1
                   the 1 subarray: [0..0]

    --------------------------------------------------------------------------
    r = 1, nums[r] = 2
      add          mp[2]++              -> mp = {1:1, 2:1}
      size         2, not > 2           -> no shrink (at budget, not over)
      count        c += (1 - 0 + 1) = 2 -> c = 3
      advance      l = 0, r -> 2
                   the 2 subarrays: [0..1], [1..1]

    --------------------------------------------------------------------------
    r = 2, nums[r] = 1
      add          mp[1]++              -> mp = {1:2, 2:1}
      size         2 (count grew, distinct did not) -> no shrink
      count        c += (2 - 0 + 1) = 3 -> c = 6
      advance      l = 0, r -> 3
                   the 3 subarrays: [0..2], [1..2], [2..2]

    --------------------------------------------------------------------------
    r = 3, nums[r] = 2
      add          mp[2]++              -> mp = {1:2, 2:2}
      size         2                    -> no shrink
      count        c += (3 - 0 + 1) = 4 -> c = 10
      advance      l = 0, r -> 4

    --------------------------------------------------------------------------
    r = 4, nums[r] = 3      <-- the only shrink, and it runs THREE times
      add          mp[3]++              -> mp = {1:2, 2:2, 3:1}
      size         3 > 2                -> shrink until back within budget

                   step 1: drop nums[l=0] = 1
                             mp[1]-- -> 1, not zero, key STAYS
                             mp = {1:1, 2:2, 3:1}, size still 3,  l -> 1
                   step 2: drop nums[l=1] = 2
                             mp[2]-- -> 1, not zero, key STAYS
                             mp = {1:1, 2:1, 3:1}, size still 3,  l -> 2
                   step 3: drop nums[l=2] = 1
                             mp[1]-- -> 0, hit zero -> ERASE key 1
                             mp = {2:1, 3:1}, size = 2,           l -> 3
                             2 is not > 2 -> stop

                   Note it took three drops to lose ONE distinct value: the
                   duplicates of 1 and 2 had to be consumed first. This is
                   exactly why the erase-at-zero rule matters - size() only
                   falls when the LAST copy of a value leaves the window.

                       1   2   1   2   3
                                   ^   ^
                                   l   r        mp = {2:1, 3:1}

      count        c += (4 - 3 + 1) = 2 -> c = 12
      advance      r -> 5   ->   r == n, loop ends

    atMostK(nums, 2) = 12

    ==========================================================================
    CALL 2 of 2:  atMostK(nums, k = 1)
    ==========================================================================
    With a budget of one distinct value, every new value forces the window
    down to just itself, so the width is 1 at every step and the count is
    simply n. Compressed, since the mechanism is identical:

    | r | nums[r] | mp after add | size | shrink (drops)   | l | width | c |
    |---|---------|--------------|------|------------------|---|-------|---|
    | 0 |    1    | {1:1}        |  1   | none             | 0 |   1   | 1 |
    | 1 |    2    | {1:1, 2:1}   |  2   | drop 1 -> erase  | 1 |   1   | 2 |
    | 2 |    1    | {2:1, 1:1}   |  2   | drop 2 -> erase  | 2 |   1   | 3 |
    | 3 |    2    | {1:1, 2:1}   |  2   | drop 1 -> erase  | 3 |   1   | 4 |
    | 4 |    3    | {2:1, 3:1}   |  2   | drop 2 -> erase  | 4 |   1   | 5 |

    atMostK(nums, 1) = 5      (the 5 single-element subarrays, nothing more)

    ==========================================================================
    RETURN 12 - 5 = 7
    ==========================================================================

    The seven subarrays with exactly 2 distinct values:
      [1,2]  [2,1]  [1,2]  [1,2,1]  [2,1,2]  [1,2,1,2]  [2,3]

    Summary table for the k = 2 call
    | r | nums[r] | mp after add   | size | shrunk?  | l | width | c  |
    |---|---------|----------------|------|----------|---|-------|----|
    | 0 |    1    | {1:1}          |  1   | no       | 0 |   1   |  1 |
    | 1 |    2    | {1:1, 2:1}     |  2   | no       | 0 |   2   |  3 |
    | 2 |    1    | {1:2, 2:1}     |  2   | no       | 0 |   3   |  6 |
    | 3 |    2    | {1:2, 2:2}     |  2   | no       | 0 |   4   | 10 |
    | 4 |    3    | {1:2, 2:2, 3:1}|  3   | 3 steps  | 3 |   2   | 12 |

    Step count behind the O(n log k) claim:
      across the k = 2 call, r advanced 5 times and l advanced 3 times, all
      of them inside that single r = 4 iteration. 8 pointer steps for a
      5-element array. The inner while loop looks nested but draws on one
      shared budget: l only moves forward and never passes r, so all the
      shrinking together costs at most n steps per call.

    The subtlety to hold onto:
      the subtraction is what makes "exactly" reachable at all. A single
      window cannot count exactly-k subarrays, because for a given r the
      qualifying starts do not form one contiguous run that the window can
      track - but "at most k" does, and exactly(k) is the difference of two
      of those. The same trick answers "exactly k zeros", "exactly k odds",
      and any other equality constraint whose <= form is window-friendly.
*/
