#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an UNSORTED array arr and a value x, return {floor, ceil} as
// VALUES (not indices), where floor is the largest element <= x and ceil is
// the smallest element >= x. Return -1 in either slot when no such element
// exists. Note that when x is present in the array it is both the floor and
// the ceil.
//
// Example:
// arr = {5, 6, 8, 9, 6, 5, 5, 6}, x = 7 -> {6, 8}
// arr = {5, 6, 8, 9, 6, 5, 5, 6}, x = 6 -> {6, 6}   (x present: both are x)
// arr = {5, 6, 8, 9, 6, 5, 5, 6}, x = 2 -> {-1, 5}  (nothing is <= 2)

/*
    Approach: Single linear scan keeping the best candidate on each side

    - The array is UNSORTED, so binary search does not apply: there is no
      half that can be discarded, because a qualifying element may sit
      anywhere. Every element must be looked at at least once, which puts a
      hard O(n) lower bound on any single-query solution.
    - Given that, one pass is enough. Each element is independently tested
      against both questions, and the two answers never interfere:
        * num <= x makes num a floor candidate; keep it only if it is
          LARGER than the best floor so far (closer to x from below).
        * num >= x makes num a ceil candidate; keep it only if it is
          SMALLER than the best ceil so far (closer to x from above).
      An element equal to x satisfies both tests and correctly becomes both
      answers, which is why the comparisons are <= and >= rather than < and >.
    - The invariant: after processing the first k elements, floor and ceil
      hold the exact answers for the prefix arr[0..k-1]. Extending the prefix
      by one element can only improve them, never invalidate them, so the
      answer for the whole array falls out when the loop ends.
    - Why the sorted version is not applicable, and what it would cost:
      sorting first to reuse the O(log n) search costs O(n log n), strictly
      worse than this O(n) scan for a SINGLE query. Sorting only pays off
      when many queries are asked against the same array - then the O(n log n)
      is amortised and each query drops to O(log n). That trade-off is the
      real difference between this file and 05-find_floor_and_ceil.cpp.
    - Tracking "is anything recorded yet" with a boolean rather than with a
      -1 sentinel matters: -1 is a legal array VALUE, so testing
      `floor == -1` to mean "unset" silently corrupts the answer on arrays
      containing negatives (see the Notes for a concrete failing case). The
      -1 only belongs in the RETURNED result, as the problem's contract.

    Algorithm Steps
    ----------------
    1. Set floorVal = ceilVal = -1 and hasFloor = hasCeil = false.
    2. For each num in arr:
       a. If num <= x and (!hasFloor or num > floorVal): floorVal = num,
          hasFloor = true.
       b. If num >= x and (!hasCeil or num < ceilVal): ceilVal = num,
          hasCeil = true.
    3. Return {floorVal, ceilVal} - each still -1 if its flag was never set.

    Time Complexity: O(n) - one pass, two comparisons per element, and no
                      element can be skipped since the array is unsorted.
    Space Complexity: O(1) - two values and two flags, plus the returned pair.
*/
vector<int> getFloorAndCeil(int x, vector<int>& arr) {
  int floorVal = -1, ceilVal = -1;
  bool hasFloor = false, hasCeil = false;  // -1 is a legal value, so track "unset" separately

  for (int num : arr) {
    if (num <= x && (!hasFloor || num > floorVal)) {
      floorVal = num;  // closer to x from below than anything seen so far
      hasFloor = true;
    }

    if (num >= x && (!hasCeil || num < ceilVal)) {
      ceilVal = num;  // closer to x from above than anything seen so far
      hasCeil = true;
    }
  }

  return {floorVal, ceilVal};
}

