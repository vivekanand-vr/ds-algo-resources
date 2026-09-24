#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an array of DISTINCT integers, sorted in ascending order and then
// ROTATED at some unknown pivot, return its minimum element in O(log n).
//
// Example:
// arr = {3, 4, 5, 1, 2}           -> 1
// arr = {4, 5, 6, 7, 0, 1, 2}     -> 0
// arr = {11, 13, 15, 17}          -> 11   (rotated by 0 - already sorted)
// arr = {5}                       -> 5

/*
    Approach 1: Shrink toward the break by comparing mid with the RIGHT end

    - A rotated sorted array is two ascending runs, the second made of values
      all smaller than the first: {4, 5, 6, 7 | 0, 1, 2}. The minimum is the
      first element of the second run - the element right after the break.
    - Compare arr[mid] with arr[high]:
        arr[mid] >  arr[high]  ->  mid sits in the FIRST run, above the
                                    break. The break (and the minimum) is
                                    strictly to the right: low = mid + 1.
        arr[mid] <= arr[high]  ->  mid..high rises cleanly, so mid is in the
                                    second run. The minimum is mid itself or
                                    something to its left: high = mid (NOT
                                    mid - 1, since mid may be the answer).
    - Because the right branch keeps mid, the loop runs while low < high and
      stops when the range is a single index - that index is the minimum.
      There is no "found" test and no candidate variable: the range itself
      converges onto the answer.
    - Why arr[high] and not arr[low]: arr[high] is correct even when the
      array was rotated by 0. On {1, 2, 3, 4, 5}, arr[mid] > arr[low] would
      claim "first run, go right" and walk away from index 0. Against
      arr[high] the same array always takes high = mid and lands on index 0.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1.
    2. While low < high, compute mid = low + (high - low) / 2.
    3. If arr[mid] > arr[high], the minimum is right of mid: low = mid + 1.
    4. Else the minimum is at mid or to its left: high = mid.
    5. Return arr[low] (low == high here).

    Time Complexity: O(log n) - every iteration halves the range.
    Space Complexity: O(1) - three indices.
*/
int findMin(vector<int>& arr) {
  int n = arr.size();
  int low = 0, high = n - 1;

  while (low < high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (arr[mid] > arr[high]) {
      low = mid + 1;  // mid is in the first (higher) run, the break is right of it
    } else {
      high = mid;  // mid..high is sorted, so mid itself may be the minimum
    }
  }

  return arr[low];
}

/*
    Approach 2: Identify the sorted half and take its smallest element

    - Reuses the reasoning of 09-search_in_rotated_sorted.cpp: at least one
      of low..mid and mid..high is sorted, and a sorted range's minimum is
      simply its first element - so it can be recorded and thrown away.
    - If arr[low] <= arr[high], the whole live range is sorted: arr[low] is
      its minimum and the search can stop.
    - Else if arr[low] <= arr[mid], the LEFT half is sorted: record arr[low]
      as a candidate and search the right half (low = mid + 1).
    - Else the RIGHT half is sorted: record arr[mid] as a candidate and
      search the left half (high = mid - 1). mid is already recorded, so it
      can be excluded.
    - The answer is the smallest candidate recorded. It does more work per
      iteration than Approach 1 and needs the extra ans variable, but it
      carries over directly to "search in rotated sorted", so it is worth
      knowing both.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1, ans = INT_MAX.
    2. While low <= high, compute mid = low + (high - low) / 2.
    3. If arr[low] <= arr[high]: ans = min(ans, arr[low]) and stop.
    4. Else if arr[low] <= arr[mid]: ans = min(ans, arr[low]), low = mid + 1.
    5. Else: ans = min(ans, arr[mid]), high = mid - 1.
    6. Return ans.

    Time Complexity: O(log n) - every iteration halves the range.
    Space Complexity: O(1) - three indices and the running minimum.
*/
int findMinSortedHalf(vector<int>& arr) {
  int n = arr.size();
  int low = 0, high = n - 1;
  int ans = INT_MAX;

  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (arr[low] <= arr[high]) {
      ans = min(ans, arr[low]);  // live range is fully sorted, its first is its min
      break;
    }

    if (arr[low] <= arr[mid]) {
      ans = min(ans, arr[low]);  // left half sorted, arr[low] is its min
      low = mid + 1;
    } else {
      ans = min(ans, arr[mid]);  // right half sorted, arr[mid] is its min
      high = mid - 1;
    }
  }

  return ans;
}

