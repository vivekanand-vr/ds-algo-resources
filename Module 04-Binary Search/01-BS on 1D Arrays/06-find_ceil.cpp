#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array arr and a value x, return the INDEX of the ceil of
// x - the smallest element in arr that is >= x. Return -1 if no such element
// exists (i.e. x is larger than every element).
//
// Example:
// arr = {1, 2, 8, 10, 11, 12, 19}, x = 5  -> 2   (arr[2] = 8 is the ceil)
// arr = {1, 2, 8, 10, 11, 12, 19}, x = 0  -> 0   (the first element qualifies)
// arr = {1, 2, 8, 10, 11, 12, 19}, x = 20 -> -1  (nothing is >= 20)

/*
    Approach: Binary Search with a recorded candidate - the mirror of floor

    - Same shape as 02-find_floor.cpp, and deliberately the same loop: the
      test arr[mid] <= x splits the array into a "too small" side and a
      "qualifies as ceil" side. Floor reports the best index from the first
      side; ceil reports the best index from the second. Only the recorded
      branch and the returned variable differ.
    - The ceil is a near-miss answer, so the loop cannot return on the first
      qualifying probe. Any arr[mid] > x qualifies, but a SMALLER qualifying
      element may still sit to its left, so record mid and keep searching
      left. The candidate only ever moves leftward - to strictly better
      answers.
    - The invariant: highb always holds the index of the smallest element
      seen so far that is > x. When the loop ends the range is empty, every
      index has been classified, and highb is final.
    - One subtlety worth being explicit about: the branch is arr[mid] <= x,
      not arr[mid] < x. So an element EQUAL to x goes to the floor side and
      is never recorded as a ceil. This function therefore returns the
      smallest element STRICTLY GREATER than x - the "upper bound". On the
      usual GfG-style definition (smallest element >= x, so x itself when
      present) change the comparison to arr[mid] < x; that variant is what
      03-search_insert_position.cpp computes as a gap position. Both
      conventions exist - pick one deliberately rather than by accident.
    - The naive scan walks left to right and stops at the first element > x.
      Correct, but O(n) and it throws away the sortedness.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1, highb = -1.
    2. While low <= high, compute mid = low + (high - low) / 2.
    3. If arr[mid] <= x: search right (low = mid + 1), recording nothing.
    4. Else: record highb = mid, then search left (high = mid - 1).
    5. Return highb.

    Time Complexity: O(log n) - the live range halves on every probe, and the
                      loop does O(1) work per probe.
    Space Complexity: O(1) - three indices, no auxiliary structure.
*/
int findCeil(vector<int>& arr, int x) {
  int n = arr.size();
  int low = 0, high = n - 1;
  int highb = -1;  // best index so far with arr[index] > x; -1 means none yet

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (arr[mid] <= x) {
      low = mid + 1;
    } else {
      highb = mid;  // qualifies, but something further left may be better
      high = mid - 1;
    }
  }

  return highb;
}

