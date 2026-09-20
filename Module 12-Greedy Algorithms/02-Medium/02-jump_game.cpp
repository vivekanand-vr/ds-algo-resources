#include <bits/stdc++.h>
using namespace std;

// Q: You are given an integer array nums. You start at index 0. nums[i]
//    is the maximum length you may jump forward from index i (you may
//    jump any length from 0 up to nums[i]). Determine whether you can
//    reach the last index.
//
// Example:
// nums = [2,3,1,1,4]  -> true   (0->1->4, or 0->2->3->4, among others)
// nums = [3,2,1,0,4]  -> false  (every path gets stuck at index 3, whose
//                                 jump length is 0, before reaching 4)
// nums = [0]          -> true   (already at the last index)

/*
    Approach: Greedy, working backward with a "last good index" pointer

    - Instead of asking "how far can I get starting from the front", ask
      the reverse question: "how close to the front can a guaranteed
      success still be found". Maintain t = the leftmost index that is
      already known to be able to reach the end (initially the last
      index itself, since it trivially "reaches" itself).
    - Scan i from n-2 down to 0. If index i can jump at least as far as
      t (i.e. i + nums[i] >= t), then from i you can reach some index
      that is already known good, and index i itself becomes the new,
      further-left good index: t = i.
    - Because t only ever moves left (or stays put) and every index
      between the old t and the new t was skipped over precisely
      because it could NOT reach t, the final value of t is the
      smallest index from which the end is reachable. The array is
      solvable from the start if and only if that smallest good index
      is 0.

    Algorithm Steps
    ----------------
    1. Let n = nums.size() and t = n - 1 (the last index is trivially
       "good").
    2. For i from n - 2 down to 0:
         a. If i + nums[i] >= t: this index can reach a good index, so
            it becomes the new good index -> t = i.
    3. Return true if t == 0, false otherwise.

    Time Complexity: O(n) - a single backward pass over the array.
    Space Complexity: O(1) - only the pointer t is tracked.
*/
bool canJump(vector<int>& nums) {
  int n = nums.size();
  int t = n - 1;   // last index is trivially able to "reach" itself

  for (int i = n - 2; i >= 0; --i) {
    if (i + nums[i] >= t) {
      t = i;   // index i can reach a good index, so it is good too
    }
  }

  return t == 0;
}

int main() {
  struct Test {
    vector<int> nums;
  };
  vector<Test> tests = {
      {{2, 3, 1, 1, 4}}, {{3, 2, 1, 0, 4}}, {{0}}, {{1, 0, 1, 0}}};

  for (auto& t : tests) {
    cout << "Input:  nums = [";
    for (size_t i = 0; i < t.nums.size(); ++i) {
      cout << t.nums[i] << (i + 1 < t.nums.size() ? "," : "");
    }
    cout << "]" << endl;
    cout << "Output: " << (canJump(t.nums) ? "true" : "false") << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = [3,2,1,0,4]   (n = 5, answer false)
    ==========================================================================

      index:   0   1   2   3   4
      nums:    3   2   1   0   4

    Tracked state:
      i  - index being examined, scanning right to left
      t  - leftmost index known to be able to reach the end

    Initial state: t = n - 1 = 4

    --------------------------------------------------------------------------
    i = 3: nums[3] = 0
      compare   i + nums[i] >= t?   3 + 0 = 3 >= 4?   NO
      action    t unchanged
      state     t = 4

    --------------------------------------------------------------------------
    i = 2: nums[2] = 1
      compare   2 + 1 = 3 >= 4?   NO
      action    t unchanged
      state     t = 4

    --------------------------------------------------------------------------
    i = 1: nums[1] = 2
      compare   1 + 2 = 3 >= 4?   NO
      action    t unchanged
      state     t = 4

    --------------------------------------------------------------------------
    i = 0: nums[0] = 3
      compare   0 + 3 = 3 >= 4?   NO
      action    t unchanged
      state     t = 4

    --------------------------------------------------------------------------
    Loop ends (i has gone past 0)

    RETURN t == 0?   4 == 0?   NO   ->   false

      index 3 has jump length 0, so index 3 can never reach index 4.
      Because index 3 is unreachable-from in the "good" sense, no index
      to its left (0, 1, or 2) can jump far enough to skip past it and
      land on the last index either - every one of them can reach at
      most as far as index 3, which is itself stuck. t never moves off
      the last index, so index 0 is never marked good.

    ==========================================================================
    Summary table
    ==========================================================================

    |  i  | nums[i] | i+nums[i] | >= t (4)? | action        | t after |
    |-----|---------|-----------|-----------|---------------|---------|
    |  3  |    0    |     3     |    no     | t unchanged   |    4    |
    |  2  |    1    |     3     |    no     | t unchanged   |    4    |
    |  1  |    2    |     3     |    no     | t unchanged   |    4    |
    |  0  |    3    |     3     |    no     | t unchanged   |    4    |

    ==========================================================================
    Second dry run: nums = [2,3,1,1,4]   (answer true)
    ==========================================================================

      index:   0   1   2   3   4
      nums:    2   3   1   1   4

      t = 4 initially.
      i=3: nums[3]=1, 3+1=4 >= 4   -> YES -> t = 3
      i=2: nums[2]=1, 2+1=3 >= 3   -> YES -> t = 2
      i=1: nums[1]=3, 1+3=4 >= 2   -> YES -> t = 1
      i=0: nums[0]=2, 0+2=2 >= 1   -> YES -> t = 0

      RETURN t == 0?   YES   ->   true

    ==========================================================================
    Notes
    ==========================================================================

    Why the "last good index" invariant is correct:
      t always denotes the smallest index proven to reach the end, given
      everything examined so far (indices > t). When index i is checked,
      every index between i and the current t has already been
      determined, and i is compared only against the CLOSEST known-good
      index, t. If i can reach t, it can reach the end via t, so marking
      i as the new t is safe. If i cannot reach t, it certainly cannot
      reach any good index further right either (they are all farther
      away), so i is correctly left as not (yet) known good.

    Why scanning backward instead of forward works just as well:
      the forward version of this greedy tracks the furthest reachable
      index while scanning left to right and fails early if that
      frontier ever falls behind the current position. This backward
      version instead shrinks a "boundary of success" from the right
      until it either reaches index 0 (success) or cannot be pulled any
      further left (failure). Both express the same greedy idea - only
      the direction of the scan and what is tracked differ.

    Complexity note:
      a single O(n) pass with O(1) extra space; no sorting or auxiliary
      structures are needed.
*/
