#include <bits/stdc++.h>
using namespace std;

// Q: Given a binary array arr and an integer k, return the length of the
//    longest contiguous subarray containing only 1s, if you are allowed to
//    flip at most k zeroes into ones.
//
// Example:
// arr = [1,1,1,0,0,0,1,1,1,1,0], k = 2 -> 6   (flip the 0s at index 4 and 5,
//                                              giving six 1s at arr[3..8])
// arr = [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], k = 3 -> 10
// arr = [0,0,0], k = 0                 -> 0   (nothing may be flipped)

/*
    Approach: Variable-size sliding window counting the zeroes inside it

    - Do not think about *which* zeroes to flip. A subarray is achievable
      if and only if it contains at most k zeroes, because turning it into
      all-ones costs exactly (number of zeroes) flips. So the problem
      collapses to: find the longest window with zeroCount <= k.
    - Keep a window [l, r] and one counter c = how many zeroes are inside
      it. Growing the window by one element can raise c by at most 1, so
      the invariant c <= k can only ever be broken by the element just
      added - shrink from the left until it holds again.
    - Because the invariant is restored before measuring, every width
      r - l + 1 recorded is genuinely achievable, and the largest wins.
    - No map is needed here: the only thing worth knowing about the window
      is a single integer. That is why this is O(1) extra space, while the
      distinct-character variants of the same pattern need a count table.
    - l and r each only move forward, so the nested while loop is not
      quadratic: every index enters and leaves the window at most once.

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = 0, c = 0 (zeroes in window), res = 0.
    2. While r < n:
         a. If arr[r] == 0, c++ (grow the window to include arr[r]).
         b. While c > k (more zeroes than the flip budget):
              - if arr[l] == 0, c--
              - l++ (shrink from the left)
         c. The window is now flippable -> res = max(res, r - l + 1).
         d. r++.
    3. Return res.

    Time Complexity: O(n) - r advances n times and l advances at most n
                      times in total, with O(1) work per step.
    Space Complexity: O(1) - just the counter and the two pointers.
*/
int longestOnes(vector<int>& arr, int k) {
  int n = arr.size();
  int l = 0, r = 0, c = 0, res = 0;

  while (r < n) {
    if (arr[r] == 0) c++;          // grow: arr[r] joins the window

    while (c > k) {                // more zeroes than flips allowed
      if (arr[l] == 0) {
        c--;                       // the element leaving was a zero
      }
      l++;                         // shrink from the left
    }

    res = max(res, (r - l + 1));   // window holds <= k zeroes here
    r++;
  }

  return res;
}

