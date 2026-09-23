#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array nums and a target, return the starting and ending
// index of target as {first, last}. Return {-1, -1} if the target is not
// present. The solution must run in O(log n).
//
// Example:
// nums = {5, 7, 7, 8, 8, 10}, target = 8  -> {3, 4}
// nums = {5, 7, 7, 8, 8, 10}, target = 7  -> {1, 2}
// nums = {5, 7, 7, 8, 8, 10}, target = 6  -> {-1, -1}  (absent)
// nums = {}, target = 0                   -> {-1, -1}  (empty array)

/*
    Approach: Two lower-bound searches, at target and at target + 1

    - Plain binary search is not enough on its own: with duplicates it returns
      SOME index holding the target, chosen by wherever the probes happened to
      land, with no way to tell whether it is the first, the last, or one in
      the middle. Walking outward from that hit to find the ends is O(n) in
      the worst case (an array of all equal values), which breaks the required
      bound. Both ends must be found by searching, not by scanning.
    - The helper below is a LOWER BOUND, despite its plain name: it returns
      the number of elements strictly less than x, i.e. the index of the first
      element >= x, or n if there is none. It never returns -1 and never
      returns early on a hit - it always runs the range down to empty, which
      is what makes its answer a position rather than a lucky index.
    - That single primitive answers both halves of the question:
        first = lowerBound(target)         - the first index holding target
        last  = lowerBound(target + 1) - 1 - one before the first index
                                            holding something bigger
      The second line is the trick worth remembering. Since the array is
      sorted and holds integers, "the first element > target" is the same as
      "the first element >= target + 1", so the same helper locates it. Step
      back one index and you are on the last target.
    - The presence check has to come last, because the arithmetic above is
      unconditional and produces a plausible-looking pair even for an absent
      target. If target is missing, lowerBound(target) and lowerBound(target+1)
      return the SAME insertion position p, so first = p and last = p - 1, a
      backwards range. Testing nums[first] == target rejects exactly those
      cases - and it has to be guarded by first < n first, since p can be n
      when the target is larger than everything.
    - Why the invariant holds: on arr[mid] < x the loop discards mid and
      everything left of it (all provably < x); otherwise it discards mid and
      everything right of it (all provably >= x). low therefore only ever
      counts elements known to be < x, and when the range empties, low is the
      exact count - the lower bound.

    Algorithm Steps
    ----------------
    1. lowerBound(arr, x): set low = 0, high = n - 1; while low <= high, take
       mid; if arr[mid] < x then low = mid + 1 else high = mid - 1; return low.
    2. first = lowerBound(nums, target).
    3. last  = lowerBound(nums, target + 1) - 1.
    4. If first < n and nums[first] == target, return {first, last}.
    5. Otherwise return {-1, -1}.

    Time Complexity: O(log n) - two independent binary searches, each halving
                      the range per probe. 2 * O(log n) is still O(log n).
    Space Complexity: O(1) - a handful of indices; the returned pair aside, no
                      auxiliary storage.
*/

// Lower bound: the count of elements strictly less than x, which is also the
// index of the first element >= x (or n when every element is smaller).
int binarySearch(vector<int>& arr, int x) {
  int n = arr.size();
  int low = 0, high = n - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;  // to prevent overflow

    if (arr[mid] < x) {
      low = mid + 1;
    } else {
      high = mid - 1;  // mid may itself be the answer, so it is not skipped over
    }
  }

  return low;  // never -1: on a miss this is where x would be inserted
}

vector<int> searchRange(vector<int>& nums, int target) {
  int n = nums.size();
  int first = binarySearch(nums, target);
  int last = binarySearch(nums, target + 1) - 1;  // one before the first element > target

  // On an absent target the two bounds coincide, leaving last = first - 1;
  // this check is what rejects that backwards range.
  if (first < n && nums[first] == target) return {first, last};
  return {-1, -1};
}