int main() {
  vector<int> arr = {1, 2, 8, 10, 11, 12, 19};

  cout << "Array: ";
  for (int v : arr) cout << v << " ";
  cout << endl;

  for (int x : {5, 0, 20, 10}) {
    int idx = findCeil(arr, x);
    cout << "ceil index of " << x << ": " << idx;
    if (idx != -1) cout << "   (value " << arr[idx] << ")";
    cout << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {1, 2, 8, 10, 11, 12, 19}, x = 5   (n = 7, answer = 2)
    ======================================================================

      index:    0    1    2    3    4    5    6
      value:    1    2    8   10   11   12   19
                          ^
                          ceil of 5 is 8, at index 2

    Tracked state:
      low, high - the inclusive bounds of the part still being searched
      mid       - low + (high - low) / 2, the probe index
      highb     - the best index found so far with arr[index] > x.
                  Written ONLY on the non-qualifying-for-floor branch, and
                  never reset.

    Initial state: low = 0, high = 6, highb = -1

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 6      (7 candidates alive)
      mid          0 + (6 - 0) / 2 = 3
      a[mid]       arr[3] = 10
      compare      10 > 5  -> QUALIFIES as a ceil
      record       highb = 3      (best so far, but probably beatable)
      discard      indices 3..6 - index 3 because it is already banked in
                   highb, indices 4..6 because they are all >= 10 and so
                   can only be worse
      update       high = mid - 1 = 2

                     1   2   8 | 10  11  12  19
                   [ .   .   . | x   x   x   x ]
                                 live range is now 0..2

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 0, high = 2      (3 candidates alive)
      mid          0 + (2 - 0) / 2 = 1
      a[mid]       arr[1] = 2
      compare      2 <= 5  -> too small to be a ceil
      record       nothing - highb stays 3
      discard      indices 0..1 (both <= 2 < 5)
      update       low = mid + 1 = 2

                     1   2 | 8  10  11  12  19
                   [ x   x | .   x   x   x   x ]
                             live range is now 2..2

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 2, high = 2      (1 candidate alive: arr[2] = 8)
      mid          2 + (2 - 2) / 2 = 2
      a[mid]       arr[2] = 8
      compare      8 > 5  -> QUALIFIES, and 8 < 10 so it beats the old one
      record       highb = 2                      <-- the eventual answer
      update       high = mid - 1 = 1

                     1   2   8  10  11  12  19
                         ^   ^
                      high   low      high = 1 < low = 2 -> range is empty
      loop test    low = 2 > high = 1  ->  while (low <= high) fails

    ----------------------------------------------------------------------
    RETURN highb = 2        (arr[2] = 8, the smallest element > 5)

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | a[mid] | vs x = 5 | highb | half discarded |
    |------|-----|------|-----|--------|----------|-------|----------------|
    |  1   |  0  |  6   |  3  |   10   |  10 > 5  |   3   | right (3..6)   |
    |  2   |  0  |  2   |  1  |    2   |   2 <= 5 |   3   | left  (0..1)   |
    |  3   |  2  |  2   |  2  |    8   |   8 > 5  |   2   | right (2..2)   |
    |      |  2  |  1   |  -  |    -   | low>high |   2   | loop ends      |

    Compare this table with the one in 02-find_floor.cpp: the low, high, mid
    and a[mid] columns are IDENTICAL, because both functions run the same
    loop on the same input. Only which column is updated changes. That is
    the point of keeping them as twin files.

    Step count behind the O(log n) claim:
      7 elements, 3 probes; the live range shrank 7 -> 3 -> 1 -> 0, i.e.
      ceil(log2(7 + 1)) = 3 probes, the worst case for n = 7.

    The teaching point - the candidate improves monotonically:
      highb = mid is assigned with no "if (mid < highb)" guard, which is
      safe because the recording branch always moves high leftward. Every
      later probe therefore has a strictly smaller index than any previously
      recorded one, and a smaller index in a sorted array means a smaller
      (or equal) value. Iteration 3 overwriting 3 with 2 is exactly this
      happening - and it is why the loop must not stop at iteration 1.

    Edge cases:
      - x above everything (x = 20): the recording branch never fires, highb
        is never written, and -1 comes back. No special case needed.
      - x below everything (x = 0): every probe records, high walks down to
        -1, and highb ends at 0.
      - x present in the array (x = 10): because the split is arr[mid] <= x,
        index 3 is treated as floor material, and the answer is index 4
        (value 11) - the strict upper bound, not 10 itself. See the note in
        the Approach on the two ceil conventions.
      - empty array: n = 0 makes high = -1, the loop never runs, and -1 is
        returned - the right answer.

    Companion files:
      02-find_floor.cpp is this same loop returning the other variable, and
      05-find_floor_and_ceil.cpp keeps both candidates in one pass so a
      single O(log n) search answers both questions at once.
*/