int main() {
  vector<pair<vector<int>, int>> tests = {
      {{1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0}, 2},
      {{0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1}, 3},
      {{0, 0, 0}, 0}};

  for (auto& t : tests) {
    cout << "Input:  arr = [";
    for (size_t i = 0; i < t.first.size(); ++i) {
      cout << t.first[i] << (i + 1 < t.first.size() ? "," : "");
    }
    cout << "], k = " << t.second << endl;
    cout << "Output: " << longestOnes(t.first, t.second) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [1,1,1,0,0,0,1,1,1,1,0], k = 2   (n = 11, answer = 6)
    ==========================================================================

      index:  0  1  2  3  4  5  6  7  8  9 10
      value:  1  1  1  0  0  0  1  1  1  1  0

    Tracked state:
      c     - how many ZEROES are currently inside the window [l..r]
      l, r  - the window bounds; the window is arr[l..r], width = r - l + 1
      res   - the best (largest) width seen so far

    Initial state: l = 0, r = 0, c = 0, res = 0

    --------------------------------------------------------------------------
    r = 0..2, values 1, 1, 1
      add          all ones, c stays 0
      shrink?      c = 0 <= k = 2 -> never
      windows      "1", "11", "111" of width 1, 2, 3
      measure      res climbs 1 -> 2 -> 3
      state        l = 0, c = 0

    --------------------------------------------------------------------------
    r = 3, arr[r] = 0
      add          zero -> c = 1
      shrink?      c = 1 <= 2 -> no
      window       arr[0..3] = 1,1,1,0, width = 4        (one flip spent)
      measure      res = max(3, 4) = 4
      advance      l = 0, r -> 4

    --------------------------------------------------------------------------
    r = 4, arr[r] = 0
      add          zero -> c = 2
      shrink?      c = 2 <= 2 -> no, exactly at budget
      window       arr[0..4] = 1,1,1,0,0, width = 5      (both flips spent)
      measure      res = max(4, 5) = 5                   <-- best so far
      advance      l = 0, r -> 5

    --------------------------------------------------------------------------
    r = 5, arr[r] = 0      <-- OVER BUDGET: a third zero
      add          zero -> c = 3
      shrink       c = 3 > 2, so drop from the left until only two zeroes
                   remain. Note the leading ONES have to leave first even
                   though they are not the problem - they sit to the left of
                   the oldest zero and a window cannot skip over them.

                   step 1: drop arr[l=0] = 1  -> not a zero, c stays 3, l -> 1
                           re-check: 3 > 2, keep shrinking
                   step 2: drop arr[l=1] = 1  -> c stays 3, l -> 2
                           re-check: 3 > 2, keep shrinking
                   step 3: drop arr[l=2] = 1  -> c stays 3, l -> 3
                           re-check: 3 > 2, keep shrinking
                   step 4: drop arr[l=3] = 0  -> a zero leaves, c = 2, l -> 4
                           re-check: 2 <= 2 -> stop

                     1  1  1  0  0  0  1  1  1  1  0
                                 ^  ^
                                 l  r        window = 0,0 (both flipped)

      window       arr[4..5], width = 2
      measure      res = max(5, 2) = 5       (unchanged - smaller window)
      advance      l = 4, r -> 6

    --------------------------------------------------------------------------
    r = 6..9, values 1, 1, 1, 1
      add          all ones, c stays 2
      shrink?      c = 2 <= 2 -> never, l stays parked at 4
      windows      arr[4..6] w=3, arr[4..7] w=4, arr[4..8] w=5, arr[4..9] w=6
      measure      res reaches max(5, 6) = 6 at r = 9    <-- ANSWER FOUND

                     1  1  1  0  0  0  1  1  1  1  0
                                 ^              ^
                                 l              r
                     flip the zeroes at 4 and 5 -> 1,1,1,1,1,1

    --------------------------------------------------------------------------
    r = 10, arr[r] = 0     <-- OVER BUDGET again
      add          zero -> c = 3
      shrink       step 1: drop arr[4] = 0 -> c = 2, l -> 5, 2 <= 2, stop
      window       arr[5..10] = 0,1,1,1,1,0, width = 6
      measure      res = max(6, 6) = 6
      advance      l = 5, r -> 11   ->   r == n, loop ends

    --------------------------------------------------------------------------
    RETURN res = 6      (arr[3..8] flipped to 1,1,1,1,1,1 - or equally
                         arr[5..10], which also costs exactly two flips)

    ==========================================================================
    Summary table
    ==========================================================================

    |  r | arr[r] | c after add | shrunk? |  l | width | res |
    |----|--------|-------------|---------|----|-------|-----|
    |  0 |   1    |      0      | no      |  0 |   1   |  1  |
    |  1 |   1    |      0      | no      |  0 |   2   |  2  |
    |  2 |   1    |      0      | no      |  0 |   3   |  3  |
    |  3 |   0    |      1      | no      |  0 |   4   |  4  |
    |  4 |   0    |      2      | no      |  0 |   5   |  5  |
    |  5 |   0    |      3      | 4 steps |  4 |   2   |  5  |
    |  6 |   1    |      2      | no      |  4 |   3   |  5  |
    |  7 |   1    |      2      | no      |  4 |   4   |  5  |
    |  8 |   1    |      2      | no      |  4 |   5   |  5  |
    |  9 |   1    |      2      | no      |  4 |   6   |  6  |
    | 10 |   0    |      3      | 1 step  |  5 |   6   |  6  |

    Why this is O(n) and not O(n^2):
      r advanced 11 times and l advanced 5 times in total (0 -> 4 at r = 5,
      then 4 -> 5 at r = 10). That is 16 pointer steps for an 11-element
      array, never 11 x 11. The inner while loop is nested in the source but
      not in the cost: l only moves forward and never passes r, so all the
      shrinking across the whole run costs at most n steps combined.

    Note on the k = 0 edge case:
      arr = [0,0,0], k = 0 -> the moment a zero is added c = 1 > 0, so the
      window is shrunk until it is EMPTY (l lands on r + 1) and the width
      r - l + 1 evaluates to 0. The loop is still safe because l stops as
      soon as the invariant holds, and an empty window trivially holds it.
      res stays 0, which is the correct answer.
*/
