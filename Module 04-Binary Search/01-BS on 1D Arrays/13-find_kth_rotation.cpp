#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an array of DISTINCT integers that was sorted in ascending order and
// then rotated RIGHT k times, find k - the number of rotations.
//
// Example:
// arr = {5, 1, 2, 3, 4}           -> 1
// arr = {15, 18, 2, 3, 6, 12}     -> 2
// arr = {4, 5, 6, 7, 0, 1, 2}     -> 4
// arr = {1, 2, 3, 4, 5}           -> 0   (not rotated)

/*
    Approach 1: Find the index of the minimum, comparing against the LEFT end

    - Rotating a sorted array right k times moves its smallest element from
      index 0 to index k. So the rotation count IS the index of the minimum,
      and this is 11-find_minimum_in_rotated_sorted.cpp returning an index
      instead of a value.
    - Early exit: if arr[low] < arr[high], the live range contains no break
      and is sorted, so its minimum is at low. This also answers the
      unrotated case immediately.
    - Otherwise the range is genuinely rotated (arr[low] > arr[high]), and
      comparing mid with arr[low] tells which run mid is in:
        arr[mid] <  arr[low]  ->  mid is in the SECOND (lower) run. The
                                   minimum is mid or to its left: high = mid.
        arr[mid] >= arr[low]  ->  mid is in the FIRST (higher) run. The
                                   minimum is strictly right: low = mid + 1.
    - The loop runs while low < high and keeps mid on the high = mid branch,
      so the range converges on the minimum's index.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1.
    2. While low < high, compute mid = low + (high - low) / 2.
    3. If arr[low] < arr[high], the range is sorted: return low.
    4. Else if arr[mid] < arr[low], the minimum is at mid or left: high = mid.
    5. Else the minimum is right of mid: low = mid + 1.
    6. Return low.

    Time Complexity: O(log n) - every iteration halves the range, and the
                      early exit can only cut it shorter.
    Space Complexity: O(1) - three indices.
*/
int findKRotation(vector<int>& arr) {
  int n = arr.size();
  int low = 0, high = n - 1;

  while (low < high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (arr[low] < arr[high]) {
      return low;  // no break inside low..high, so the minimum is its first element
    } else if (arr[mid] < arr[low]) {
      high = mid;  // mid fell below arr[low], so the break is at or before mid
    } else {
      low = mid + 1;  // mid is still in the higher run, the break is after it
    }
  }

  return low;
}

/*
    Approach 2: Find the index of the minimum, comparing against the RIGHT end

    - The findMin loop from 11-find_minimum_in_rotated_sorted.cpp, returning
      low instead of arr[low]:
        arr[mid] >  arr[high]  ->  mid is in the first run: low = mid + 1.
        arr[mid] <= arr[high]  ->  mid..high is sorted: high = mid.
    - Comparing with arr[high] needs no early exit: on an unrotated array
      every probe takes high = mid and the range walks down to index 0 on
      its own. That is one branch fewer than Approach 1, at the cost of not
      stopping early when the range becomes sorted.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1.
    2. While low < high, compute mid = low + (high - low) / 2.
    3. If arr[mid] > arr[high], low = mid + 1; else high = mid.
    4. Return low.

    Time Complexity: O(log n)
    Space Complexity: O(1)
*/
int findKRotationCompareHigh(vector<int>& arr) {
  int low = 0, high = arr.size() - 1;

  while (low < high) {
    int mid = low + (high - low) / 2;

    if (arr[mid] > arr[high]) {
      low = mid + 1;  // mid is above the break
    } else {
      high = mid;  // mid..high is sorted, mid may be the minimum
    }
  }

  return low;
}

