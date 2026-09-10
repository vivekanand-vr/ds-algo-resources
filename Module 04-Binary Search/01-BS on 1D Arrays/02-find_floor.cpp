#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given a sorted array and a value x, find the index of the floor of x
// (the largest element in the array that is <= x)

/*
    Approach: Binary Search
    - Binary search for x while tracking the best candidate seen so far
    - Whenever arr[mid] <= x, it's a valid floor candidate, so record it and
      search the right half for something closer to x
    - Whenever arr[mid] > x, it can't be the floor, so search the left half

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = n - 1, ans = -1
    2. While l <= r, compute mid = l + (r - l) / 2
    3. If arr[mid] == x, return mid (exact match is the floor)
    4. If arr[mid] < x, update ans = mid and search right (l = mid + 1)
    5. Else search left (r = mid - 1)
    6. Return ans (-1 if no floor exists)

    Time Complexity: O(log n)
    Space Complexity: O(1)
*/
int findFloor(vector<int>& arr, int x) {
    int n = arr.size();
    int l = 0, r = n - 1;
    int ans = -1;   // store floor index (default -1 if no floor exists)

    while (l <= r) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == x) {
            return mid;  // exact match is the floor
        }
        else if (arr[mid] < x) {
            ans = mid;   // arr[mid] is a candidate for floor
            l = mid + 1; // try to find closer floor on right
        }
        else {
            r = mid - 1; // move left since arr[mid] > x
        }
    }

    return ans; // returns index of floor element OR -1 if none found
}

int main() {
    vector<int> arr = {1, 2, 8, 10, 11, 12, 19};
    int x = 5;

    cout << "Array: ";
    for (int n : arr) cout << n << " ";
    cout << endl;

    int idx = findFloor(arr, x);
    cout << "Floor of " << x << ": ";
    if (idx == -1) cout << "does not exist" << endl;
    else cout << arr[idx] << " (index " << idx << ")" << endl;

    return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {1, 2, 8, 10, 11, 12, 19}, x = 5
             (n = 7, answer = index 1, value 2)
    ======================================================================

      index:    0    1    2    3    4    5    6
      value:    1    2    8   10   11   12   19
                     ^
                     floor of 5 - note 5 is NOT in the array, which is
                     exactly the case that makes `ans` necessary

    Tracked state:
      l, r  - the inclusive bounds of the part still being searched
      mid   - l + (r - l) / 2, the probe index
      ans   - index of the best floor candidate seen so far, start -1.
              A candidate is any mid with arr[mid] < x; each new one is
              further right, hence closer to x, so it overwrites the old.

    Initial state: l = 0, r = 6, ans = -1

    ----------------------------------------------------------------------
    Iteration 1
      bounds       l = 0, r = 6
      mid          0 + (6 - 0) / 2 = 3
      a[mid]       arr[3] = 10
      compare      10 > 5  -> too big, cannot be a floor of 5
      record       nothing; ans stays -1
      discard      indices 3..6 (sorted, so all >= 10 > 5)
      update       r = mid - 1 = 2

                    1   2   8  10  11  12  19
                  [ .   .   . | x   x   x   x ]
                               live range now 0..2

    ----------------------------------------------------------------------
    Iteration 2
      bounds       l = 0, r = 2
      mid          0 + (2 - 0) / 2 = 1
      a[mid]       arr[1] = 2
      compare      2 < 5  -> valid floor candidate
      record       ans = mid = 1                       <-- first candidate
      discard      indices 0..1 as candidates: anything left of mid is
                   <= 2, so it can only be a WORSE floor
      update       l = mid + 1 = 2

    ----------------------------------------------------------------------
    Iteration 3
      bounds       l = 2, r = 2      (one element left: arr[2] = 8)
      mid          2 + (2 - 2) / 2 = 2
      a[mid]       arr[2] = 8
      compare      8 > 5  -> too big
      record       nothing; ans stays 1
      update       r = mid - 1 = 1

                    1   2   8  10  11  12  19
                        ^   ^
                        r   l          r < l -> range is empty
      loop test    l = 2 > r = 1  ->  while (l <= r) fails, loop ends

    ----------------------------------------------------------------------
    RETURN ans = 1        (arr[1] = 2, the largest element <= 5)

    ======================================================================
    Summary table
    ======================================================================

    | iter | l | r | mid | a[mid] | vs x = 5 | ans | half discarded |
    |------|---|---|-----|--------|----------|-----|----------------|
    |  1   | 0 | 6 |  3  |   10   | 10 > 5   | -1  | right (3..6)   |
    |  2   | 0 | 2 |  1  |    2   |  2 < 5   |  1  | left  (0..1)   |
    |  3   | 2 | 2 |  2  |    8   |  8 > 5   |  1  | right (2..2)   |
    |      | 2 | 1 |  -  |    -   | l > r    |  1  | loop ends      |

    Step count behind the O(log n) claim:
      7 elements, 3 probes; the live range shrank 7 -> 3 -> 1 -> 0, which
      is ceil(log2(7 + 1)) = 3 halvings.

    The teaching point - why `ans` exists here and not in plain search:
      x = 5 is absent, so the loop never hits the arr[mid] == x return and
      instead runs until l > r. At that moment nothing about the final l
      or r is looked at - the answer lives in `ans`, recorded back at
      iteration 2 and never improved on. Plain binary search can return
      on the hit and report -1 otherwise; floor must remember its best
      near-miss while it keeps searching right for a better one.
      (Here ans also equals r, but that is a coincidence of this input,
      not something the code relies on.)
      If x were smaller than every element, e.g. x = 0, no mid would ever
      satisfy arr[mid] < x, ans would stay -1, and "no floor" is
      correctly reported.
*/
