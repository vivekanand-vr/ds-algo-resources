#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: (LeetCode 4) Given two sorted arrays a (size n) and b (size m), return
// the MEDIAN of the two arrays combined. The overall run time should be
// O(log(n + m)).
//   - odd total length:  the middle element of the merged array.
//   - even total length: the average of the two middle elements.
//
// Example:
// a = {1, 3},             b = {2}                 -> 2.0   (merged 1 2 3)
// a = {1, 2},             b = {3, 4}              -> 2.5   ((2 + 3) / 2)
// a = {},                 b = {1}                 -> 1.0
// a = {1, 3, 8, 9, 15},   b = {7, 11, 18, 19, 21} -> 10.0  ((9 + 11) / 2)

/*
    Approach: Binary search on the partition of the smaller array

    - Split the merged array into a LEFT half and a RIGHT half, with the
      left half holding leftSize = (n + m + 1) / 2 elements (one extra
      when the total is odd). If cutA elements of the left half come from
      a, then cutB = leftSize - cutA come from b. Only cutA needs to be
      found.
    - Around the cuts there are four elements:
          aLeft  = a[cutA - 1]    aRight = a[cutA]
          bLeft  = b[cutB - 1]    bRight = b[cutB]
      (a missing element is -infinity on the left, +infinity on the right).
      The split is valid when everything on the left is <= everything on
      the right. Each array is sorted on its own, so only the two cross
      checks are needed:
          aLeft <= bRight  and  bLeft <= aRight.
    - Once the split is valid the median comes straight from the edges:
        total odd  ->  max(aLeft, bLeft)          (the extra left element)
        total even ->  (max(aLeft, bLeft) + min(aRight, bRight)) / 2
    - If aLeft > bRight, too many elements came from a: high = cutA - 1.
      Otherwise bLeft > aRight, too few came from a: low = cutA + 1.
    - Searching the smaller array (swap if a is longer) lets cutA range over
      the whole 0..n while cutB always stays inside 0..m.
    - This is the same partition idea as 14-kth_element_of_2_sorted_arrays.cpp
      with k fixed at leftSize, but one search gives BOTH middle elements,
      so no separate kthElement calls are needed.

    Algorithm Steps
    ----------------
    1. If a is longer than b, call findMedianSortedArrays(b, a).
    2. leftSize = (n + m + 1) / 2, low = 0, high = n.
    3. While low <= high:
         cutA = low + (high - low) / 2, cutB = leftSize - cutA.
         Read aLeft, aRight, bLeft, bRight (INT_MIN / INT_MAX at the ends).
    4. If aLeft <= bRight and bLeft <= aRight:
         odd total  -> return max(aLeft, bLeft).
         even total -> return (max(aLeft, bLeft) + min(aRight, bRight)) / 2.0.
    5. Else if aLeft > bRight, high = cutA - 1.
    6. Else low = cutA + 1.

    Time Complexity: O(log(min(n, m))) - one binary search over the smaller
                      array.
    Space Complexity: O(1)
*/
double findMedianSortedArrays(vector<int>& a, vector<int>& b) {
  if (a.size() > b.size()) {
    return findMedianSortedArrays(b, a);  // always search the smaller array
  }

  int n = a.size();
  int m = b.size();
  int leftSize = (n + m + 1) / 2;  // left half gets the extra one when odd

  int low = 0, high = n;

  while (low <= high) {
    int cutA = low + (high - low) / 2;  // left-half elements taken from a
    int cutB = leftSize - cutA;         // left-half elements taken from b

    int aLeft = (cutA == 0) ? INT_MIN : a[cutA - 1];
    int aRight = (cutA == n) ? INT_MAX : a[cutA];

    int bLeft = (cutB == 0) ? INT_MIN : b[cutB - 1];
    int bRight = (cutB == m) ? INT_MAX : b[cutB];

    if (aLeft <= bRight && bLeft <= aRight) {
      if ((n + m) % 2 == 1) {
        return max(aLeft, bLeft);  // the single middle
      }
      // average as double, no int overflow
      return ((double)max(aLeft, bLeft) + min(aRight, bRight)) / 2.0;
    } else if (aLeft > bRight) {
      high = cutA - 1;  // took too many from a
    } else {
      low = cutA + 1;  // took too few from a
    }
  }

  return 0.0;  // unreachable for sorted input
}