int main() {
  vector<int> nums = {5, 7, 7, 8, 8, 10};

  cout << "Array: ";
  for (int v : nums) cout << v << " ";
  cout << endl;

  for (int target : {8, 7, 5, 10, 6, 11}) {
    vector<int> res = searchRange(nums, target);
    cout << "target = " << target << "  ->  {" << res[0] << ", " << res[1] << "}" << endl;
  }

  vector<int> empty;
  vector<int> res = searchRange(empty, 0);
  cout << endl << "empty array, target = 0  ->  {" << res[0] << ", " << res[1] << "}" << endl;

  vector<int> same = {2, 2, 2, 2};
  vector<int> res2 = searchRange(same, 2);
  cout << "all-equal array {2,2,2,2}, target = 2  ->  {" << res2[0] << ", " << res2[1] << "}"
       << "   (the case an outward scan would do in O(n))" << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: nums = {5, 7, 7, 8, 8, 10}, target = 8   (n = 6, answer = {3, 4})
    ======================================================================

      index:    0    1    2    3    4    5
      value:    5    7    7    8    8   10
                               ^    ^
                           first    last

    Tracked state:
      low, high - the inclusive bounds of the part still being searched
      mid       - low + (high - low) / 2, the probe index
      (no candidate variable exists - low IS the running answer, and it is
       always the count of elements proven to be < x)

    Two searches run, one after the other. Both are traced.

    ======================================================================
    CALL 1: binarySearch(nums, 8)   -> expect 3, the FIRST index holding 8
    ======================================================================

    Initial state: low = 0, high = 5

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 5      (6 candidates alive)
      mid          0 + (5 - 0) / 2 = 2
      a[mid]       nums[2] = 7
      compare      7 < 8  -> mid and everything left of it are too small
      discard      indices 0..2
      update       low = mid + 1 = 3      (3 elements now proven < 8)

                     5   7   7 | 8   8  10
                   [ x   x   x | .   .   . ]
                                 live range is now 3..5

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 3, high = 5
      mid          3 + (5 - 3) / 2 = 4
      a[mid]       nums[4] = 8
      compare      8 < 8 is FALSE  -> mid is >= x, so mid itself might be the
                   answer and must NOT be stepped over
      discard      indices 5..5 only (nums[5] = 10 >= 8 but is further right)
      update       high = mid - 1 = 3     (low is untouched, still 3)

                     5   7   7   8 | 8  10
                   [ x   x   x   . | x   x ]
                                     live range is now 3..3

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 3, high = 3      (1 candidate: nums[3] = 8)
      mid          3 + (3 - 3) / 2 = 3
      a[mid]       nums[3] = 8
      compare      8 < 8 is FALSE  -> again, do not step over it
      update       high = mid - 1 = 2
      loop test    low = 3 > high = 2  ->  loop ends

    RETURN low = 3        -> first = 3

    ======================================================================
    CALL 2: binarySearch(nums, 9)   -> expect 5, the first index holding > 8
    ======================================================================
    (target + 1 = 9; nothing in the array equals 9, and that does not
     matter - the lower bound of an absent value is still a valid position)

    Initial state: low = 0, high = 5

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 5
      mid          2,  nums[2] = 7
      compare      7 < 9  -> too small
      update       low = 3

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 3, high = 5
      mid          4,  nums[4] = 8
      compare      8 < 9  -> too small; note this is the branch that CALL 1
                   did not take at the same index, and it is the entire
                   difference between the two searches
      update       low = 5

                     5   7   7   8   8 | 10
                   [ x   x   x   x   x | . ]
                                         live range is now 5..5

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 5, high = 5      (1 candidate: nums[5] = 10)
      mid          5,  nums[5] = 10
      compare      10 < 9 is FALSE
      update       high = 4
      loop test    low = 5 > high = 4  ->  loop ends

    RETURN low = 5        -> last = 5 - 1 = 4

    ======================================================================
    Final assembly
    ======================================================================

      first = 3
      last  = 5 - 1 = 4
      check   first = 3 < n = 6, and nums[3] = 8 == target  -> present

    RETURN {3, 4}

      5   7   7   8   8  10
                  |---|
               first   last      both 8s, and nothing outside that span

    ======================================================================
    Summary table
    ======================================================================

    | call        | iter | low | high | mid | a[mid] | test      | action     |
    |-------------|------|-----|------|-----|--------|-----------|------------|
    | lb(8)       |  1   |  0  |  5   |  2  |   7    | 7 < 8  T  | low = 3    |
    | lb(8)       |  2   |  3  |  5   |  4  |   8    | 8 < 8  F  | high = 3   |
    | lb(8)       |  3   |  3  |  3   |  3  |   8    | 8 < 8  F  | high = 2   |
    | lb(8)       |  -   |  3  |  2   |  -  |   -    | low > high| return 3   |
    | lb(9)       |  1   |  0  |  5   |  2  |   7    | 7 < 9  T  | low = 3    |
    | lb(9)       |  2   |  3  |  5   |  4  |   8    | 8 < 9  T  | low = 5    |
    | lb(9)       |  3   |  5  |  5   |  5  |  10    | 10 < 9 F  | high = 4   |
    | lb(9)       |  -   |  5  |  4   |  -  |   -    | low > high| return 5   |

    Step count behind the O(log n) claim:
      3 probes per call on 6 elements, 6 in total; each live range shrank
      6 -> 3 -> 1 -> 0. Two searches double the constant, not the order.

    ======================================================================
    Notes
    ======================================================================

    Why the helper has no early return and no candidate variable:
      it never asks "did I find the target?" - only "is this element too
      small?". So there is nothing to return early ON. low accumulates the
      count of elements proven < x and is the answer by construction when
      the range empties. That is what makes it usable for an ABSENT value,
      which the target + 1 call relies on: 9 is not in the array, yet
      lowerBound(9) = 5 is still exactly the position we need.

    Why last = lowerBound(target + 1) - 1 and not a separate upper-bound
    search:
      on integers, "first element > target" and "first element >= target + 1"
      are the same position, so one primitive covers both ends and there is
      no second loop to get subtly wrong. The trade-off is that it only works
      on integer keys - on doubles or strings there is no "+1", and a real
      upper-bound search (flip the test to arr[mid] <= x) is required.

    A dry run of the ABSENT case, target = 6:
      lowerBound(6) = 1   (one element, the 5, is < 6)
      lowerBound(7) = 1   (still just the 5)
      first = 1, last = 1 - 1 = 0    -> a backwards range {1, 0}
      the guard catches it: nums[1] = 7 != 6, so {-1, -1} is returned.
      This is why the check cannot be skipped - the arithmetic itself never
      signals failure, it just produces a range where last < first.

    Why first < n must be tested BEFORE nums[first]:
      when target is larger than every element (target = 11 here),
      lowerBound returns n = 6 and nums[6] is out of bounds. C++ evaluates
      && left to right and short-circuits, so the bounds test written first
      prevents the read. Swapping the two operands is undefined behaviour,
      not a style issue. Comparing against an int n rather than nums.size()
      also sidesteps the signed/unsigned comparison warning.

    The one input constraint this approach carries:
      target + 1 overflows if target is INT_MAX. LeetCode 34 bounds the
      values by 10^9 so it cannot arise there, but if the range were the
      full int domain, either special-case INT_MAX or do the +1 in a wider
      type.

    Edge cases:
      - empty array: n = 0, high = -1, the loop never runs, both calls
        return 0, and first = 0 < n = 0 is false -> {-1, -1}.
      - target present once (target = 5 or 10): first == last, giving a
        one-element range.
      - all elements equal to the target ({2,2,2,2}): lowerBound(2) = 0 and
        lowerBound(3) = 4, so {0, 3} - found in 2*log n probes, whereas
        expanding outward from a plain binary-search hit would have touched
        all 4 elements. This is the case that justifies the whole approach.

    Companion files:
      03-search_insert_position.cpp computes this same lower bound, and its
      answer IS first - the insert position and the first occurrence are the
      same index whenever the target is present. 02-find_floor.cpp and
      06-find_ceil.cpp solve the neighbouring near-miss questions with a
      recorded candidate instead of a converging bound.
*/
