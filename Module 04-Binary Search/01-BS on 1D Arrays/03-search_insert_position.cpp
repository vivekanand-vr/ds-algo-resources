#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array and a target value, return the index if the target
// is found, or the index where it would be inserted to keep the array sorted

/*
    Approach: Binary Search (Lower Bound)
    - This is equivalent to finding the "ceil" position of the target
    - Binary search for the target; whenever nums[mid] < t, the insert
      position must be to the right of mid, so update ans = mid + 1
    - Whenever nums[mid] >= t, the insert position is at or before mid,
      so search left without updating ans

    Algorithm Steps
    ----------------
    1. Initialize l = 0, h = n - 1, ans = 0
    2. While l <= h, compute mid = l + (h - l) / 2
    3. If nums[mid] == t, return mid (target found)
    4. If nums[mid] < t, set ans = l = mid + 1 (insert position moves right)
    5. Else set h = mid - 1 (insert position stays at or before mid)
    6. Return ans (the insert position if target isn't found)

    Time Complexity: O(log n) - binary search on sorted array
    Space Complexity: O(1) - constant extra space
*/
int searchInsert(vector<int>& nums, int t) {
  int n = nums.size();
  int l = 0, h = n - 1;
  int ans = 0;  // stores the index where target should be inserted

  while (l <= h) {
    int mid = l + (h - l) / 2;  // avoid overflow compared to (l+h)/2

    if (nums[mid] == t) {
      // If we find the target, return its index
      return mid;
    } else if (nums[mid] < t) {
      // If target is bigger, move to the right half
      l = mid + 1;
      ans = l;  // potential insert position is just after mid
    } else {
      // If target is smaller, move to the left half
      h = mid - 1;
      // ans doesn't update here, because insertion would be at 'mid'
    }
  }

  return ans;  // if not found, return insert position
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<int> nums = {1, 3, 5, 6};
  int target = 5;

  cout << "Array: ";
  for (int n : nums) cout << n << " ";
  cout << endl;

  cout << "Search insert position of " << target << ": " << searchInsert(nums, target) << endl;

  return 0;
}
/*
    ======================================================================
    DRY RUN: nums = {1, 3, 5, 6}, t = 2   (n = 4, answer = 1)
    ======================================================================

    (main() runs t = 5, which is PRESENT and returns early at mid = 2.
     The instructive case is a MISSING target, so this trace uses t = 2 -
     the run below is what searchInsert({1,3,5,6}, 2) does.)

      index:    0    1    2    3
      value:    1    3    5    6
                     ^
                     2 belongs here: {1, 2, 3, 5, 6} -> insert index 1

    Tracked state:
      l, h  - the inclusive bounds of the part still being searched
      mid   - l + (h - l) / 2, the probe index
      ans   - the insert position so far, start 0. It is written ONLY on
              the nums[mid] < t branch, and always as the new l
              (ans = l = mid + 1) - i.e. "everything up to mid is too
              small, so t goes at least after mid".

    Initial state: l = 0, h = 3, ans = 0

    ----------------------------------------------------------------------
    Iteration 1
      bounds       l = 0, h = 3
      mid          0 + (3 - 0) / 2 = 1
      a[mid]       nums[1] = 3
      compare      3 != 2, and 3 > 2  -> t must be inserted at or BEFORE
                   mid, so mid itself is still a possible answer
      record       nothing - ans stays 0 (this is the "ans does not
                   update here" comment in the code)
      discard      indices 1..3 (all >= 3 > 2)
      update       h = mid - 1 = 0

                     1   3   5   6
                   [ . | x   x   x ]
                          live range now 0..0

    ----------------------------------------------------------------------
    Iteration 2
      bounds       l = 0, h = 0      (one element left: nums[0] = 1)
      mid          0 + (0 - 0) / 2 = 0
      a[mid]       nums[0] = 1
      compare      1 < 2  -> t goes strictly after index 0
      update       l = mid + 1 = 1
      record       ans = l = 1                    <-- the eventual answer
      discard      index 0

                     1   3   5   6
                     ^   ^
                     h   l        h = 0 < l = 1 -> range is empty
      loop test    l = 1 > h = 0  ->  while (l <= h) fails, loop ends

    ----------------------------------------------------------------------
    RETURN ans = 1        (insert 2 at index 1)

    ======================================================================
    Summary table
    ======================================================================

    | iter | l | h | mid | a[mid] | vs t = 2 | ans | half discarded |
    |------|---|---|-----|--------|----------|-----|----------------|
    |  1   | 0 | 3 |  1  |   3    |  3 > 2   |  0  | right (1..3)   |
    |  2   | 0 | 0 |  0  |   1    |  1 < 2   |  1  | left  (0..0)   |
    |      | 1 | 0 |  -  |   -    | l > h    |  1  | loop ends      |

    Step count behind the O(log n) claim:
      4 elements, 2 probes; the live range shrank 4 -> 1 -> 0, i.e.
      ceil(log2(4 + 1)) = 3 probes at worst.

    The teaching point - ans IS l, so either would do:
      ans is only ever assigned the value of l right after l moves, and l
      never moves anywhere else. So at the end ans == l always, and
      `return l;` would be an equivalent (and more idiomatic lower-bound)
      implementation. That is the invariant to hold onto:

          l is always the count of elements known to be strictly < t

      Starting ans at 0 rather than -1 is what makes t = 0 (smaller than
      everything) return 0, and l walking all the way to n is what makes
      t = 9 return 4 - both without any special-casing.

    Contrast with the other three files in this folder:
      - plain binary search returns on the hit, no candidate recorded;
      - floor and ceil record `ans = mid` (an index INTO the array) and
        report -1 when no candidate ever qualifies;
      - insert position records `ans = mid + 1` (a GAP position, so n is a
        legal answer) and can never fail - every t has an insert slot.
      This file still returns mid on an exact hit, which for duplicates
      means it may return any matching index rather than the first one.
*/
