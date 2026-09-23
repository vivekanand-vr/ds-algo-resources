#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array arr and a value x, return the INDEX of the floor of
// x - the largest element in arr that is <= x. Return -1 if no such element
// exists (i.e. x is smaller than every element).
//
// Example:
// arr = {1, 2, 8, 10, 11, 12, 19}, x = 5  -> 1   (arr[1] = 2 is the floor)
// arr = {1, 2, 8, 10, 11, 12, 19}, x = 20 -> 6   (the last element qualifies)
// arr = {1, 2, 8, 10, 11, 12, 19}, x = 0  -> -1  (nothing is <= 0)

/*
    Approach: Binary Search with a recorded candidate

    - The floor is a NEAR-MISS answer, not an exact hit, so the loop cannot
      return the moment it probes a qualifying element. Any arr[mid] <= x is
      only the best answer SO FAR; a larger qualifying element may still sit
      to its right. The fix is to record mid and keep searching right.
    - The array is sorted, so "arr[mid] <= x" splits it cleanly:
        * arr[mid] <= x : mid qualifies, and so does everything left of it -
          but they are all smaller, so they can never beat mid. Record mid as
          the best candidate and discard the whole left half along with mid.
        * arr[mid] >  x : mid is too big, and so is everything right of it.
          Discard that half without recording anything.
      Each probe therefore halves the range while the recorded candidate only
      ever moves rightward - to strictly better answers.
    - The invariant that makes it correct: lowb always holds the index of the
      largest element seen so far that is <= x. When the loop ends the range
      is empty, every index has been classified, and lowb is final.
    - Starting lowb at -1 is what makes the "no floor exists" case fall out
      for free: if the first branch never fires, nothing is ever recorded and
      -1 is returned unchanged.
    - The naive scan walks left to right and keeps the last index with
      arr[i] <= x. Correct, but O(n) - it ignores the sortedness that is the
      whole reason binary search applies here.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1, lowb = -1.
    2. While low <= high, compute mid = low + (high - low) / 2.
    3. If arr[mid] <= x: record lowb = mid, then search right (low = mid + 1).
    4. Else: search left (high = mid - 1), recording nothing.
    5. Return lowb.

    Time Complexity: O(log n) - the live range halves on every probe, and the
                      loop does O(1) work per probe.
    Space Complexity: O(1) - three indices, no auxiliary structure.
*/
int findFloor(vector<int>& arr, int x) {
  int n = arr.size();
  int low = 0, high = n - 1;
  int lowb = -1;  // best index so far with arr[index] <= x; -1 means none yet

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (arr[mid] <= x) {
      lowb = mid;  // qualifies, but something further right may be better
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return lowb;
}

int main() {
  vector<int> arr = {1, 2, 8, 10, 11, 12, 19};

  cout << "Array: ";
  for (int v : arr) cout << v << " ";
  cout << endl;

  for (int x : {5, 20, 0, 10}) {
    int idx = findFloor(arr, x);
    cout << "floor index of " << x << ": " << idx;
    if (idx != -1) cout << "   (value " << arr[idx] << ")";
    cout << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {1, 2, 8, 10, 11, 12, 19}, x = 5   (n = 7, answer = 1)
    ======================================================================

      index:    0    1    2    3    4    5    6
      value:    1    2    8   10   11   12   19
                     ^
                     floor of 5 is 2, at index 1

    Tracked state:
      low, high - the inclusive bounds of the part still being searched
      mid       - low + (high - low) / 2, the probe index
      lowb      - the best index found so far with arr[index] <= x.
                  Written ONLY on the qualifying branch, and never reset.

    Initial state: low = 0, high = 6, lowb = -1

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 6      (7 candidates alive)
      mid          0 + (6 - 0) / 2 = 3
      a[mid]       arr[3] = 10
      compare      10 > 5  -> too big to be a floor
      record       nothing - lowb stays -1
      discard      indices 3..6 (all >= 10 > 5, since the array is sorted)
      update       high = mid - 1 = 2

                     1   2   8 | 10  11  12  19
                   [ .   .   . | x   x   x   x ]
                                 live range is now 0..2

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 0, high = 2      (3 candidates alive)
      mid          0 + (2 - 0) / 2 = 1
      a[mid]       arr[1] = 2
      compare      2 <= 5  -> QUALIFIES
      record       lowb = 1                       <-- the eventual answer
      discard      indices 0..1 - index 1 because it is already banked in
                   lowb, index 0 because arr[0] = 1 < 2 can only be worse
      update       low = mid + 1 = 2

                     1   2 | 8  10  11  12  19
                   [ x   x | .   x   x   x   x ]
                             live range is now 2..2

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 2, high = 2      (1 candidate alive: arr[2] = 8)
      mid          2 + (2 - 2) / 2 = 2
      a[mid]       arr[2] = 8
      compare      8 > 5  -> too big
      record       nothing - lowb stays 1
      update       high = mid - 1 = 1

                     1   2   8  10  11  12  19
                         ^   ^
                      high   low      high = 1 < low = 2 -> range is empty
      loop test    low = 2 > high = 1  ->  while (low <= high) fails

    ----------------------------------------------------------------------
    RETURN lowb = 1        (arr[1] = 2, the largest element <= 5)

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | a[mid] | vs x = 5 | lowb | half discarded |
    |------|-----|------|-----|--------|----------|------|----------------|
    |  1   |  0  |  6   |  3  |   10   |  10 > 5  |  -1  | right (3..6)   |
    |  2   |  0  |  2   |  1  |    2   |   2 <= 5 |   1  | left  (0..1)   |
    |  3   |  2  |  2   |  2  |    8   |   8 > 5  |   1  | right (2..2)   |
    |      |  2  |  1   |  -  |    -   | low>high |   1  | loop ends      |

    Step count behind the O(log n) claim:
      7 elements, 3 probes; the live range shrank 7 -> 3 -> 1 -> 0, i.e.
      ceil(log2(7 + 1)) = 3 probes, which is the worst case for n = 7.

    The teaching point - why the loop cannot return early:
      at iteration 2 the probe found a qualifying element (arr[1] = 2 <= 5)
      and it happened to be the answer, but the algorithm had no way to know
      that yet - arr[2] = 8 was still unexamined and could just as easily
      have been a 4. So the qualifying branch must RECORD and CONTINUE. This
      is the structural difference from 01-binary_search.cpp, which returns
      on the hit and needs no candidate variable at all.

    Why lowb is never re-checked before being overwritten:
      lowb = mid is an unconditional assignment, with no "if (mid > lowb)"
      guard. That is safe because the qualifying branch always moves low
      rightward, so every later probe has a strictly larger index than any
      previously recorded one - and a larger index in a sorted array means a
      larger (or equal) value. The candidate can only improve.

    Edge cases:
      - x below everything (x = 0): the first branch never fires, lowb is
        never written, and -1 comes back. No special case needed.
      - x above everything (x = 20): the qualifying branch fires every time,
        low walks past the end, and lowb ends at n - 1 = 6.
      - x present with duplicates: this returns the LAST index holding a
        value <= x, so on {2, 2, 2} with x = 2 it reports index 2, not 0.
        That is the correct floor index by the usual definition; use a
        lower-bound style search instead if the FIRST occurrence is wanted.
      - empty array: n = 0 makes high = -1, the loop never runs, and -1 is
        returned - which is the right answer.

    Companion file:
      06-find_ceil.cpp runs this exact loop and reports the other side of
      the split. The two together are what 05-find_floor_and_ceil.cpp folds
      into a single pass.
*/