int main() {
  vector<pair<vector<int>, vector<int>>> tests = {
      {{1, 3}, {2}}, {{1, 2}, {3, 4}}, {{}, {1}}, {{1, 3, 8, 9, 15}, {7, 11, 18, 19, 21}}};

  for (auto& [a, b] : tests) {
    cout << "a: ";
    for (int v : a) cout << v << " ";
    cout << " b: ";
    for (int v : b) cout << v << " ";
    cout << endl;
    cout << "  median: " << findMedianSortedArrays(a, b) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of findMedianSortedArrays: a = {1, 3, 8, 9, 15},
                                       b = {7, 11, 18, 19, 21}
    (answer = 10.0)
    ======================================================================

    Sizes:           n = 5 <= m = 5, so no swap
    Initial state:   leftSize = (5 + 5 + 1) / 2 = 5, low = 0, high = 5

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 5
      cuts         cutA = 0 + (5 - 0) / 2 = 2, cutB = 5 - 2 = 3
      left half    a: 1 3                  b: 7 11 18
      neighbours   aLeft = 3, aRight = 8, bLeft = 18, bRight = 19
      check        3 <= 19 ok, 18 <= 8 NO
      update       too few from a  ->  low = 3

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 3, high = 5
      cuts         cutA = 3 + (5 - 3) / 2 = 4, cutB = 5 - 4 = 1
      left half    a: 1 3 8 9              b: 7
      neighbours   aLeft = 9, aRight = 15, bLeft = 7, bRight = 11
      check        9 <= 11 ok, 7 <= 15 ok  ->  valid split
      total 10     even  ->  (max(9, 7) + min(15, 11)) / 2.0
      RETURN       (9 + 11) / 2.0 = 10.0

    Check: merged = 1 3 7 8 [9 11] 15 18 19 21, middles 9 and 11.

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | cutA | cutB | aLeft | aRight | bLeft | bRight | action      |
    |------|-----|------|------|------|-------|--------|-------|--------|-------------|
    |  1   |  0  |  5   |  2   |  3   |   3   |   8    |  18   |   19   | low = 3     |
    |  2   |  3  |  5   |  4   |  1   |   9   |   15   |   7   |   11   | return 10.0 |

    ======================================================================
    Notes
    ======================================================================

    Why leftSize = (n + m + 1) / 2:
      for an even total it is exactly half; for an odd total the left half
      gets the extra element, so the median is simply the largest element
      on the left - no separate case for which half holds it.

    Why high = n with no max/min clamp:
      a is the smaller array, so n <= leftSize <= m. Any cutA in 0..n then
      gives cutB = leftSize - cutA inside 0..m, and b always has enough
      elements to fill the rest of the left half.

    Compared with using kthElement twice:
      14-kth_element_of_2_sorted_arrays.cpp finds one order statistic per
      search, so the even case needs two searches. Here one valid split
      exposes both middles at once - max of the left and min of the right.

    INT_MIN / INT_MAX sentinels:
      an empty left side can never be the maximum and an empty right side
      can never be the minimum or fail a <= check, so the edge cuts need no
      special case.

    Averaging safely:
      one side is cast to double before adding, so two values near INT_MAX
      cannot overflow. With LeetCode's limits (|value| <= 10^6) int would
      also be fine.

    Edge cases:
      - one array empty: after the swap it is a, so low = high = 0,
        cutA = 0, and the median comes straight from b (example 3).
      - all of a smaller than all of b: the valid cut is cutA = n, and the
        +infinity sentinel stands in for aRight.
*/
