#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: (GfG K-th Element of Two Sorted Arrays) Given two sorted arrays a (size
// n) and b (size m) and an integer k (1 <= k <= n + m), return the element
// that would be at position k (1-based) if both arrays were merged into one
// sorted array. Do it without actually merging.
//
// Example:
// a = {2, 3, 6, 7, 9},             b = {1, 4, 8, 10},  k = 5  -> 6
// a = {100, 112, 256, 349, 770},
// b = {72, 86, 113, 119, 265, 445, 892},               k = 7  -> 256
// a = {1, 3},                      b = {2},            k = 3  -> 3

/*
    Approach: Binary search on how many elements come from a

    - The first k elements of the merged array are some prefix of a plus
      some prefix of b. If cutA elements come from a, then cutB = k - cutA
      come from b. Only cutA needs to be found.
    - Around the cuts there are four elements:
          aLeft  = a[cutA - 1]    aRight = a[cutA]
          bLeft  = b[cutB - 1]    bRight = b[cutB]
      (a missing element is -infinity on the left, +infinity on the right).
      The split is valid when everything on the left is <= everything on
      the right. Each array is sorted on its own, so only the two cross
      checks are needed:
          aLeft <= bRight  and  bLeft <= aRight.
      Then the k-th element is the largest one on the left, max(aLeft, bLeft).
    - If aLeft > bRight, too many elements came from a: high = cutA - 1.
      Otherwise bLeft > aRight, too few came from a: low = cutA + 1.
    - cutA ranges over max(0, k - m)..min(k, n): at most n elements can come
      from a, at most k in total, and b can supply at most m, so at least
      k - m must come from a.
    - Searching the smaller array keeps the range as small as possible, so
      the function swaps the arrays when a is the longer one.

    Algorithm Steps
    ----------------
    1. If a is longer than b, call kthElement(b, a, k).
    2. Set low = max(0, k - m), high = min(k, n).
    3. While low <= high:
         cutA = low + (high - low) / 2, cutB = k - cutA.
         Read aLeft, aRight, bLeft, bRight (INT_MIN / INT_MAX at the ends).
    4. If aLeft <= bRight and bLeft <= aRight, return max(aLeft, bLeft).
    5. Else if aLeft > bRight, high = cutA - 1.
    6. Else low = cutA + 1.
    7. (Unreachable for a valid k) return -1.

    Time Complexity: O(log(min(n, m))) - binary search over the cuts of
                      the smaller array.
    Space Complexity: O(1)
*/
int kthElement(vector<int>& a, vector<int>& b, int k) {
  if (a.size() > b.size()) {
    return kthElement(b, a, k);  // always search the smaller array
  }

  int n = a.size();
  int m = b.size();

  int low = max(0, k - m);  // b can give at most m, the rest must come from a
  int high = min(k, n);     // a can give at most n, and never more than k

  while (low <= high) {
    int cutA = low + (high - low) / 2;  // elements taken from a
    int cutB = k - cutA;                // elements taken from b

    int aLeft = (cutA == 0) ? INT_MIN : a[cutA - 1];
    int aRight = (cutA == n) ? INT_MAX : a[cutA];

    int bLeft = (cutB == 0) ? INT_MIN : b[cutB - 1];
    int bRight = (cutB == m) ? INT_MAX : b[cutB];

    if (aLeft <= bRight && bLeft <= aRight) {
      return max(aLeft, bLeft);  // valid split, largest on the left side
    } else if (aLeft > bRight) {
      high = cutA - 1;  // took too many from a
    } else {
      low = cutA + 1;  // took too few from a
    }
  }

  return -1;  // only reached when k is out of range
}

struct Test {
  vector<int> a, b;
  int k;
};

int main() {
  vector<Test> tests = {{{2, 3, 6, 7, 9}, {1, 4, 8, 10}, 5},
                        {{100, 112, 256, 349, 770}, {72, 86, 113, 119, 265, 445, 892}, 7},
                        {{1, 3}, {2}, 3}};

  for (auto& [a, b, k] : tests) {
    cout << "a: ";
    for (int v : a) cout << v << " ";
    cout << " b: ";
    for (int v : b) cout << v << " ";
    cout << " k = " << k << endl;
    cout << "  kth element: " << kthElement(a, b, k) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of kthElement: a = {100, 112, 256, 349, 770},
                           b = {72, 86, 113, 119, 265, 445, 892}, k = 7
    (answer = 256)
    ======================================================================

    Sizes:           n = 5 <= m = 7, so no swap
    Initial state:   low = max(0, 7 - 7) = 0, high = min(7, 5) = 5

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 5
      cuts         cutA = 0 + (5 - 0) / 2 = 2, cutB = 7 - 2 = 5
      left side    a: 100 112              b: 72 86 113 119 265
      neighbours   aLeft = 112, aRight = 256, bLeft = 265, bRight = 445
      check        112 <= 445 ok, 265 <= 256 NO
      update       too few from a  ->  low = 3

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 3, high = 5
      cuts         cutA = 3 + (5 - 3) / 2 = 4, cutB = 7 - 4 = 3
      left side    a: 100 112 256 349      b: 72 86 113
      neighbours   aLeft = 349, aRight = 770, bLeft = 113, bRight = 119
      check        349 <= 119 NO
      update       too many from a  ->  high = 3

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 3, high = 3
      cuts         cutA = 3, cutB = 7 - 3 = 4
      left side    a: 100 112 256          b: 72 86 113 119
      neighbours   aLeft = 256, aRight = 349, bLeft = 119, bRight = 265
      check        256 <= 265 ok, 119 <= 349 ok  ->  valid split
      RETURN       max(256, 119) = 256

    Check: merged = 72 86 100 112 113 119 [256] 265 349 ..., 7th is 256.

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | cutA | cutB | aLeft | aRight | bLeft | bRight | action     |
    |------|-----|------|------|------|-------|--------|-------|--------|------------|
    |  1   |  0  |  5   |  2   |  5   |  112  |  256   |  265  |  445   | low  = 3   |
    |  2   |  3  |  5   |  4   |  3   |  349  |  770   |  113  |  119   | high = 3   |
    |  3   |  3  |  3   |  3   |  4   |  256  |  349   |  119  |  265   | return 256 |

    ======================================================================
    Notes
    ======================================================================

    Why cutA must be a midpoint:
      with cutA = low the loop still finds the answer, but it walks the
      range one step at a time - an O(n) linear scan, not a binary search.
      Taking the midpoint halves the range on every probe.

    Why the lower bound is max(0, k - m) and not 0:
      with cutA < k - m we would need cutB > m elements from b, which do not
      exist. Starting at k - m keeps cutB inside b.

    INT_MIN / INT_MAX sentinels:
      an empty left side can never be the maximum and an empty right side
      can never fail a <= check, so the edge cuts need no special case.

    Median of two sorted arrays:
      13-median_of_2_sorted_arrays.cpp uses the same partition with k fixed
      at (n + m + 1) / 2, and reads both middle elements from one valid
      split instead of calling this function twice.

    Edge cases:
      - k == 1: the smaller of a[0] and b[0].
      - k == n + m: the larger of the two last elements.
      - one array empty: after the swap it is a, so n = 0 gives
        low = high = 0, cutA = 0, and the answer is b[k - 1].
*/
