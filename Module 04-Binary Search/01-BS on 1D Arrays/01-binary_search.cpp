#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array and a target value, return the index of target
// in the array, or -1 if it is not present

/*
    Approach: Binary Search
    - Repeatedly halve the search space by comparing the target with the
      middle element
    - If the middle element equals the target, we're done
    - If the middle element is smaller, the target must be in the right half
    - If the middle element is larger, the target must be in the left half

    Algorithm Steps
    ----------------
    1. Initialize left = 0, right = n - 1
    2. While left <= right, compute mid = left + (right - left) / 2
    3. If arr[mid] == target, return mid
    4. If arr[mid] < target, search the right half (left = mid + 1)
    5. Else search the left half (right = mid - 1)
    6. Return -1 if the loop ends without finding the target

    Time Complexity: O(log n)
    Space Complexity: O(1)
*/
int binarySearch(int target, vector<int> arr) {
  int n = arr.size();
  int left = 0, right = n - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;  // to prevent overflow
    if (arr[mid] == target) {
      return mid;
    } else if (arr[mid] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return -1;
}

int main() {
  vector<int> arr = {-1, 0, 3, 5, 9, 12};
  int target = 9;

  cout << "Array: ";
  for (int x : arr) cout << x << " ";
  cout << endl;

  int result = binarySearch(target, arr);
  cout << "Index of " << target << ": " << result << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {-1, 0, 3, 5, 9, 12}, target = 9   (n = 6, answer = 4)
    ======================================================================

      index:    0    1    2    3    4    5
      value:   -1    0    3    5    9   12

    Tracked state:
      left, right - the inclusive bounds of the part still being searched
      mid         - left + (right - left) / 2, the probe index
      (no candidate is recorded - a hit returns immediately)

    Initial state: left = 0, right = 5

    ----------------------------------------------------------------------
    Iteration 1
      bounds       left = 0, right = 5      (6 candidates alive)
      mid          0 + (5 - 0) / 2 = 2      (integer division)
      a[mid]       arr[2] = 3
      compare      3 != 9, and 3 < 9  -> target must sit to the RIGHT
      discard      indices 0..2 (arr[0..2] are all <= 3 < 9)
      update       left = mid + 1 = 3, right stays 5

                    -1   0   3   5   9  12
                   [ x   x   x |  .   .   . ]
                                ^--- live range is now 3..5

    ----------------------------------------------------------------------
    Iteration 2
      bounds       left = 3, right = 5      (3 candidates alive)
      mid          3 + (5 - 3) / 2 = 4
      a[mid]       arr[4] = 9
      compare      9 == 9  -> HIT
      return       mid = 4 immediately, no further work

                    -1   0   3   5   9  12
                    x   x   x   .   ^   .
                                    mid = 4  <-- found here

    ----------------------------------------------------------------------
    RETURN 4        (arr[4] == 9)

    ======================================================================
    Summary table
    ======================================================================

    | iter | left | right | mid | a[mid] | compare | action        |
    |------|------|-------|-----|--------|---------|---------------|
    |  1   |  0   |   5   |  2  |   3    | 3 < 9   | left = 3      |
    |  2   |  3   |   5   |  4  |   9    | 9 == 9  | return 4      |

    Step count behind the O(log n) claim:
      6 elements needed 2 probes; the live range went 6 -> 3 -> hit. Even
      the worst case (target absent) would take only ceil(log2(6+1)) = 3
      probes: 6 -> 3 -> 1 -> 0 candidates.

    The teaching point - contrast with floor/ceil/insert-position:
      plain search RETURNS ON THE HIT and needs no memory of anything it
      passed. There is no `ans` variable, and when the loop ends normally
      (left > right) the only possible report is -1. The floor, ceil and
      insert-position variants cannot do this: on a non-hit they have to
      RECORD the current mid as a candidate and keep shrinking, because
      the value they must report is the best near-miss, not an exact hit.
*/
