#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array and a value x, find the index of the ceil of x
// (the smallest element in the array that is >= x)

/*
    Approach: Binary Search
    - Binary search for x while tracking the best candidate seen so far
    - Whenever arr[mid] >= x, it's a valid ceil candidate, so record it and
      search the left half for something closer to x
    - Whenever arr[mid] < x, it can't be the ceil, so search the right half

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = n - 1, ans = -1
    2. While l <= r, compute mid = l + (r - l) / 2
    3. If arr[mid] == x, return mid (exact match is the ceil)
    4. If arr[mid] > x, update ans = mid and search left (r = mid - 1)
    5. Else search right (l = mid + 1)
    6. Return ans (-1 if no ceil exists)

    Time Complexity: O(log n) - binary search
    Space Complexity: O(1) - constant extra space
*/
int findCeil(vector<int>& arr, int x) {
  int n = arr.size();
  int l = 0, r = n - 1;
  int ans = -1;  // store ceil index (default -1 if no ceil exists)

  while (l <= r) {
    int mid = l + (r - l) / 2;

    if (arr[mid] == x) {
      return mid;  // exact match is the ceil itself
    } else if (arr[mid] > x) {
      ans = mid;    // candidate for ceil
      r = mid - 1;  // try to find smaller candidate on left
    } else {
      l = mid + 1;  // move right since arr[mid] < x
    }
  }

  return ans;  // returns index of ceil element OR -1 if none found
}

int main() {
  vector<int> arr = {1, 2, 8, 10, 11, 12, 19};
  int x = 5;

  cout << "Array: ";
  for (int n : arr) cout << n << " ";
  cout << endl;

  int idx = findCeil(arr, x);
  cout << "Ceil of " << x << ": ";
  if (idx == -1) cout << "does not exist" << endl;
  else cout << arr[idx] << " (index " << idx << ")" << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {1, 2, 8, 10, 11, 12, 19}, x = 5
             (n = 7, answer = index 2, value 8)
    ======================================================================

      index:    0    1    2    3    4    5    6
      value:    1    2    8   10   11   12   19
                          ^
                          ceil of 5 - x = 5 is NOT in the array, which is
                          exactly the case that makes `ans` necessary

    Tracked state:
      l, r  - the inclusive bounds of the part still being searched
      mid   - l + (r - l) / 2, the probe index
      ans   - index of the best ceil candidate seen so far, start -1.
              A candidate is any mid with arr[mid] > x; each new one is
              further left, hence closer to x, so it overwrites the old.

    Initial state: l = 0, r = 6, ans = -1

    ----------------------------------------------------------------------
    Iteration 1
      bounds       l = 0, r = 6
      mid          0 + (6 - 0) / 2 = 3
      a[mid]       arr[3] = 10
      compare      10 > 5  -> valid ceil candidate (it is >= x)
      record       ans = mid = 3                       <-- first candidate
      discard      indices 3..6 as candidates: anything right of mid is
                   >= 10, so it can only be a WORSE (larger) ceil
      update       r = mid - 1 = 2

                    1   2   8  10  11  12  19
                  [ .   .   . | x   x   x   x ]
                               live range now 0..2, ans = 3 held in hand

    ----------------------------------------------------------------------
    Iteration 2
      bounds       l = 0, r = 2
      mid          0 + (2 - 0) / 2 = 1
      a[mid]       arr[1] = 2
      compare      2 < 5  -> too small, cannot be a ceil of 5
      record       nothing; ans stays 3
      discard      indices 0..1 (sorted, so all <= 2 < 5)
      update       l = mid + 1 = 2

    ----------------------------------------------------------------------
    Iteration 3
      bounds       l = 2, r = 2      (one element left: arr[2] = 8)
      mid          2 + (2 - 2) / 2 = 2
      a[mid]       arr[2] = 8
      compare      8 > 5  -> better candidate, and it is further left
      record       ans = 2           <-- overwrites 3, now the true ceil
      update       r = mid - 1 = 1

                    1   2   8  10  11  12  19
                        ^   ^
                        r   l          r < l -> range is empty
      loop test    l = 2 > r = 1  ->  while (l <= r) fails, loop ends

    ----------------------------------------------------------------------
    RETURN ans = 2        (arr[2] = 8, the smallest element >= 5)

    ======================================================================
    Summary table
    ======================================================================

    | iter | l | r | mid | a[mid] | vs x = 5 | ans | half discarded |
    |------|---|---|-----|--------|----------|-----|----------------|
    |  1   | 0 | 6 |  3  |   10   | 10 > 5   |  3  | right (3..6)   |
    |  2   | 0 | 2 |  1  |    2   |  2 < 5   |  3  | left  (0..1)   |
    |  3   | 2 | 2 |  2  |    8   |  8 > 5   |  2  | right (2..2)   |
    |      | 2 | 1 |  -  |    -   | l > r    |  2  | loop ends      |

    Step count behind the O(log n) claim:
      7 elements, 3 probes; the live range shrank 7 -> 3 -> 1 -> 0, which
      is ceil(log2(7 + 1)) = 3 halvings.

    The teaching point - `ans` is overwritten, so the LAST write wins:
      unlike plain binary search, which returns the instant it hits the
      target, ceil keeps going after finding a valid candidate. ans was 3
      (value 10) after iteration 1 - correct but not best - and only
      iteration 3 improved it to 2 (value 8). Stopping the loop early
      would have reported the wrong ceil. Note also that the answer sits
      in `ans`, NOT in l: at the end l = 2 here only by coincidence,
      while r = 1 points at a value below x.
      If x were larger than every element, e.g. x = 25, no mid would ever
      satisfy arr[mid] > x, ans would stay -1, and "no ceil" is correctly
      reported.
*/
