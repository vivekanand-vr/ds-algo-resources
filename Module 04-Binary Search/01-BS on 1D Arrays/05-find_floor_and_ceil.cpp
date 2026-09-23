#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a SORTED array arr and a value x, return {floor, ceil} as VALUES
// (not indices), where floor is the largest element <= x and ceil is the
// smallest element >= x. Return -1 in either slot when no such element
// exists. When x is present in the array it is both the floor and the ceil.
//
// Example:
// arr = {3, 4, 4, 7, 8, 10}, x = 5  -> {4, 7}
// arr = {3, 4, 4, 7, 8, 10}, x = 8  -> {8, 8}   (x present: both are x)
// arr = {3, 4, 4, 7, 8, 10}, x = 1  -> {-1, 3}  (nothing is <= 1)
// arr = {3, 4, 4, 7, 8, 10}, x = 99 -> {10, -1} (nothing is >= 99)

/*
    Approach: One binary search maintaining BOTH candidates

    - Sortedness is what separates this from 04-find_floor_and_ceil_unsorted.
      There, every element had to be examined because a qualifying value
      could hide anywhere. Here a single comparison against arr[mid] proves
      something about an entire half, so the search space halves per probe.
    - Both answers are near-miss answers, so neither branch may return early
      on a non-hit - each one records its side's best-so-far and keeps
      shrinking. The two candidates are updated on opposite branches and
      never compete for the same probe:
        * arr[mid] < x : mid is a floor candidate, and it beats any earlier
          one because the search only moves rightward after this branch.
          Record it and discard the left half.
        * arr[mid] > x : mid is a ceil candidate, and it beats any earlier
          one because the search only moves leftward after this branch.
          Record it and discard the right half.
        * arr[mid] == x : x is present, so it is simultaneously the largest
          element <= x and the smallest element >= x. Nothing can beat a
          distance of zero, so {x, x} is returned immediately - the one case
          where an early exit is justified.
    - The invariant: floorVal always holds the largest value seen so far that
      is < x, and ceilVal the smallest seen so far that is > x. Each is
      overwritten unconditionally, with no "is this better?" guard, which is
      safe precisely because the branch that writes it also moves the range
      in the direction that can only produce better values.
    - Why one search suffices: running 02-find_floor.cpp and 06-find_ceil.cpp
      separately would answer the same question in two passes. They probe the
      same midpoints on the same input (compare their dry-run tables), so
      merging them costs nothing and halves the work.
    - The naive approach scans linearly for both, which is correct but O(n)
      and ignores the sortedness - that is what the unsorted companion file
      is for.

    Algorithm Steps
    ----------------
    1. Set low = 0, high = n - 1, floorVal = -1, ceilVal = -1.
    2. While low <= high, compute mid = low + (high - low) / 2.
    3. If arr[mid] == x, return {x, x} - both answers are x.
    4. If arr[mid] < x: record floorVal = arr[mid], then low = mid + 1.
    5. Else: record ceilVal = arr[mid], then high = mid - 1.
    6. Return {floorVal, ceilVal}, each still -1 if never recorded.

    Time Complexity: O(log n) - one binary search; the live range halves on
                      every probe and each probe does O(1) work. This is the
                      whole gain over the O(n) unsorted version.
    Space Complexity: O(1) - four ints, plus the returned pair.
*/
vector<int> getFloorAndCeil(int x, vector<int>& arr) {
  int n = arr.size();
  int low = 0, high = n - 1;
  int floorVal = -1, ceilVal = -1;

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (arr[mid] == x) {
      return {x, x};  // distance zero on both sides - nothing can beat it
    } else if (arr[mid] < x) {
      floorVal = arr[mid];  // best floor so far; the search now only moves right
      low = mid + 1;
    } else {
      ceilVal = arr[mid];  // best ceil so far; the search now only moves left
      high = mid - 1;
    }
  }

  return {floorVal, ceilVal};
}

