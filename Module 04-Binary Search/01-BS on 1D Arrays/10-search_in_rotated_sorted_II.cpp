#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an array sorted in ascending order and then ROTATED at some unknown
// pivot, and a target x, return whether x is present. Unlike part I, the array
// MAY CONTAIN DUPLICATES - so only a true/false answer is asked for, not an
// index.
//
// Example:
// arr = {2, 5, 6, 0, 0, 1, 2}, x = 0  -> true
// arr = {2, 5, 6, 0, 0, 1, 2}, x = 3  -> false
// arr = {3, 1, 2, 3, 3, 3, 3}, x = 2  -> true   (the case part I would fail)
// arr = {1, 1, 1, 1, 1},       x = 2  -> false  (the O(n) worst case)

/*
    Approach: Part I's search, plus a shrink step for the ambiguous case

    - 09-search_in_rotated_sorted.cpp rests on one comparison: arr[mid] >=
      arr[low] means the left half is sorted, otherwise the right half is.
      With distinct values that test is exact. Duplicates break it, and they
      break it in the one way that cannot be patched by a smarter comparison.
    - The failing shape: arr[low] == arr[mid] == arr[high]. Consider
      {3, 1, 2, 3, 3, 3, 3} with low = 0, mid = 3, high = 6 - all three read
      3, and the break is on the LEFT. Now consider {3, 3, 3, 3, 1, 2, 3} -
      the same three reads, but the break is on the RIGHT. The two arrays are
      indistinguishable from those three positions, so NO decision made from
      them can be right for both. The information simply is not there.
    - So the algorithm stops trying to decide and instead makes the ambiguity
      smaller: low++ and high--. This is safe because arr[low] and arr[high]
      both equal arr[mid], which has already been tested against x and failed
      - so neither endpoint can be the answer, and discarding them loses
      nothing. It is the only branch in this file that does not halve the
      range, and it is the reason the worst case is O(n).
    - Every other branch is character-for-character part I: find the sorted
      half, test whether x lies between its endpoints, keep that half if so
      and the other half if not. The ambiguous branch is checked FIRST, so
      the sortedness test is only ever reached when it is still meaningful.
    - Why the answer is a bool, not an index: with duplicates there may be
      many valid indices, and the shrink step discards positions without
      examining them, so no particular occurrence can be promised. Part I can
      return an index precisely because distinct values make the answer
      unique.
    - The naive alternative is a linear scan: O(n), and on the pathological
      input it is no worse than this. The binary search still wins decisively
      whenever duplicates are sparse, which is the common case - it only
      degrades in proportion to how many equal values crowd the endpoints.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1.
    2. While low <= high, compute mid = low + (high - low) / 2.
    3. If arr[mid] == x, return true.
    4. If arr[low] == arr[mid] && arr[mid] == arr[high]: low++, high--, and
       continue - the sorted half cannot be identified from these reads.
    5. Else if arr[mid] >= arr[low] (left half sorted):
       if arr[low] <= x <= arr[mid], go left (high = mid - 1), else go right.
    6. Else (right half sorted):
       if arr[mid] <= x <= arr[high], go right (low = mid + 1), else go left.
    7. Return false if the loop empties the range.

    Time Complexity: O(log n) average, O(n) worst case. Steps 5 and 6 halve
                      the range, but step 4 only trims one element from each
                      end. An array like {1,1,1,...,1,2} searched for 2 hits
                      step 4 almost every time, giving ~n/2 iterations. The
                      worst case is unavoidable, not an artefact of this
                      implementation: distinguishing {1,1,1,1} from
                      {1,1,2,1} requires reading a position that no
                      comparison of the endpoints can point to.
    Space Complexity: O(1) - three indices.
*/
bool search(vector<int>& arr, int x) {
  int n = arr.size();
  int low = 0, high = n - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (arr[mid] == x) {
      return true;
    } else if (arr[low] == arr[mid] && arr[mid] == arr[high]) {
      // the three reads are identical, so which half is sorted cannot be told;
      // both endpoints equal arr[mid], which already failed, so drop them
      low++;
      high--;
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

  return false;
}

int main() {
  vector<int> arr = {2, 5, 6, 0, 0, 1, 2};

  cout << "Array: ";
  for (int v : arr) cout << v << " ";
  cout << endl;

  for (int x : {0, 3, 2, 6}) {
    cout << "search(" << x << "): " << (search(arr, x) ? "true" : "false") << endl;
  }

  // the shape that makes part I's sortedness test ambiguous
  vector<int> tricky = {3, 1, 2, 3, 3, 3, 3};
  cout << endl << "Array: ";
  for (int v : tricky) cout << v << " ";
  cout << "   (arr[low] == arr[mid] == arr[high] on the first probe)" << endl;
  cout << "search(2): " << (search(tricky, 2) ? "true" : "false") << endl;

  // the O(n) worst case
  vector<int> flat(11, 1);
  flat[5] = 2;
  cout << endl << "Array: ";
  for (int v : flat) cout << v << " ";
  cout << "   (the shrink branch fires almost every iteration)" << endl;
  cout << "search(2): " << (search(flat, 2) ? "true" : "false") << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {3, 1, 2, 3, 3, 3, 3}, x = 2   (n = 7, answer = true)
    ======================================================================

      index:    0    1    2    3    4    5    6
      value:    3    1    2    3    3    3    3
                \/   \-------------------------/
             run 1            run 2
                ^
                the break: arr[0] = 3 > arr[1] = 1

    This input is chosen because it is precisely the one part I gets wrong:
    on the first probe arr[low], arr[mid] and arr[high] all read 3, and the
    left half {3, 1, 2, 3} is NOT sorted even though arr[mid] >= arr[low]
    would claim it is.

    Tracked state:
      low, high - the inclusive bounds of the part still being searched
      mid       - low + (high - low) / 2, the probe index
      (no candidate variable - a hit returns immediately)

    Initial state: low = 0, high = 6

    ----------------------------------------------------------------------
    Iteration 1        <-- the ambiguous case
      bounds       low = 0, high = 6      (7 candidates alive)
      mid          0 + (6 - 0) / 2 = 3
      reads        arr[low] = arr[0] = 3
                   arr[mid] = arr[3] = 3
                   arr[high] = arr[6] = 3
      hit test     3 != 2
      ambiguity    all three are equal -> which half is sorted is UNKNOWABLE
                   from these reads. The mirror array {3,3,3,3,1,2,3} gives
                   the identical three values with the break on the other
                   side, so any decision here would be wrong for one of them.
      why safe     arr[low] and arr[high] both equal arr[mid] = 3, which has
                   already been compared against x = 2 and failed. Neither
                   endpoint can be the answer, so dropping both loses nothing.
      update       low++ -> 1, high-- -> 5
      note         this is the ONLY branch that does not halve the range; it
                   removed 2 elements out of 7 instead of about half

                     3 | 1   2   3   3   3 | 3
                   [ x | .   .   .   .   . | x ]
                         live range is now 1..5

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 1, high = 5      (5 candidates alive)
      mid          1 + (5 - 1) / 2 = 3
      reads        arr[1] = 1, arr[3] = 3, arr[5] = 3
      hit test     3 != 2
      ambiguity    arr[low] = 1 != arr[mid] = 3  ->  not ambiguous, proceed
      which half   arr[mid] = 3 >= arr[low] = 1  ->  LEFT half sorted
                   indeed arr[1..3] = 1, 2, 3 rises cleanly - the break was
                   trimmed away by iteration 1, which is exactly what the
                   shrink step bought
      bracket      is 1 <= 2 <= 3 ?   yes, both ends hold
      conclude     x lies inside the sorted left half
      discard      indices 4..5
      update       high = mid - 1 = 2

                     3   1   2 | 3   3   3   3
                   [ x   .   . | x   x   x   x ]
                                 live range is now 1..2

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 1, high = 2      (2 candidates: 1 and 2)
      mid          1 + (2 - 1) / 2 = 1
      reads        arr[1] = 1, arr[1] = 1, arr[2] = 2
      hit test     1 != 2
      ambiguity    arr[low] = arr[mid] = 1, but arr[high] = 2 differs -> no
      which half   arr[mid] = 1 >= arr[low] = 1  ->  LEFT half sorted
                   (mid == low here, so the "left half" is the single element
                    arr[1], trivially sorted - this is the case the >= in the
                    comparison exists for)
      bracket      is 1 <= 2 <= 1 ?   arr[mid] >= x fails (1 >= 2 is false)
      conclude     x is above this half's top, so it can only be to the right
      update       low = mid + 1 = 2

                     3   1 | 2   3   3   3   3
                   [ x   x | .   x   x   x   x ]
                             live range is now 2..2

    ----------------------------------------------------------------------
    Iteration 4
      bounds       low = 2, high = 2      (1 candidate: arr[2] = 2)
      mid          2 + (2 - 2) / 2 = 2
      hit test     arr[2] = 2 == 2  ->  HIT
      return       true immediately

                     3   1   2   3   3   3   3
                             ^
                             mid = 2  <-- found here

    ----------------------------------------------------------------------
    RETURN true

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | a[lo],a[mid],a[hi] | branch taken      | action        |
    |------|-----|------|-----|--------------------|-------------------|---------------|
    |  1   |  0  |  6   |  3  |     3, 3, 3        | ambiguous         | low=1, high=5 |
    |  2   |  1  |  5   |  3  |     1, 3, 3        | left sorted [1,3] | high = 2      |
    |  3   |  1  |  2   |  1  |     1, 1, 2        | left sorted [1,1] | low = 2       |
    |  4   |  2  |  2   |  2  |     2, 2, 2        | HIT               | return true   |

      Iteration 4's three reads are all equal too, but the hit test is
      checked BEFORE the ambiguity test, so it returns rather than shrinking.
      That ordering is load-bearing - see the Notes.

    Step count behind the complexity claim:
      7 elements, 4 probes. Iteration 1 removed only 2 elements where a clean
      halving would have removed 3 or 4; iterations 2 and 3 then halved
      normally. That is the degradation in miniature - one ambiguous probe
      costs a constant amount of progress instead of a proportional amount.

    ======================================================================
    The O(n) worst case, traced briefly
    ======================================================================

      arr = {1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1}, x = 2      (n = 11)

      Every probe that lands on a 1 finds arr[low] == arr[mid] == arr[high]
      == 1, so the shrink branch fires and the range loses one element from
      each end:

        low=0  high=10  mid=5   arr[5] = 2  ->  HIT on the very first probe

      Move the 2 to index 1 instead and the picture inverts:

        low=0  high=10  mid=5   1,1,1  -> shrink  -> low=1,  high=9
        low=1  high=9   mid=5   1,1,1  -> shrink  -> low=2,  high=8
        low=2  high=8   mid=5   1,1,1  -> shrink  -> low=3,  high=7
        ...                                           and so on

      ...until low rises past index 1 - meaning the answer was trimmed away
      from the left edge, having been compared against x on the way out. The
      range shrinks by 2 per iteration rather than halving, so the run is
      linear in n. No implementation can avoid this: the array {1,1,1,1} and
      the array {1,1,2,1} agree at every endpoint, so only a position-by-
      position look can tell them apart.

    ======================================================================
    Notes
    ======================================================================

    Why the hit test must come before the ambiguity test:
      if the shrink branch were checked first, a range whose three reads are
      all equal to x would discard both endpoints without noticing that they
      ARE the answer. Iteration 4 above is that situation. The current order
      makes the shrink branch reachable only when arr[mid] != x, which is
      what licenses the claim "neither endpoint can be the answer" - both
      equal arr[mid], and arr[mid] has just been rejected.

    Why low++ and high-- rather than just low++:
      only low++ would also terminate and would also be correct, since the
      same argument covers arr[low] alone. Trimming both ends is strictly
      better - it removes two known-useless positions per iteration instead
      of one, halving the constant on the worst case - and it keeps the
      branch symmetric with the ambiguity that caused it.

    Why this is O(n) and part I is not:
      part I's sortedness test is exact because distinct values make
      arr[low] == arr[mid] possible only when low == mid. Duplicates sever
      the link between "these values are equal" and "these positions are
      close", so the test can be satisfied by an unsorted half. The lost
      information has to be recovered by looking, and looking is linear.

    What carries over unchanged from part I:
      the sortedness test arr[mid] >= arr[low] against the LIVE left
      endpoint (not arr[0]); the bracket test needing BOTH comparisons, since
      it asks containment rather than magnitude; and the guarantee that at
      least one half of any split is sorted, which follows from there being
      exactly one break point. Only the ambiguous branch is new.

    Edge cases:
      - no duplicates at all: the ambiguity branch never fires (arr[low] ==
        arr[mid] == arr[high] would force low == mid == high, and that single
        element is either a hit or the range empties), so the function
        behaves exactly like part I at a clean O(log n).
      - every element identical, target absent ({1,1,1,1,1}, x = 2): the
        shrink branch fires every iteration until low passes high, returning
        false in ~n/2 steps.
      - rotation by 0: the left branch is always taken and this degenerates
        into ordinary binary search.
      - empty array: n = 0 makes high = -1, the loop never runs -> false.

    Companion file:
      09-search_in_rotated_sorted.cpp is this function minus the ambiguous
      branch, returning an INDEX because distinct values make the answer
      unique.
*/
