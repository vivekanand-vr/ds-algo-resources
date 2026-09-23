#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array nums and a target, count how many times target
// occurs in nums. Return 0 if it does not occur at all. The solution must run
// in O(log n).
//
// Example:
// nums = {1, 2, 2, 2, 3, 4}, target = 2  -> 3
// nums = {1, 2, 2, 2, 3, 4}, target = 4  -> 1
// nums = {1, 2, 2, 2, 3, 4}, target = 5  -> 0   (absent, above the range)
// nums = {2, 2, 2, 2},       target = 2  -> 4   (the whole array)

/*
    Approach: Subtract two lower bounds - count = lb(target + 1) - lb(target)

    - The helper below is a LOWER BOUND despite its plain name: it returns the
      number of elements strictly less than x, which is also the index of the
      first element >= x (or n when every element is smaller). It never
      returns -1 and never exits early on a hit, so it answers for values that
      are not in the array at all - which is what makes the trick below work.
    - Read the two calls as counts rather than as positions, and the whole
      problem collapses to one subtraction:
          lb(target)     = how many elements are  <  target
          lb(target + 1) = how many elements are <= target   (integers, so
                           "< target + 1" and "<= target" are the same set)
      Everything in the second count that is not in the first is an element
      equal to target. So the difference IS the answer, with no index
      arithmetic and no special cases.
    - The absent case needs no handling. If target does not occur, nothing is
      <= target that was not already < target, the two bounds return the same
      position, and the difference is 0 on its own. This is the one real
      simplification over 07-first_last_position_in_sorted.cpp: that file must
      report INDICES, and {-1, -1} is a genuinely different answer from the
      backwards pair {p, p - 1} the arithmetic produces on a miss, so it has
      to test nums[first] == target to tell them apart. A count has no such
      ambiguity - a backwards range and an empty range both mean zero - so the
      guard, the bounds check and the +1 all disappear.
    - Why not find the two ends and subtract indices: that is the same idea
      expressed one step further from the definition (last - first + 1, valid
      only after proving the target is present). Counting elements rather than
      locating them removes the off-by-one and the guard it depends on.
    - The naive approach binary-searches for any occurrence and then walks
      outward over the equal neighbours. Correct, but the walk is O(n) - on
      {2, 2, 2, 2} it touches every element, which defeats the required bound.
      Both ends must be found by searching, not by scanning.

    Algorithm Steps
    ----------------
    1. lowerBound(arr, x): set low = 0, high = n - 1; while low <= high, take
       mid = low + (high - low) / 2; if arr[mid] < x then low = mid + 1, else
       high = mid - 1; return low.
    2. Return lowerBound(nums, target + 1) - lowerBound(nums, target).

    Time Complexity: O(log n) - two independent binary searches, each halving
                      the live range per probe. 2 * O(log n) is still O(log n).
    Space Complexity: O(1) - a few indices; nothing is allocated.
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

int countFreq(vector<int>& nums, int target) {
  // (# elements <= target) - (# elements < target) = (# elements == target),
  // which is already 0 when target is absent - no presence check needed.
  return binarySearch(nums, target + 1) - binarySearch(nums, target);
}

int main() {
  vector<int> nums = {1, 2, 2, 2, 3, 4};

  cout << "Array: ";
  for (int v : nums) cout << v << " ";
  cout << endl;

  for (int target : {2, 4, 1, 5, 0, 3}) {
    cout << "count of " << target << ": " << countFreq(nums, target) << endl;
  }

  vector<int> same = {2, 2, 2, 2};
  cout << endl << "all-equal array {2,2,2,2}, target = 2  ->  " << countFreq(same, 2)
       << "   (the case an outward scan would do in O(n))" << endl;

  vector<int> empty;
  cout << "empty array, target = 2  ->  " << countFreq(empty, 2) << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: nums = {1, 2, 2, 2, 3, 4}, target = 2   (n = 6, answer = 3)
    ======================================================================

      index:    0    1    2    3    4    5
      value:    1    2    2    2    3    4
                     |---------|
                     the three 2s, at indices 1..3

    Tracked state:
      low, high - the inclusive bounds of the part still being searched
      mid       - low + (high - low) / 2, the probe index
      (there is no candidate variable: low IS the running answer, and it is
       always the count of elements already proven to be < x)

    Two searches run, one after the other. Both are traced, then subtracted.

    ======================================================================
    CALL 1: binarySearch(nums, 2)   -> expect 1, since only the 1 is < 2
    ======================================================================

    Initial state: low = 0, high = 5

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 5      (6 candidates alive)
      mid          0 + (5 - 0) / 2 = 2
      a[mid]       nums[2] = 2
      compare      2 < 2 is FALSE  -> mid is >= x, so mid itself may be the
                   answer and must NOT be stepped over
      discard      indices 2..5
      update       high = mid - 1 = 1     (low untouched, still 0)

                     1   2 | 2   2   3   4
                   [ .   . | x   x   x   x ]
                             live range is now 0..1

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 0, high = 1
      mid          0 + (1 - 0) / 2 = 0
      a[mid]       nums[0] = 1
      compare      1 < 2  -> too small, so index 0 is counted and dropped
      update       low = mid + 1 = 1      (1 element now proven < 2)

                     1 | 2   2   2   3   4
                   [ x | .   x   x   x   x ]
                         live range is now 1..1

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 1, high = 1      (1 candidate: nums[1] = 2)
      mid          1 + (1 - 1) / 2 = 1
      a[mid]       nums[1] = 2
      compare      2 < 2 is FALSE
      update       high = mid - 1 = 0
      loop test    low = 1 > high = 0  ->  loop ends

    RETURN low = 1        -> exactly one element (the 1) is < 2

    ======================================================================
    CALL 2: binarySearch(nums, 3)   -> expect 4, since 1, 2, 2, 2 are all < 3
    ======================================================================
    (target + 1 = 3, and note 3 IS present here - irrelevant either way, the
     lower bound is a position, not a search for a hit)

    Initial state: low = 0, high = 5

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 0, high = 5
      mid          2,  nums[2] = 2
      compare      2 < 3  -> too small; this is the branch CALL 1 did not
                   take at the same index, and it is the entire difference
                   between the two searches
      update       low = 3

                     1   2   2 | 2   3   4
                   [ x   x   x | .   .   . ]
                                 live range is now 3..5

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 3, high = 5
      mid          3 + (5 - 3) / 2 = 4
      a[mid]       nums[4] = 3
      compare      3 < 3 is FALSE
      update       high = mid - 1 = 3

                     1   2   2   2 | 3   4
                   [ x   x   x   . | x   x ]
                                     live range is now 3..3

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 3, high = 3      (1 candidate: nums[3] = 2)
      mid          3,  nums[3] = 2
      compare      2 < 3  -> too small
      update       low = mid + 1 = 4      (4 elements now proven < 3)
      loop test    low = 4 > high = 3  ->  loop ends

    RETURN low = 4        -> four elements (1, 2, 2, 2) are < 3

    ======================================================================
    Final assembly
    ======================================================================

      lb(3) = 4     elements <= 2   ->  {1, 2, 2, 2}
      lb(2) = 1     elements <  2   ->  {1}
      count = 4 - 1 = 3             ->  the three 2s, which is the answer

      1 | 2   2   2 | 3   4
      ^   \---------/
    lb(2)    counted     lb(3) = 4 is the index just past them

    RETURN 3

    ======================================================================
    Summary table
    ======================================================================

    | call  | iter | low | high | mid | a[mid] | test      | action     |
    |-------|------|-----|------|-----|--------|-----------|------------|
    | lb(2) |  1   |  0  |  5   |  2  |   2    | 2 < 2  F  | high = 1   |
    | lb(2) |  2   |  0  |  1   |  0  |   1    | 1 < 2  T  | low = 1    |
    | lb(2) |  3   |  1  |  1   |  1  |   2    | 2 < 2  F  | high = 0   |
    | lb(2) |  -   |  1  |  0   |  -  |   -    | low > high| return 1   |
    | lb(3) |  1   |  0  |  5   |  2  |   2    | 2 < 3  T  | low = 3    |
    | lb(3) |  2   |  3  |  5   |  4  |   3    | 3 < 3  F  | high = 3   |
    | lb(3) |  3   |  3  |  3   |  3  |   2    | 2 < 3  T  | low = 4    |
    | lb(3) |  -   |  5  |  3   |  -  |   -    | low > high| return 4   |

    Step count behind the O(log n) claim:
      3 probes per call on 6 elements, 6 in total; each live range shrank
      6 -> 2 -> 1 -> 0 and 6 -> 3 -> 1 -> 0. Two searches double the
      constant, not the order.

    ======================================================================
    Notes
    ======================================================================

    Why no presence check is needed - the absent case, target = 5:
      lb(5) = 5   (1, 2, 2, 2, 3 are all < 5)
      lb(6) = 5   (nothing new becomes eligible)
      count = 5 - 5 = 0
      The two bounds collapse onto the same position precisely because no
      element sits between them, and "no element between them" is what
      "absent" means. The subtraction reports it without being asked.
      Compare 07-first_last_position_in_sorted.cpp, which computes the same
      two bounds but must then test nums[first] == target: reporting an
      index pair forces it to distinguish {-1, -1} from {5, 4}, whereas a
      count treats both as zero. Same primitive, one fewer thing to check,
      because the return type carries less information.

    Why the helper has no early return and no candidate variable:
      it never asks "did I find the target?" - only "is this element too
      small?". There is nothing to return early ON. low accumulates the
      count of elements proven < x and is the answer by construction once
      the range empties. That is also why it works for a value absent from
      the array, which the target + 1 call depends on.

    Why low is the count and not merely an index:
      the two branches maintain an invariant - everything strictly left of
      low is known to be < x, and everything strictly right of high is known
      to be >= x. When low > high the two regions meet with nothing
      unclassified between them, so low counts exactly the < x elements.
      Reading it as an index and as a count is the same fact stated twice,
      and the count reading is the one that makes this file's subtraction
      obvious.

    On "< x" versus "<= x" in the helper:
      the strict < is what makes this a LOWER bound - equal elements fall to
      the else branch and are kept in play, so low stops at the FIRST of
      them. Changing it to <= would make the helper an upper bound, and then
      the count would be lb_upper(target) - lb_upper(target - 1). Either
      pair works; mixing one of each does not.

    The one input constraint this approach carries:
      target + 1 overflows if target is INT_MAX. Under the usual constraints
      (values bounded by 10^9) it cannot arise, but on a full-int domain
      either special-case INT_MAX or widen the arithmetic. This is now the
      only delicate line in the function.

    Edge cases:
      - target absent, below the range (target = 0): lb(0) = lb(1) = 0 -> 0.
      - target absent, above the range (target = 5): both bounds return
        n = 6 -> 0. Nothing is ever indexed, so the out-of-bounds read that
        07 had to guard against cannot happen here at all.
      - empty array: n = 0 makes high = -1, both loops are skipped, both
        calls return 0, and the count is 0.
      - every element equal to the target ({2,2,2,2}): lb(2) = 0 and
        lb(3) = 4, giving 4 in 2*log n probes - whereas expanding outward
        from a plain binary-search hit would have touched all four. This is
        the case that justifies the whole approach.

    Companion files:
      07-first_last_position_in_sorted.cpp returns the two ends as indices
      using these same two bounds, and needs the presence guard to do it.
      03-search_insert_position.cpp is lb(target) on its own. 02-find_floor
      and 06-find_ceil answer the neighbouring near-miss questions with a
      recorded candidate instead of a converging bound.
*/
