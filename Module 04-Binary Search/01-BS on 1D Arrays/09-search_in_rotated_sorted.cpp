#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an array sorted in ascending order and then ROTATED at some unknown
// pivot, and a target x, return the index of x, or -1 if it is absent. All
// values are DISTINCT. The solution must run in O(log n).
//
// A rotation of {0,1,2,4,5,6,7} might be {4,5,6,7,0,1,2} - rotated by 4.
//
// Example:
// arr = {4, 5, 6, 7, 0, 1, 2}, x = 0  -> 4
// arr = {4, 5, 6, 7, 0, 1, 2}, x = 3  -> -1  (absent)
// arr = {4, 5, 6, 7, 0, 1, 2}, x = 4  -> 0   (the pivot itself)

/*
    Approach: Binary search on the half that is still sorted

    - Plain binary search needs "everything left of mid is smaller, everything
      right is larger", and rotation breaks exactly that. Comparing x against
      arr[mid] no longer tells you which side to keep: in {4,5,6,7,0,1,2} with
      mid = 3 (value 7), a target of 0 is smaller than 7 yet lives to the
      RIGHT. So the comparison against arr[mid] alone is not enough to decide.
    - The saving observation: a single rotation cuts the array into TWO sorted
      runs, so any midpoint splits it into a left part and a right part of
      which AT LEAST ONE is fully sorted. The break point can only be on one
      side of mid. Identify the sorted side, and on that side ordinary
      reasoning applies - a simple range test says whether x is in it.
    - Which side is sorted is one comparison: if arr[low] <= arr[mid], the
      whole stretch low..mid rises without a break, so the LEFT half is
      sorted; otherwise the break is in there and the RIGHT half must be the
      clean one. (The code writes this as arr[mid] >= arr[low], the same
      test.) Note it compares against arr[low], not against arr[high] - an
      endpoint of the live range, never a fixed end of the array.
    - Then the decision is a containment test, not a magnitude test:
        * left sorted  : if arr[low] <= x <= arr[mid], x can only be in the
                         left half, so go left; otherwise it can only be in
                         the messy right half, so go right.
        * right sorted : if arr[mid] <= x <= arr[high], go right; otherwise
                         go left.
      Either way one half is eliminated per probe, so the O(log n) bound
      survives the rotation intact.
    - Why the eliminated half really is safe to drop: the sorted half's two
      endpoints bracket every value it contains. If x falls outside that
      bracket, it is provably absent from that entire half - no probing
      needed. If it falls inside, it cannot be in the other half, because the
      other half's values are all outside the bracket. The test is exact in
      both directions, which is why nothing is ever lost.
    - The naive alternative finds the pivot with one binary search and then
      runs a second one on the correct run. That is also O(log n) and is
      easier to reason about, but it is two passes and needs its own careful
      pivot-finding loop; this does it in one.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1.
    2. While low <= high, compute mid = low + (high - low) / 2.
    3. If arr[mid] == x, return mid.
    4. If arr[mid] >= arr[low] (left half sorted):
       if arr[low] <= x <= arr[mid], go left (high = mid - 1), else go right.
    5. Else (right half sorted):
       if arr[mid] <= x <= arr[high], go right (low = mid + 1), else go left.
    6. Return -1 if the loop empties the range.

    Time Complexity: O(log n) - exactly one half is discarded per probe, just
                      as in plain binary search; the rotation only changes WHICH
                      half, never how much is eliminated.
    Space Complexity: O(1) - three indices.
*/
int search(vector<int>& arr, int x) {
  int n = arr.size();
  int low = 0, high = n - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (arr[mid] == x) {
      return mid;
    } else if (arr[mid] >= arr[low]) {
      // low..mid rises without a break, so the LEFT half is the sorted one
      if (arr[mid] >= x && arr[low] <= x) {
        high = mid - 1;  // x is bracketed by the sorted half's endpoints
      } else {
        low = mid + 1;  // x is outside that bracket, so it can only be right
      }
    } else {
      // the break is in low..mid, so the RIGHT half is the sorted one
      if (arr[mid] <= x && arr[high] >= x) {
        low = mid + 1;
      } else {
        high = mid - 1;
      }
    }
  }

  return -1;
}

