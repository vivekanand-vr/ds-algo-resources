#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: A peak element is one that is strictly greater than its neighbours. Given
// an array where no two ADJACENT elements are equal, return the index of ANY
// peak. Treat arr[-1] and arr[n] as -infinity, so the first and last elements
// only need to beat their one real neighbour. Must run in O(log n).
//
// Example:
// arr = {1, 2, 3, 1}              -> 2
// arr = {1, 2, 1, 3, 5, 6, 4}     -> 5   (index 1 is also a valid answer)
// arr = {1, 2, 3, 4, 5}           -> 4   (strictly rising: the last is a peak)
// arr = {5, 4, 3, 2, 1}           -> 0   (strictly falling: the first is a peak)
// arr = {1}                       -> 0

/*
    Approach 1: Binary search on the slope - always climb uphill

    - The array is NOT sorted, so there is no target to compare against.
      What mid can tell us instead is the slope at mid, by looking at
      arr[mid + 1]:
        arr[mid] < arr[mid + 1]  ->  rising at mid. Keep walking right and
                                      the values either start falling (that
                                      turning point is a peak) or keep rising
                                      to the end (the last element is a peak,
                                      since arr[n] = -inf). Either way a peak
                                      exists in mid+1..high: low = mid + 1.
        arr[mid] > arr[mid + 1]  ->  falling at mid. By the mirror argument
                                      a peak exists in low..mid, and mid
                                      itself may be it: high = mid.
    - The invariant behind it: the element just left of the range is smaller
      than arr[low], and the element just right of it is smaller than
      arr[high] (initially both are the -inf sentinels). Each branch keeps
      that true. When low == high, the single element beats both of its
      neighbours - it is a peak.
    - This is why binary search works on an unsorted array: it is not
      searching for a value, it is halving a range that is GUARANTEED to
      still contain a peak.
    - arr[mid + 1] is always in bounds: low < high gives mid < high <= n - 1.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1.
    2. While low < high, compute mid = low + (high - low) / 2.
    3. If arr[mid] < arr[mid + 1], a peak lies right of mid: low = mid + 1.
    4. Else a peak lies at mid or to its left: high = mid.
    5. Return low.

    Time Complexity: O(log n) - every iteration halves the range.
    Space Complexity: O(1) - three indices.
*/
int findPeakElement(vector<int>& arr) {
  int n = arr.size();
  int low = 0, high = n - 1;

  while (low < high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (arr[mid] < arr[mid + 1]) {
      low = mid + 1;  // uphill to the right, so a peak is somewhere past mid
    } else {
      high = mid;  // downhill to the right, so mid or something left is a peak
    }
  }

  return low;
}

/*
    Approach 2: Linear scan for the first descent

    - Walk left to right and return the first i where arr[i] > arr[i + 1].
      Everything before i was rising (otherwise an earlier i would have been
      returned), so arr[i - 1] < arr[i] as well - arr[i] is a peak.
    - If no descent exists, the array rises all the way and the last element
      is a peak because arr[n] = -inf.
    - Simple and obviously correct, but O(n), so it misses the required
      bound. Useful as a reference to test Approach 1 against, bearing in
      mind the two may return DIFFERENT peaks - both are valid.

    Algorithm Steps
    ----------------
    1. For i = 0 .. n - 2: if arr[i] > arr[i + 1], return i.
    2. Return n - 1.

    Time Complexity: O(n) - one pass in the worst case (a rising array).
    Space Complexity: O(1)
*/
int findPeakElementLinear(vector<int>& arr) {
  int n = arr.size();

  for (int i = 0; i < n - 1; i++) {
    if (arr[i] > arr[i + 1]) {
      return i;  // first descent: rising before it, falling after it
    }
  }

  return n - 1;  // never descended, so the last element is the peak
}

