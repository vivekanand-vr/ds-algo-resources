#include <bits/stdc++.h>
using namespace std;

// Q: You are given a row of fruit trees, arr[i] being the type of fruit on
//    tree i. You carry two baskets, and each basket can hold only ONE type
//    of fruit (any amount of it). Starting at a tree of your choice you must
//    pick exactly one fruit from every tree, moving right, and you stop the
//    moment you reach a fruit that fits in neither basket. Return the
//    maximum number of fruits you can pick.
//
//    Restated: find the length of the longest subarray containing at most
//    2 distinct values.
//
// Example:
// arr = [1,2,1]       -> 3   (the whole row: types {1,2})
// arr = [0,1,2,2]     -> 3   ([1,2,2], types {1,2}; the whole row has 3 types)
// arr = [1,2,3,2,2]   -> 4   ([2,3,2,2], types {2,3})

/*
    Approach: Variable-size sliding window with a type -> count map

    - "Two baskets" is just a cap on DISTINCT values, not on quantity, so
      the condition to maintain is mp.size() <= 2 where mp maps each fruit
      type inside the window to how many of it the window holds.
    - Keep a window [l, r]. Adding one fruit can raise the distinct count
      by at most 1, so the invariant can only break because of the fruit
      just added - shrink from the left until the window is back to two
      types or fewer.
    - The counts matter even though the question is about distinctness:
      a type only really leaves the window when its LAST copy is dropped.
      That is why the value is a count and the key is erased only when the
      count hits 0 - mp.size() must equal the number of types actually
      present, or the shrink loop would stop too early.
    - k is kept as a named variable rather than hard-coding 2, because the
      identical code solves the general "longest subarray with at most k
      distinct" problem - this problem is just that with k = 2.
    - l and r each only move forward, so the nested while loop is linear
      overall, not quadratic.

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = 0, res = 0, k = 2 and an empty count map.
    2. While r < n:
         a. mp[arr[r]]++ (grow the window to include tree r).
         b. While mp.size() > k (a third fruit type is inside):
              - mp[arr[l]]--, erase the key if its count hits 0
              - l++ (shrink from the left)
         c. The window now uses at most 2 baskets -> res = max(res, r-l+1).
         d. r++.
    3. Return res.

    Time Complexity: O(n log k) - each index enters and leaves the window
                      once, and each map operation costs O(log k). Swapping
                      map for unordered_map makes it O(n); with k = 2 the
                      log factor is a constant anyway.
    Space Complexity: O(k) = O(1) here - the map never holds more than 3
                      keys (2, briefly 3 before the shrink).
*/
int totalFruit(vector<int>& arr) {
  int n = arr.size();
  int l = 0, r = 0, res = 0, k = 2;  // max 2 fruits
  map<int, int> mp;

  while (r < n) {
    mp[arr[r]]++;                    // grow: tree r's fruit joins the window

    while (mp.size() > k) {          // a third type appeared
      mp[arr[l]]--;
      if (mp[arr[l]] == 0) {
        mp.erase(arr[l]);            // last copy gone -> the type truly left
      }
      l++;                           // shrink from the left
    }

    res = max(res, (r - l + 1));     // window fits in 2 baskets here
    r++;
  }

  return res;
}