int main() {
  vector<int> arr = {3, 4, 4, 7, 8, 10};

  cout << "Array: ";
  for (int v : arr) cout << v << " ";
  cout << endl;

  for (int x : {5, 8, 1, 99}) {
    vector<int> res = getFloorAndCeil(x, arr);
    cout << "x = " << x << "  ->  floor = " << res[0] << ", ceil = " << res[1] << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {3, 4, 4, 7, 8, 10}, x = 5   (n = 6, answer = {4, 7})
    ======================================================================

      index:    0    1    2    3    4    5
      value:    3    4    4    7    8   10
                          ^    ^
                      floor    ceil       (5 sits in the gap between them)

    Tracked state:
      low, high - the inclusive bounds of the part still being searched
      mid       - low + (high - low) / 2, the probe index
      floorVal  - largest value seen so far that is < x; written only on the
                  arr[mid] < x branch
      ceilVal   - smallest value seen so far that is > x; written only on the
                  arr[mid] > x branch

    Initial state: low = 0, high = 5, floorVal = -1, ceilVal = -1

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 5      (6 candidates alive)
      mid          0 + (5 - 0) / 2 = 2
      a[mid]       arr[2] = 4
      compare      4 != 5, and 4 < 5  -> floor material
      record       floorVal = 4                    <-- the eventual answer
      discard      indices 0..2 - index 2 is banked, and arr[0..1] are <= 4
                   so they can only be worse floors
      update       low = mid + 1 = 3

                     3   4   4 | 7   8  10
                   [ x   x   x | .   .   . ]
                                 live range is now 3..5

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 3, high = 5      (3 candidates alive)
      mid          3 + (5 - 3) / 2 = 4
      a[mid]       arr[4] = 8
      compare      8 != 5, and 8 > 5  -> ceil material
      record       ceilVal = 8      (best so far, but probably beatable)
      discard      indices 4..5 (both >= 8, further from x than 8 is)
      update       high = mid - 1 = 3

                     3   4   4   7 | 8  10
                   [ x   x   x   . | x   x ]
                                     live range is now 3..3

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 3, high = 3      (1 candidate alive: arr[3] = 7)
      mid          3 + (3 - 3) / 2 = 3
      a[mid]       arr[3] = 7
      compare      7 != 5, and 7 > 5  -> ceil material, and 7 beats 8
      record       ceilVal = 7                     <-- the eventual answer
      update       high = mid - 1 = 2

                     3   4   4   7   8  10
                             ^   ^
                          high   low    high = 2 < low = 3 -> range empty
      loop test    low = 3 > high = 2  ->  while (low <= high) fails

    ----------------------------------------------------------------------
    RETURN {floorVal, ceilVal} = {4, 7}

      verify: 4 is the largest element <= 5, and 7 the smallest element >= 5.
      Neither was ever probed together - floorVal was fixed at iteration 1
      and ceilVal took two probes to converge, which is the normal pattern:
      the two candidates tighten independently from opposite sides.

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | a[mid] | vs x = 5 | floorVal | ceilVal | discarded |
    |------|-----|------|-----|--------|----------|----------|---------|-----------|
    |  1   |  0  |  5   |  2  |   4    |  4 < 5   |    4     |   -1    | left  0..2|
    |  2   |  3  |  5   |  4  |   8    |  8 > 5   |    4     |    8    | right 4..5|
    |  3   |  3  |  3   |  3  |   7    |  7 > 5   |    4     |    7    | right 3..3|
    |      |  3  |  2   |  -  |   -    | low>high |    4     |    7    | loop ends |

    Step count behind the O(log n) claim:
      6 elements, 3 probes; the live range shrank 6 -> 3 -> 1 -> 0, i.e.
      ceil(log2(6 + 1)) = 3 probes, the worst case for n = 6. The unsorted
      companion would have taken all 6.

    ======================================================================
    Notes
    ======================================================================

    Why the candidates need no "is this better?" guard:
      floorVal = arr[mid] is unconditional, yet it never regresses. The
      branch that writes it also sets low = mid + 1, so every subsequent
      probe has a strictly larger index - and in a sorted array that means a
      value at least as large. The same argument mirrored covers ceilVal,
      which is why iteration 3 could safely overwrite 8 with 7 while
      iteration 2 could not have gone the other way. Drop the sortedness and
      this reasoning collapses, which is exactly why the unsorted version
      must compare explicitly before recording.

    Why returning {x, x} on an exact hit is safe - and why it is the ONLY
    justified early exit here:
      floor is the largest element <= x and ceil the smallest element >= x.
      If x itself is in the array, both definitions are satisfied by x at
      distance zero, so no further probing can improve either answer. Every
      other branch is a near miss and must keep searching - stopping there
      would report whichever candidate happened to be found first rather
      than the closest one. Iteration 2 is the concrete proof: returning 8
      at that point would have been wrong by one probe.

    On duplicates:
      the exact-hit branch returns the VALUE x, not an index, so which of
      several equal elements the probe landed on is irrelevant - {4, 4} for
      x = 4 is correct no matter whether index 1 or 2 was hit. This is why
      returning values rather than indices makes the duplicate question
      disappear here, while 02-find_floor.cpp has to be explicit about
      which index it reports.

    The -1 sentinel is safe here, unlike in the unsorted version:
      floorVal and ceilVal are only ever assigned from arr[mid] and are
      never READ BACK as a test for "unset" - the code never compares them
      against -1. So an array containing -1 cannot confuse them. The
      unsorted companion does test its accumulator before overwriting, which
      is precisely why it needs boolean flags instead; see its Notes for the
      case that breaks.

    Edge cases:
      - x below everything (x = 1): the floor branch never fires and {-1, 3}
        comes back.
      - x above everything (x = 99): the ceil branch never fires and
        {10, -1} comes back.
      - empty array: n = 0 makes high = -1, the loop never runs, and
        {-1, -1} is returned.

    Companion files:
      02-find_floor.cpp and 06-find_ceil.cpp are this search split in two,
      each returning an INDEX instead of a value and each using the single
      test arr[mid] <= x. 04-find_floor_and_ceil_unsorted.cpp answers the
      same question in O(n) when sortedness cannot be assumed.
*/
