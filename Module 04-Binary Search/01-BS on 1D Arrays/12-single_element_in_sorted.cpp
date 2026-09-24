#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array in which every element appears exactly TWICE except
// one element that appears once, return that single element in O(log n) time
// and O(1) space.
//
// Example:
// arr = {1, 1, 2, 3, 3, 4, 4, 8, 8}  -> 2
// arr = {3, 3, 7, 7, 10, 11, 11}     -> 10
// arr = {1, 1, 2}                    -> 2    (single element at the end)
// arr = {1}                          -> 1

/*
    Approach 1: Snap mid to an even index and check whether its pair is intact

    - Since the array is sorted, equal values sit next to each other. Before
      the single element, every pair starts at an EVEN index:
      (0,1), (2,3), ... After it, everything is shifted by one and every pair
      starts at an ODD index.

        index:   0  1  2  3  4  5  6  7  8
        value:   1  1  2  3  3  4  4  8  8
                 \__/  ^  \__/ \__/ \__/
               even-start |  odd-start pairs
                        single

    - So looking at an even index i answers the question "is the single
      element before or after i": if arr[i] == arr[i + 1], the pairing is
      still aligned at i and the single element is strictly to the right of
      i + 1; otherwise the alignment has already broken and the single
      element is at i or to its left.
    - mid is snapped to even with mid-- when it is odd. low only ever moves
      to mid + 2 and high to mid, so both stay even and the answer always
      sits at an even index inside low..high.
    - The loop runs while low < high and keeps mid on the else branch
      (high = mid), because mid may be the single element itself. When the
      range shrinks to one index, that index is the answer.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1 (n is odd, so high is even).
    2. While low < high, compute mid = low + (high - low) / 2, and if mid is
       odd, decrement it.
    3. If arr[mid] == arr[mid + 1], the pair is intact: low = mid + 2.
    4. Else the single element is at mid or before it: high = mid.
    5. Return arr[low].

    Time Complexity: O(log n) - every iteration halves the range.
    Space Complexity: O(1) - three indices.
*/
int singleNonDuplicate(vector<int>& arr) {
  int n = arr.size();
  int low = 0, high = n - 1;

  while (low < high) {
    int mid = low + (high - low) / 2;  // to prevent overflow
    if (mid & 1) {
      mid--;  // snap to the even index, where a pair should start
    }

    if (mid < n - 1 && arr[mid] == arr[mid + 1]) {
      low = mid + 2;  // pair (mid, mid+1) is intact, the single one is further right
    } else {
      high = mid;  // alignment already broken, the single one is at mid or left
    }
  }

  return arr[low];
}

/*
    Approach 2: Compare mid with its partner mid ^ 1, no snapping

    - The same parity argument, expressed with one XOR. For any index i,
      i ^ 1 is the index its partner SHOULD be at if the pairing is aligned:
        i even  ->  i ^ 1 = i + 1
        i odd   ->  i ^ 1 = i - 1
    - So arr[mid] == arr[mid ^ 1] means "mid is paired correctly", and that
      is only possible to the LEFT of the single element. The answer is then
      right of mid: low = mid + 1. Otherwise the answer is at mid or left of
      it: high = mid.
    - This removes the odd/even branch entirely. It is the same algorithm
      as Approach 1 with the snapping folded into the partner index.
    - mid ^ 1 is always in range: low < high gives mid < high, so for an
      even mid, mid + 1 <= high; for an odd mid, mid - 1 >= 0.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1.
    2. While low < high, compute mid = low + (high - low) / 2.
    3. If arr[mid] == arr[mid ^ 1], low = mid + 1; else high = mid.
    4. Return arr[low].

    Time Complexity: O(log n)
    Space Complexity: O(1)
*/
int singleNonDuplicateXorPartner(vector<int>& arr) {
  int low = 0, high = arr.size() - 1;

  while (low < high) {
    int mid = low + (high - low) / 2;

    if (arr[mid] == arr[mid ^ 1]) {
      low = mid + 1;  // mid is correctly paired, so the single one lies to the right
    } else {
      high = mid;
    }
  }

  return arr[low];
}

