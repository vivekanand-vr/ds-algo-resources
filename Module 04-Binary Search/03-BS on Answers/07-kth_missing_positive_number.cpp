#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an array arr of positive integers sorted in strictly increasing
// order and an integer k, return the k-th positive integer missing from arr.
//
// Example:
// arr = {2, 3, 4, 7, 11},  k = 5   -> 9   (missing: 1 5 6 8 [9] 10 12 ...)
// arr = {1, 2, 3, 4},      k = 2   -> 6   (missing: 5 [6] 7 ...)
// arr = {5, 6, 7},         k = 3   -> 3   (missing: 1 2 [3] 4 8 ...)

/*
    Approach: Binary search on the count of missing numbers

    - With nothing missing, index i would hold i + 1. So the number of
      positives missing BEFORE arr[i] is
          missing(i) = arr[i] - (i + 1).
      It never decreases as i grows, so we can binary search it.
    - Find the first index low with missing(low) >= k. Then the k-th
      missing number lies between arr[low - 1] and arr[low] (or after the
      end, if low == n):
        missing(mid) <  k  ->  fewer than k gaps up to mid, the answer is
                               after arr[mid]: low = mid + 1.
        missing(mid) >= k  ->  the answer is before arr[mid]: high = mid - 1.
    - When the loop ends, high = low - 1 is the last index with fewer than k
      missing. The answer is arr[high] plus the gaps still to cover:
          arr[high] + (k - missing(high))
        = arr[high] + k - arr[high] + high + 1
        = high + 1 + k
        = low + k
      so arr is not even needed for the final step. This also works when
      high = -1 (the answer is before arr[0]) and when low = n.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1.
    2. While low <= high, compute mid = low + (high - low) / 2.
    3. If arr[mid] - (mid + 1) < k, low = mid + 1.
    4. Else high = mid - 1.
    5. Return low + k.

    Time Complexity: O(log n) - a plain binary search over the indices.
    Space Complexity: O(1)
*/
int findKthPositive(vector<int>& arr, int k) {
  int n = arr.size();
  int low = 0, high = n - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow
    int missing = arr[mid] - (mid + 1);  // positives missing before arr[mid]

    if (missing < k) {
      low = mid + 1;  // not enough gaps yet, the answer is further right
    } else {
      high = mid - 1;  // k or more gaps already, the answer is to the left
    }
  }

  return low + k;
}

int main() {
  vector<pair<vector<int>, int>> tests = {{{2, 3, 4, 7, 11}, 5}, {{1, 2, 3, 4}, 2}, {{5, 6, 7}, 3}};

  for (auto& [arr, k] : tests) {
    cout << "Array: ";
    for (int v : arr) cout << v << " ";
    cout << " k = " << k << endl;
    cout << "  k-th missing: " << findKthPositive(arr, k) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of findKthPositive: arr = {2, 3, 4, 7, 11}, k = 5
    (answer = 9)
    ======================================================================

      index:        0    1    2    3    4
      value:        2    3    4    7   11
      expected:     1    2    3    4    5      (i + 1)
      missing:      1    1    1    3    6      (value - expected)
                                        ^
                                        first index with missing >= 5

    Initial state: low = 0, high = 4

    | iter | low | high | mid | arr[mid] | missing | < 5 ? | action    |
    |------|-----|------|-----|----------|---------|-------|-----------|
    |  1   |  0  |  4   |  2  |    4     |    1    |  yes  | low  = 3  |
    |  2   |  3  |  4   |  3  |    7     |    3    |  yes  | low  = 4  |
    |  3   |  4  |  4   |  4  |   11     |    6    |  no   | high = 3  |

      Loop ends with low = 4, high = 3.
      Check: arr[3] = 7 has 3 missing before it, 2 more to go -> 8, 9.
      RETURN low + k = 4 + 5 = 9

    ======================================================================
    Notes
    ======================================================================

    Brute-force alternative (O(n)):
      walk the array and bump k for every arr[i] <= k:
          for (int x : arr) if (x <= k) k++; else break;
          return k;
      Each array value at or below the current candidate pushes the answer
      one further. Simple, but linear - the binary search is O(log n).

    Changes from the original (no bug fixes needed):
      - Pulled arr[mid] - (mid + 1) into a named variable missing, so the
        comparison reads as "fewer than k missing".

    Edge cases:
      - the answer is before arr[0] ({5, 6, 7}, k = 3): every missing(i)
        is >= 3, high walks down to -1, low = 0 -> 0 + 3 = 3.
      - the answer is past the end ({1, 2, 3, 4}, k = 2): nothing is
        missing inside the array, low walks up to n = 4 -> 4 + 2 = 6.
*/