int main() {
  vector<vector<int>> tests = {{1, 2, 3, 1}, {1, 2, 1, 3, 5, 6, 4}, {1, 2, 3, 4, 5}, {5, 4, 3, 2, 1}, {1}};

  for (auto& arr : tests) {
    cout << "Array: ";
    for (int v : arr) cout << v << " ";
    cout << endl;
    cout << "  peak index (binary search): " << findPeakElement(arr) << endl;
    cout << "  peak index (linear scan):   " << findPeakElementLinear(arr) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of findPeakElement (Approach 1): arr = {1, 2, 1, 3, 5, 6, 4}
    (n = 7, peaks at index 1 and index 5)
    ======================================================================

      index:    0    1    2    3    4    5    6
      value:    1    2    1    3    5    6    4
                     ^                   ^
                   peak                peak

    Tracked state:
      low, high - the inclusive bounds; always contain at least one peak
      mid       - low + (high - low) / 2, the probe index

    Initial state: low = 0, high = 6

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 6      (7 candidates alive)
      mid          0 + (6 - 0) / 2 = 3
      slope        arr[3] = 3  vs  arr[4] = 5  ->  3 < 5, RISING
      conclude     climbing right from 3 must reach a peak before the end
                   (or at the end); 0..3 is dropped even though it holds the
                   peak at index 1 - only ONE peak is needed
      update       low = mid + 1 = 4

                     1   2   1   3 | 5   6   4
                   [ x   x   x   x | .   .   . ]
                                     live range is now 4..6

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 4, high = 6      (3 candidates alive)
      mid          4 + (6 - 4) / 2 = 5
      slope        arr[5] = 6  vs  arr[6] = 4  ->  6 > 4, FALLING
      conclude     a peak is at 5 or left of it; keep mid
      update       high = mid = 5

                     1   2   1   3   5   6 | 4
                   [ x   x   x   x   .   . | x ]
                                     live range is now 4..5

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 4, high = 5      (2 candidates alive)
      mid          4 + (5 - 4) / 2 = 4
      slope        arr[4] = 5  vs  arr[5] = 6  ->  5 < 6, RISING
      update       low = mid + 1 = 5

    ----------------------------------------------------------------------
    Loop ends: low = high = 5
    RETURN 5      (arr[5] = 6 > arr[4] = 5 and > arr[6] = 4)

    ======================================================================
    Summary table (Approach 1)
    ======================================================================

    | iter | low | high | mid | arr[mid] | arr[mid+1] | slope   | update   |
    |------|-----|------|-----|----------|------------|---------|----------|
    |  1   |  0  |  6   |  3  |    3     |     5      | rising  | low = 4  |
    |  2   |  4  |  6   |  5  |    6     |     4      | falling | high = 5 |
    |  3   |  4  |  5   |  4  |    5     |     6      | rising  | low = 5  |

    ======================================================================
    DRY RUN of findPeakElementLinear (Approach 2): same array
    ======================================================================

      i = 0:  arr[0] = 1 > arr[1] = 2 ?  no
      i = 1:  arr[1] = 2 > arr[2] = 1 ?  YES  ->  return 1

    RETURN 1 - a different peak from Approach 1's 5, and equally correct.
    The linear scan always finds the LEFTMOST peak; the binary search finds
    whichever peak its halving happens to steer toward.

    ======================================================================
    Notes
    ======================================================================

    Why "no two adjacent elements are equal" matters:
      with a plateau like {1, 2, 2, 2, 1}, arr[mid] == arr[mid + 1] says
      nothing about which side a strict peak is on - there may not even be
      one. The else branch here would treat equality as falling, which is
      not justified. The problem's guarantee is what makes the slope test
      exact.

    Why high = mid and not mid - 1:
      falling at mid means arr[mid] > arr[mid + 1], so mid already beats its
      right neighbour and might be the peak itself. Discarding it could
      discard the only peak in range. Keeping mid is also why the loop runs
      while low < high.

    Why this is not a contradiction of "binary search needs sorted input":
      binary search needs a test at mid that tells which half still holds an
      answer. Sortedness is one way to get that test; the slope plus the
      -inf sentinels is another.

    Edge cases:
      - single element: low == high == 0, the loop never runs -> 0.
      - strictly rising ({1, 2, 3, 4, 5}): every probe rises, low walks to
        n - 1.
      - strictly falling ({5, 4, 3, 2, 1}): every probe falls, high walks to
        0.
*/