int main() {
  vector<vector<int>> tests = {{1, 1, 2, 3, 3, 4, 4, 8, 8}, {3, 3, 7, 7, 10, 11, 11}, {1, 1, 2}, {1}};

  for (auto& arr : tests) {
    cout << "Array: ";
    for (int v : arr) cout << v << " ";
    cout << endl;
    cout << "  single (snap mid to even): " << singleNonDuplicate(arr) << endl;
    cout << "  single (mid ^ 1 partner):  " << singleNonDuplicateXorPartner(arr) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of singleNonDuplicate (Approach 1):
    arr = {1, 1, 2, 3, 3, 4, 4, 8, 8}   (n = 9, answer = 2 at index 2)
    ======================================================================

      index:    0    1    2    3    4    5    6    7    8
      value:    1    1    2    3    3    4    4    8    8

    Tracked state:
      low, high - the inclusive bounds, both always even
      mid       - the probe, snapped to even

    Initial state: low = 0, high = 8

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 8      (5 even candidates: 0,2,4,6,8)
      mid          0 + (8 - 0) / 2 = 4    (already even)
      pair test    arr[4] = 3  vs  arr[5] = 4  ->  NOT equal
      conclude     index 4 should start a pair but does not, so the
                   alignment broke at or before 4
      update       high = mid = 4

                     1   1   2   3   3 | 4   4   8   8
                   [ .   .   .   .   . | x   x   x   x ]
                                         live range is now 0..4

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 0, high = 4
      mid          0 + (4 - 0) / 2 = 2    (already even)
      pair test    arr[2] = 2  vs  arr[3] = 3  ->  NOT equal
      update       high = mid = 2

                     1   1   2 | 3   3   4   4   8   8
                   [ .   .   . | x   x   x   x   x   x ]
                                 live range is now 0..2

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 0, high = 2
      mid          0 + (2 - 0) / 2 = 1    ->  odd, snap to 0
      pair test    arr[0] = 1  vs  arr[1] = 1  ->  equal, pair intact
      conclude     pairing is aligned up to index 1, the single element is
                   at index 2 or later
      update       low = mid + 2 = 2

                     1   1 | 2 | 3   3   4   4   8   8
                   [ x   x | . | x   x   x   x   x   x ]

    ----------------------------------------------------------------------
    Loop ends: low = high = 2
    RETURN arr[2] = 2

    ======================================================================
    Summary table (Approach 1)
    ======================================================================

    | iter | low | high | mid (raw -> snapped) | arr[mid], arr[mid+1] | update   |
    |------|-----|------|----------------------|----------------------|----------|
    |  1   |  0  |  8   |       4 -> 4         |        3, 4          | high = 4 |
    |  2   |  0  |  4   |       2 -> 2         |        2, 3          | high = 2 |
    |  3   |  0  |  2   |       1 -> 0         |        1, 1          | low = 2  |

    ======================================================================
    DRY RUN of singleNonDuplicateXorPartner (Approach 2): same array
    ======================================================================

    | iter | low | high | mid | mid ^ 1 | arr[mid], arr[mid^1] | update   |
    |------|-----|------|-----|---------|----------------------|----------|
    |  1   |  0  |  8   |  4  |    5    |        3, 4          | high = 4 |
    |  2   |  0  |  4   |  2  |    3    |        2, 3          | high = 2 |
    |  3   |  0  |  2   |  1  |    0    |        1, 1          | low = 2  |

      Iteration 3 is where the two approaches differ: Approach 1 snaps mid
      from 1 to 0 and compares forward; Approach 2 keeps mid = 1 and compares
      backward with index 0. It is the same pair, looked at from its other
      end.

    RETURN arr[2] = 2      (agrees with Approach 1)

    ======================================================================
    Notes
    ======================================================================

    Why low = mid + 2 and not mid + 1 (Approach 1):
      mid + 1 is the second half of an intact pair, so it cannot be the
      single element either. Jumping by 2 also keeps low even, which is what
      lets every later mid be snapped consistently.

    The mid < n - 1 guard is never actually needed:
      low and high both stay even, and low < high means mid <= high - 2
      after snapping, so mid + 1 <= high - 1 < n. It is a harmless defensive
      check and was kept as written.

    Why this needs the array to be sorted:
      the parity argument depends on equal values being ADJACENT. In an
      unsorted array like {1, 2, 1, 3, 3} the pairs are not contiguous and
      nothing can be concluded from a neighbour.

    The O(n) alternative - XOR of everything:
      x ^ x = 0 and x ^ 0 = x, so XOR-ing the whole array cancels every
      pair and leaves the single element. It does not need the array to be
      sorted, but it reads every element, so it misses the O(log n) target:

          int res = 0;
          for (int v : arr) res ^= v;
          return res;

    Edge cases:
      - single element {1}: low == high == 0, the loop never runs.
      - single element first {2, 3, 3}: iteration 1 has mid = 0 (after
        snapping), arr[0] != arr[1] -> high = 0.
      - single element last {1, 1, 2}: mid = 0, arr[0] == arr[1] -> low = 2.
*/