int main() {
  vector<int> arr = {4, 5, 6, 7, 0, 1, 2};

  cout << "Array: ";
  for (int v : arr) cout << v << " ";
  cout << "   (sorted {0..7} minus 3, rotated by 4)" << endl;

  for (int x : {0, 3, 4, 2, 7}) {
    cout << "search(" << x << "): " << search(arr, x) << endl;
  }

  // a non-rotated array is the degenerate case - rotation by 0
  vector<int> plain = {1, 3, 5, 7, 9};
  cout << endl << "Array: ";
  for (int v : plain) cout << v << " ";
  cout << "   (rotation by 0)" << endl;
  cout << "search(7): " << search(plain, 7) << endl;
  cout << "search(4): " << search(plain, 4) << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {4, 5, 6, 7, 0, 1, 2}, x = 0   (n = 7, answer = 4)
    ======================================================================

      index:    0    1    2    3    4    5    6
      value:    4    5    6    7    0    1    2
                \------------/    \---------/
                 first run         second run
                                ^
                                the break: arr[3] = 7 > arr[4] = 0

    Tracked state:
      low, high - the inclusive bounds of the part still being searched
      mid       - low + (high - low) / 2, the probe index
      (no candidate variable - a hit returns immediately, exactly as in
       01-binary_search.cpp; the rotation changes the NAVIGATION, not the
       exact-match nature of the question)

    Initial state: low = 0, high = 6

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 6      (7 candidates alive)
      mid          0 + (6 - 0) / 2 = 3
      a[mid]       arr[3] = 7
      hit test     7 != 0
      which half   arr[mid] = 7 >= arr[low] = 4  ->  LEFT half is sorted
                   indeed 4, 5, 6, 7 rises cleanly; the break is to the right
      bracket      is 4 <= 0 <= 7 ?   arr[low] <= x fails (4 <= 0 is false)
      conclude     0 is NOT in the sorted left half - it is smaller than
                   every value there, so it can only be in the messy right
      discard      indices 0..3
      update       low = mid + 1 = 4

                     4   5   6   7 | 0   1   2
                   [ x   x   x   x | .   .   . ]
                                     live range is now 4..6

      NOTE the trap avoided here: x = 0 < arr[mid] = 7, and a plain binary
      search would have gone LEFT on that comparison alone. The bracket test
      is what sends it right instead.

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 4, high = 6      (3 candidates alive: 0, 1, 2)
      mid          4 + (6 - 4) / 2 = 5
      a[mid]       arr[5] = 1
      hit test     1 != 0
      which half   arr[mid] = 1 >= arr[low] = 0  ->  LEFT half sorted
                   (the live range no longer straddles the break at all, so
                    both halves happen to be sorted; the test just picks one)
      bracket      is 0 <= 0 <= 1 ?   yes, both ends hold
      conclude     x lies inside the sorted left half
      discard      indices 5..6
      update       high = mid - 1 = 4

                     4   5   6   7   0 | 1   2
                   [ x   x   x   x   . | x   x ]
                                         live range is now 4..4

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 4, high = 4      (1 candidate: arr[4] = 0)
      mid          4 + (4 - 4) / 2 = 4
      a[mid]       arr[4] = 0
      hit test     0 == 0  ->  HIT
      return       mid = 4 immediately

                     4   5   6   7   0   1   2
                                     ^
                                     mid = 4  <-- found here

    ----------------------------------------------------------------------
    RETURN 4        (arr[4] == 0)

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | a[mid] | sorted half | x in bracket? | action   |
    |------|-----|------|-----|--------|-------------|---------------|----------|
    |  1   |  0  |  6   |  3  |   7    | left  [4,7] | no  (0 < 4)   | low = 4  |
    |  2   |  4  |  6   |  5  |   1    | left  [0,1] | yes (0..1)    | high = 4 |
    |  3   |  4  |  4   |  4  |   0    |      -      | HIT           | return 4 |

    Step count behind the O(log n) claim:
      7 elements, 3 probes; the live range shrank 7 -> 3 -> 1 -> hit. Every
      iteration discarded a full half, which is the whole point: identifying
      the sorted side costs one comparison and buys the same elimination a
      non-rotated array would give.

    ======================================================================
    Notes
    ======================================================================

    Why the sortedness test uses arr[low] and not arr[0]:
      arr[low] is the left endpoint of the LIVE range, which shrinks as the
      search proceeds. Comparing against the array's fixed first element
      would describe the original array's shape, not the subarray actually
      being searched, and would misidentify the sorted half the moment low
      moves past the break. Iteration 2 above is exactly that situation:
      arr[low] = arr[4] = 0, not arr[0] = 4.

    Why >= rather than > in arr[mid] >= arr[low]:
      when the live range has one or two elements, mid can equal low, making
      arr[mid] == arr[low]. A single element (or a two-element range probed
      at its left) IS trivially sorted, so it must take the "left sorted"
      branch. A strict > would send it to the else branch, which would then
      reason about a right half using arr[high] - still correct here by luck,
      but the >= states the intent and is what keeps the one-element case
      obviously right. With DISTINCT values, arr[mid] == arr[low] can only
      mean mid == low; that stops being true in the duplicates variant, and
      is precisely where 10-search_in_rotated_sorted_II.cpp has to intervene.

    Why the bracket test needs BOTH comparisons:
      arr[mid] >= x alone would be the plain binary-search test and would
      have gone left at iteration 1, missing the answer. arr[low] <= x alone
      would not rule out targets above the sorted half. Together they say "x
      lies between this half's two endpoints", which - because the half is
      sorted - is exactly the condition for x to be inside it. This is a
      containment test, not a magnitude test, and that shift is the whole
      idea of the algorithm.

    Why at least one half is always sorted:
      one rotation creates exactly one break point. It sits either in
      low..mid or in mid..high, and it cannot be in both. Whichever side does
      not contain it is a clean ascending run. With two or more breaks the
      argument collapses - which is why this technique applies to a rotated
      sorted array and not to an arbitrary one.

    Edge cases:
      - rotation by 0 (a plain sorted array): arr[mid] >= arr[low] always
        holds, the left branch is always taken, and the code degenerates into
        ordinary binary search.
      - x at the pivot (x = 4, index 0): iteration 1 finds arr[low] <= 4 <= 7
        and goes left, then converges normally.
      - absent target (x = 3): the loop eliminates a half each time, low
        eventually passes high, and -1 is returned. No branch can loop
        forever, since both branches move an endpoint strictly past mid.
      - empty array: n = 0 makes high = -1 and the loop never runs -> -1.
      - single element: low = high = mid = 0, and it is either a hit or the
        range empties on the next step.

    Companion file:
      10-search_in_rotated_sorted_II.cpp is this same function with
      DUPLICATES allowed. The one comparison that identifies the sorted half
      stops being reliable there - arr[low] == arr[mid] == arr[high] tells
      you nothing - and the fix costs the O(log n) guarantee in the worst
      case.
*/