int main() {
  vector<vector<int>> tests = {{5, 1, 2, 3, 4}, {15, 18, 2, 3, 6, 12}, {4, 5, 6, 7, 0, 1, 2}, {1, 2, 3, 4, 5}};

  for (auto& arr : tests) {
    cout << "Array: ";
    for (int v : arr) cout << v << " ";
    cout << endl;
    cout << "  rotations (compare with low):  " << findKRotation(arr) << endl;
    cout << "  rotations (compare with high): " << findKRotationCompareHigh(arr) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of findKRotation (Approach 1): arr = {4, 5, 6, 7, 0, 1, 2}
    (n = 7, answer = 4)
    ======================================================================

      index:    0    1    2    3    4    5    6
      value:    4    5    6    7    0    1    2
                \-----------------/ \-----------/
                     run 1              run 2
                                    ^
                                    minimum at index 4 -> rotated 4 times

    Tracked state:
      low, high - the inclusive bounds; the minimum's index is inside them
      mid       - low + (high - low) / 2, the probe index

    Initial state: low = 0, high = 6

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 6      (7 candidates alive)
      mid          0 + (6 - 0) / 2 = 3
      sorted?      arr[low] = 4 < arr[high] = 2 ?  no  ->  range is rotated
      compare      arr[mid] = 7  vs  arr[low] = 4
                   7 >= 4  ->  mid is in run 1
      conclude     the break is strictly to the right of mid
      update       low = mid + 1 = 4

                     4   5   6   7 | 0   1   2
                   [ x   x   x   x | .   .   . ]
                                     live range is now 4..6

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 4, high = 6      (3 candidates alive)
      mid          4 + (6 - 4) / 2 = 5
      sorted?      arr[low] = 0 < arr[high] = 2 ?  YES
      conclude     the break has been cut away; 4..6 is sorted, so its
                   first index holds the minimum
      return       4 immediately (mid is never looked at)

    ----------------------------------------------------------------------
    RETURN 4

    ======================================================================
    Summary table (Approach 1)
    ======================================================================

    | iter | low | high | mid | arr[lo] < arr[hi] | arr[mid] vs arr[lo] | action     |
    |------|-----|------|-----|-------------------|---------------------|------------|
    |  1   |  0  |  6   |  3  |  4 < 2  -> no     |  7 >= 4  (run 1)    | low = 4    |
    |  2   |  4  |  6   |  5  |  0 < 2  -> YES    |  -                  | return 4   |

    ======================================================================
    DRY RUN of findKRotationCompareHigh (Approach 2): same array
    ======================================================================

    | iter | low | high | mid | arr[mid] | arr[high] | branch       | update   |
    |------|-----|------|-----|----------|-----------|--------------|----------|
    |  1   |  0  |  6   |  3  |    7     |     2     | mid > high   | low = 4  |
    |  2   |  4  |  6   |  5  |    1     |     2     | mid <= high  | high = 5 |
    |  3   |  4  |  5   |  4  |    0     |     1     | mid <= high  | high = 4 |

      Loop ends at low = high = 4.  RETURN 4      (agrees with Approach 1)

      One iteration more than Approach 1: after iteration 1 the range 4..6
      is already sorted, but Approach 2 has no test for that and keeps
      halving until it is a single index.

    ======================================================================
    Notes
    ======================================================================

    The early exit in Approach 1 is load-bearing, not just an optimisation:
      the arr[mid] vs arr[low] test only means "which run is mid in" when a
      break actually exists inside low..high. Without the early exit, the
      unrotated {1, 2, 3, 4, 5} would see arr[2] = 3 >= arr[0] = 1, conclude
      "first run, go right", and return 4 instead of 0. The early exit
      guarantees arr[low] > arr[high] whenever the comparison is reached.
      Approach 2 compares against arr[high] precisely so that it does not
      need this guard.

    Why mid == low is handled correctly:
      with two elements left, mid = low. Past the early exit we know
      arr[low] > arr[high], so the minimum is at high = low + 1. The test
      arr[mid] < arr[low] is false (they are the same element), so the else
      branch sets low = mid + 1 = high - the right answer.

    Change from the original: return low instead of return low % n.
      low is always in 0..n-1, so the modulo never changed the result, and
      on an empty array (n = 0) it was a division by zero. It was also
      likely meant for the "rotated n times" case - but rotating by n gives
      back the sorted array, whose minimum is at index 0 already.

    Duplicates are not supported by either approach:
      {2, 0, 2, 2, 2} has arr[low] == arr[mid] == arr[high], so neither
      comparison can place the break, and Approach 1 returns 4 instead of 1.
      The fix is the same shrink step as 10-search_in_rotated_sorted_II.cpp
      (see the Notes in 11-find_minimum_in_rotated_sorted.cpp).

    Edge cases:
      - not rotated ({1, 2, 3, 4, 5}): Approach 1 returns 0 on the first
        iteration via the early exit.
      - rotated n - 1 times ({2, 3, 4, 5, 1}): the minimum is the last
        element; both approaches keep taking low = mid + 1 until low = 4.
      - single element: low == high == 0, the loop never runs -> 0.
*/