int main() {
  vector<vector<int>> tests = {{1, 2, 1}, {0, 1, 2, 2}, {1, 2, 3, 2, 2}};

  for (auto& arr : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < arr.size(); ++i) {
      cout << arr[i] << (i + 1 < arr.size() ? "," : "");
    }
    cout << "]" << endl;
    cout << "Output: " << totalFruit(arr) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [1,2,3,2,2]   (n = 5, answer = 4)
    ==========================================================================

      index:  0  1  2  3  4
      type:   1  2  3  2  2

    Tracked state:
      mp    - fruit type -> how many of it sit inside the window [l..r]
              (mp.size() is therefore the number of DISTINCT types = baskets)
      l, r  - the window bounds; the window is arr[l..r], width = r - l + 1
      res   - the best (largest) width seen so far

    Initial state: l = 0, r = 0, res = 0, mp = {}

    --------------------------------------------------------------------------
    r = 0, arr[r] = 1
      add          mp[1]++              -> mp = {1:1}
      shrink?      size = 1, not > 2    -> no shrink needed
      window       arr[0..0] = [1], width = 1           (baskets: {1})
      measure      res = max(0, 1) = 1
      advance      l = 0, r -> 1

    --------------------------------------------------------------------------
    r = 1, arr[r] = 2
      add          mp[2]++              -> mp = {1:1, 2:1}
      shrink?      size = 2, not > 2    -> no, both baskets now in use
      window       arr[0..1] = [1,2], width = 2         (baskets: {1,2})
      measure      res = max(1, 2) = 2                  <-- best so far
      advance      l = 0, r -> 2

    --------------------------------------------------------------------------
    r = 2, arr[r] = 3      <-- THIRD TYPE: no basket left for it
      add          mp[3]++              -> mp = {1:1, 2:1, 3:1}, size = 3
      shrink       size = 3 > 2, so dump fruit from the left until only two
                   types remain. Dropping the single 1 is enough here, and
                   it is exactly one step because type 1 had only one copy.

                   step 1: drop arr[l=0] = 1
                             mp[1]-- -> 0, count hit zero -> ERASE key 1
                             mp = {2:1, 3:1},  l -> 1
                             re-check: size = 2, not > 2 -> stop

                     1  2  3  2  2
                        ^  ^
                        l  r          baskets are now {2,3}

      window       arr[1..2] = [2,3], width = 2
      measure      res = max(2, 2) = 2
      advance      l = 1, r -> 3

    --------------------------------------------------------------------------
    r = 3, arr[r] = 2
      add          mp[2]++              -> mp = {2:2, 3:1}
      shrink?      size = 2, not > 2    -> no. Note the COUNT grew but the
                   distinct count did not - a basket holds any amount.
      window       arr[1..3] = [2,3,2], width = 3
      measure      res = max(2, 3) = 3                  <-- new best
      advance      l = 1, r -> 4

    --------------------------------------------------------------------------
    r = 4, arr[r] = 2
      add          mp[2]++              -> mp = {2:3, 3:1}
      shrink?      size = 2, not > 2    -> no
      window       arr[1..4] = [2,3,2,2], width = 4     (baskets: {2,3})
      measure      res = max(3, 4) = 4                  <-- ANSWER
      advance      l = 1, r -> 5   ->   r == n, loop ends

                     1  2  3  2  2
                        ^        ^
                        l        r     pick 2,3,2,2 = four fruits

    --------------------------------------------------------------------------
    RETURN res = 4      (start at tree 1: basket A takes the 2s, basket B
                         takes the single 3)

    ==========================================================================
    Summary table
    ==========================================================================

    | r | arr[r] | mp after add     | size | shrunk? | l | window    | w | res |
    |---|--------|------------------|------|---------|---|-----------|---|-----|
    | 0 |   1    | {1:1}            |  1   | no      | 0 | [1]       | 1 |  1  |
    | 1 |   2    | {1:1, 2:1}       |  2   | no      | 0 | [1,2]     | 2 |  2  |
    | 2 |   3    | {1:1, 2:1, 3:1}  |  3   | 1 step  | 1 | [2,3]     | 2 |  2  |
    | 3 |   2    | {2:2, 3:1}       |  2   | no      | 1 | [2,3,2]   | 3 |  3  |
    | 4 |   2    | {2:3, 3:1}       |  2   | no      | 1 | [2,3,2,2] | 4 |  4  |

    Why the erase is mandatory here:
      In problem 01 (longest substring without repeats) the test was on
      mp[s[r]], so a leftover zero-count key was harmless. Here the test is
      mp.size() > k. If the key 1 were left behind with count 0 at r = 2,
      mp.size() would stay 3 forever and the shrink loop would keep eating
      the window - the answer would collapse. Erase on zero is what keeps
      mp.size() equal to the true number of distinct types in [l..r].

    Why this is O(n) and not O(n^2):
      r advanced 5 times and l advanced exactly once in total. Each index is
      added to the window once and removed at most once, so the inner loop's
      total work across the whole run is bounded by n, not multiplied by it.

    Generalisation:
      Set k = 3 and the same function answers "longest subarray with at most
      3 distinct values"; the only thing specific to this problem is the
      literal 2. The classic follow-up "longest substring with at most k
      distinct characters" is this exact code over a string.
*/