int main() {
  vector<int> arr = {5, 6, 8, 9, 6, 5, 5, 6};

  cout << "Array: ";
  for (int v : arr) cout << v << " ";
  cout << endl;

  for (int x : {7, 6, 2, 10}) {
    vector<int> res = getFloorAndCeil(x, arr);
    cout << "x = " << x << "  ->  floor = " << res[0] << ", ceil = " << res[1] << endl;
  }

  // negatives: the case a -1 sentinel would get wrong
  vector<int> neg = {-1, -4, 5};
  cout << endl << "Array: ";
  for (int v : neg) cout << v << " ";
  cout << endl;
  vector<int> res = getFloorAndCeil(-3, neg);
  cout << "x = -3  ->  floor = " << res[0] << ", ceil = " << res[1]
       << "   (expected floor = -4, ceil = -1)" << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {5, 6, 8, 9, 6, 5, 5, 6}, x = 7   (n = 8, answer = {6, 8})
    ======================================================================

      index:    0    1    2    3    4    5    6    7
      value:    5    6    8    9    6    5    5    6
                     ^    ^
                 floor    ceil        (values 6 and 8; note the array is
                                       NOT sorted, so these indices carry
                                       no positional meaning)

    Tracked state:
      num       - the element currently being examined
      floorVal  - largest value seen so far that is <= x
      ceilVal   - smallest value seen so far that is >= x
      hasFloor  - whether floorVal has been written at all
      hasCeil   - whether ceilVal has been written at all

    Initial state: floorVal = -1, ceilVal = -1, hasFloor = false, hasCeil = false

    ----------------------------------------------------------------------
    i = 0, num = 5
      floor test   5 <= 7 yes, and hasFloor is false -> first candidate
      record       floorVal = 5, hasFloor = true
      ceil test    5 >= 7 no  -> skipped
      state        floor = 5, ceil = unset

    ----------------------------------------------------------------------
    i = 1, num = 6
      floor test   6 <= 7 yes, and 6 > 5 -> strictly closer to x from below
      record       floorVal = 6                      <-- the eventual answer
      ceil test    6 >= 7 no  -> skipped
      state        floor = 6, ceil = unset

    ----------------------------------------------------------------------
    i = 2, num = 8
      floor test   8 <= 7 no  -> skipped
      ceil test    8 >= 7 yes, and hasCeil is false -> first candidate
      record       ceilVal = 8, hasCeil = true       <-- the eventual answer
      state        floor = 6, ceil = 8

    ----------------------------------------------------------------------
    i = 3, num = 9
      floor test   9 <= 7 no  -> skipped
      ceil test    9 >= 7 yes, but 9 < 8 is false -> 9 is FURTHER from x
      record       nothing
      state        floor = 6, ceil = 8

    ----------------------------------------------------------------------
    i = 4, num = 6
      floor test   6 <= 7 yes, but 6 > 6 is false -> ties do not replace
      ceil test    6 >= 7 no  -> skipped
      record       nothing
      state        floor = 6, ceil = 8

    ----------------------------------------------------------------------
    i = 5, num = 5    and    i = 6, num = 5
      floor test   5 <= 7 yes, but 5 > 6 is false -> worse than what we hold
      ceil test    5 >= 7 no  -> skipped
      record       nothing (both iterations)
      state        floor = 6, ceil = 8

    ----------------------------------------------------------------------
    i = 7, num = 6
      floor test   6 <= 7 yes, but 6 > 6 is false
      ceil test    6 >= 7 no
      record       nothing - the last four elements changed nothing at all
      state        floor = 6, ceil = 8

    ----------------------------------------------------------------------
    RETURN {6, 8}

    ======================================================================
    Summary table
    ======================================================================

    | i | num | num <= 7 | beats floor? | num >= 7 | beats ceil? | floor | ceil |
    |---|-----|----------|--------------|----------|-------------|-------|------|
    | 0 |  5  |   yes    | yes (first)  |   no     |     -       |   5   |  -   |
    | 1 |  6  |   yes    | yes (6 > 5)  |   no     |     -       |   6   |  -   |
    | 2 |  8  |   no     |      -       |   yes    | yes (first) |   6   |  8   |
    | 3 |  9  |   no     |      -       |   yes    | no (9 > 8)  |   6   |  8   |
    | 4 |  6  |   yes    | no (6 = 6)   |   no     |     -       |   6   |  8   |
    | 5 |  5  |   yes    | no (5 < 6)   |   no     |     -       |   6   |  8   |
    | 6 |  5  |   yes    | no (5 < 6)   |   no     |     -       |   6   |  8   |
    | 7 |  6  |   yes    | no (6 = 6)   |   no     |     -       |   6   |  8   |

    Step count behind the O(n) claim:
      8 elements, 8 iterations, exactly 2 comparisons each - 16 in total,
      with no early exit. Note that the answer was already final after
      i = 2, but the loop cannot stop there: in an unsorted array the very
      last element could still be a 7.

    ======================================================================
    Notes
    ======================================================================

    Why both tests use a non-strict comparison:
      with num <= x and num >= x, an element equal to x passes BOTH and
      becomes both the floor and the ceil - which is the definition. Writing
      num < x and num > x instead would return the nearest strict neighbours
      on either side and would never report x itself, silently answering a
      different question. The x = 6 case in main() is the one that catches
      this: it must print {6, 6}.

    Why hasFloor / hasCeil instead of testing against -1:
      -1 is a perfectly legal array value, so `floor == -1` cannot mean both
      "unset" and "the recorded floor happens to be -1". On
      arr = {-1, -4, 5} with x = -3 the sentinel version does this:
        num = -1: -1 >= -3, ceil is "unset" (== -1) -> ceil = -1   correct
        num = -4: -4 <= -3, floor is "unset" (== -1) -> floor = -4 correct
        num =  5:  5 >= -3, ceil == -1 reads as "unset" -> ceil = 5  WRONG
      The true ceil is -1, but the sentinel made the recorded answer look
      like an empty slot and let a worse value overwrite it. Boolean flags
      separate "nothing recorded" from "the recorded value is -1", so the
      bug cannot occur. It stays hidden on the usual constraint
      1 <= arr[i] <= 10^9 - which is exactly what makes it worth guarding.

    Why the two questions cannot be merged into one branch:
      it is tempting to write an if / else if chain, but an element equal to
      x must fall into both arms. Two independent `if`s is not redundancy -
      it is what makes the x-present case correct.

    Edge cases:
      - x smaller than everything: no element passes num <= x, hasFloor
        stays false, and floor comes back -1 (the x = 2 case in main()).
      - x larger than everything: symmetric, ceil comes back -1 (x = 10).
      - empty array: the loop body never runs and {-1, -1} is returned.
      - duplicates: they hit the "does not beat" branch and are ignored, so
        the repeated 5s and 6s in the trace cost time but change nothing.

    Companion file:
      05-find_floor_and_ceil.cpp answers the same question when the array IS
      sorted, replacing this O(n) scan with one O(log n) binary search that
      maintains the same two candidates.
*/