int main() {
  vector<vector<int>> tests = {{3, 4, 5, 1, 2}, {4, 5, 6, 7, 0, 1, 2}, {11, 13, 15, 17}, {5}};

  for (auto& arr : tests) {
    cout << "Array: ";
    for (int v : arr) cout << v << " ";
    cout << endl;
    cout << "  findMin (compare with high): " << findMin(arr) << endl;
    cout << "  findMin (sorted half):       " << findMinSortedHalf(arr) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of findMin (Approach 1): arr = {4, 5, 6, 7, 0, 1, 2}
    (n = 7, answer = 0 at index 4)
    ======================================================================

      index:    0    1    2    3    4    5    6
      value:    4    5    6    7    0    1    2
                \-----------------/ \-----------/
                     run 1              run 2
                                    ^
                                    the minimum: first element of run 2

    Tracked state:
      low, high - the inclusive bounds; the minimum is always inside them
      mid       - low + (high - low) / 2, the probe index

    Initial state: low = 0, high = 6

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 6      (7 candidates alive)
      mid          0 + (6 - 0) / 2 = 3
      compare      arr[mid] = 7  vs  arr[high] = 2
                   7 > 2  ->  mid is in run 1, above the break
      conclude     everything in 0..3 is larger than arr[high], so none of
                   it can be the minimum
      update       low = mid + 1 = 4

                     4   5   6   7 | 0   1   2
                   [ x   x   x   x | .   .   . ]
                                     live range is now 4..6

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 4, high = 6      (3 candidates alive)
      mid          4 + (6 - 4) / 2 = 5
      compare      arr[mid] = 1  vs  arr[high] = 2
                   1 <= 2  ->  5..6 rises cleanly, mid is in run 2
      conclude     the minimum is arr[5] or somewhere left of it; keep mid
      update       high = mid = 5

                     4   5   6   7   0   1 | 2
                   [ x   x   x   x   .   . | x ]
                                     live range is now 4..5

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 4, high = 5      (2 candidates alive)
      mid          4 + (5 - 4) / 2 = 4
      compare      arr[mid] = 0  vs  arr[high] = 1
                   0 <= 1  ->  mid is in run 2
      update       high = mid = 4

    ----------------------------------------------------------------------
    Loop ends: low = high = 4
    RETURN arr[4] = 0

    ======================================================================
    Summary table (Approach 1)
    ======================================================================

    | iter | low | high | mid | arr[mid] | arr[high] | branch       | update   |
    |------|-----|------|-----|----------|-----------|--------------|----------|
    |  1   |  0  |  6   |  3  |    7     |     2     | mid > high   | low = 4  |
    |  2   |  4  |  6   |  5  |    1     |     2     | mid <= high  | high = 5 |
    |  3   |  4  |  5   |  4  |    0     |     1     | mid <= high  | high = 4 |

    ======================================================================
    DRY RUN of findMinSortedHalf (Approach 2): same array
    ======================================================================

    Initial state: low = 0, high = 6, ans = INT_MAX

    | iter | low | high | mid | a[lo],a[mid],a[hi] | case              | ans | update  |
    |------|-----|------|-----|--------------------|-------------------|-----|---------|
    |  1   |  0  |  6   |  3  |     4, 7, 2        | left sorted       |  4  | low = 4 |
    |  2   |  4  |  6   |  5  |     0, 1, 2        | whole range sorted|  0  | break   |

      Iteration 1: arr[low] = 4 > arr[high] = 2, so the range is rotated;
      arr[low] = 4 <= arr[mid] = 7, so 0..3 is the sorted half. Its minimum
      4 is recorded and the half is dropped.
      Iteration 2: arr[4] = 0 <= arr[6] = 2 - the break has been cut away and
      4..6 is sorted, so its first element 0 is the minimum of what is left.

    RETURN ans = min(4, 0) = 0      (agrees with Approach 1)

    ======================================================================
    Notes
    ======================================================================

    Why high = mid and not high = mid - 1 (Approach 1):
      the else branch says "mid is in the second run", and the first element
      of the second run IS the answer. mid - 1 would discard it whenever mid
      lands exactly on the minimum (iteration 3 above). Keeping mid is also
      why the loop condition is low < high: with low <= high and high = mid,
      a one-element range would loop forever.

    Why the right branch cannot be arr[mid] >= arr[high]:
      with distinct values arr[mid] == arr[high] happens only when mid ==
      high, which low < high rules out, so the choice of > or >= is
      cosmetic here. It stops being cosmetic once duplicates are allowed.

    With duplicates (LeetCode 154):
      arr[mid] == arr[high] no longer says which run mid is in -
      {3, 1, 3, 3, 3} and {3, 3, 3, 1, 3} both read 3 at mid = 2 and at
      high, yet the minimum is left of mid in one and right in the other. The fix
      mirrors 10-search_in_rotated_sorted_II.cpp: when they are equal, drop
      high with high-- (safe because arr[mid] is an equal value still in
      range), giving O(n) worst case:

          if (arr[mid] > arr[high]) low = mid + 1;
          else if (arr[mid] < arr[high]) high = mid;
          else high--;

    Edge cases:
      - rotation by 0 ({11, 13, 15, 17}): Approach 1 always takes high = mid
        and ends at index 0; Approach 2 breaks on the first iteration.
      - single element: low == high from the start, the loop never runs.
      - two elements {2, 1}: mid = 0, arr[0] = 2 > arr[1] = 1 -> low = 1.

    Companion files:
      13-find_kth_rotation.cpp asks for the INDEX of this minimum (the number
      of rotations) - the same search, returning low instead of arr[low].
      09-search_in_rotated_sorted.cpp is the sorted-half test Approach 2
      borrows.
*/
