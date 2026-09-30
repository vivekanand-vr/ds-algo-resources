#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: (InterviewBit Painter's Partition) There are `painters` painters and
// boards[i] is the length of the i-th board. Every painter takes
// `unitTime` units of time to paint 1 unit of board. All painters work at
// the same time, and each one may only paint a CONTIGUOUS run of boards
// (in the given order). Return the MINIMUM time needed to paint every
// board, modulo 10000003.
//
// Example:
// painters = 2,   unitTime = 5, boards = {1, 10}           -> 50
// painters = 10,  unitTime = 1, boards = {1, 8, 11, 3}     -> 11
// painters = 2,   unitTime = 1, boards = {10, 20, 30, 40}  -> 60
//                               ({10, 20, 30} and {40})

/*
    Approach: Binary search on the answer (the time limit)

    - Convert every board to its painting time, length * unitTime. The
      problem then becomes: split the times into at most `painters`
      contiguous groups and minimise the largest group sum.
    - For a time limit mid, count the painters needed greedily: keep
      giving boards to the current painter while their total stays <= mid,
      and bring in a new painter when the next board does not fit. A
      larger limit never needs more painters, so the predicate
      "painters needed <= painters" is false and then true. We want the
      FIRST true.
    - The answer lies in max(board time)..sum(board times):
        below the longest board's time that board can never be finished;
        with the sum a single painter paints everything.
    - Needing FEWER painters than available is fine - the extra painters
      simply stay idle.
    - Probe mid:
        paintersNeeded <= painters  ->  mid works, a smaller limit might
                                        too: high = mid.
        paintersNeeded >  painters  ->  mid is too small: low = mid + 1.
      This is 10-split_array_largest_sum.cpp with every element scaled by
      unitTime.

    Algorithm Steps
    ----------------
    1. low = max(length * unitTime), high = sum(length * unitTime).
    2. While low < high, compute mid = low + (high - low) / 2.
    3. Simulate: paintersNeeded = 1, currentTime = 0. For each board, if
       currentTime + boardTime <= mid, add it; else paintersNeeded++ and
       currentTime = boardTime.
    4. If paintersNeeded <= painters, high = mid.
    5. Else low = mid + 1.
    6. Return low % 10000003.

    Time Complexity: O(n * log(sum - max)) - one O(n) simulation per probe.
    Space Complexity: O(1)
*/
int paint(int painters, int unitTime, vector<int>& boards) {
  const long long MOD = 10000003;

  long long low = 0;
  long long high = 0;

  // Minimum possible time = longest single board
  // Maximum possible time = painting all boards with one painter
  for (int length : boards) {
    long long boardTime = 1LL * length * unitTime;
    low = max(low, boardTime);
    high += boardTime;
  }

  while (low < high) {
    long long mid = low + (high - low) / 2;  // to prevent overflow

    int paintersNeeded = 1;
    long long currentTime = 0;  // time the current painter has taken on

    for (int length : boards) {
      long long boardTime = 1LL * length * unitTime;

      if (currentTime + boardTime <= mid) {
        currentTime += boardTime;  // same painter takes this board
      } else {
        paintersNeeded++;  // need another painter
        currentTime = boardTime;
      }
    }

    if (paintersNeeded <= painters) {
      high = mid;  // mid is possible, try a smaller maximum time
    } else {
      low = mid + 1;  // need more painters, so increase allowed time
    }
  }

  return low % MOD;  // reduce only at the end, the search needs real times
}

struct Test {
  int painters, unitTime;
  vector<int> boards;
};

int main() {
  vector<Test> tests = {{2, 5, {1, 10}}, {10, 1, {1, 8, 11, 3}}, {2, 1, {10, 20, 30, 40}}};

  for (auto& [painters, unitTime, boards] : tests) {
    cout << "Boards: ";
    for (int v : boards) cout << v << " ";
    cout << " painters = " << painters << ", unitTime = " << unitTime << endl;
    cout << "  minimum time: " << paint(painters, unitTime, boards) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of paint: painters = 2, unitTime = 1, boards = {10, 20, 30, 40}
    (answer = 60)
    ======================================================================

    Board times:     10  20  30  40
    Initial state:   low = 40, high = 10 + 20 + 30 + 40 = 100

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 40, high = 100
      mid          40 + (100 - 40) / 2 = 70
      assigning    painter 1: 10, 20, 30         (60)
                   60 + 40 = 100 > 70  ->  painter 2: 40
      result       2 painters <= 2  ->  fits
      update       high = 70

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 40, high = 70
      mid          40 + (70 - 40) / 2 = 55
      assigning    painter 1: 10, 20             (30)
                   30 + 30 = 60 > 55   ->  painter 2: 30
                   30 + 40 = 70 > 55   ->  painter 3: 40
      result       3 painters > 2  ->  too small
      update       low = 56

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 56, high = 70
      mid          56 + (70 - 56) / 2 = 63
      assigning    painter 1: 10, 20, 30         (60)
                   60 + 40 = 100 > 63  ->  painter 2: 40
      result       2 painters  ->  fits
      update       high = 63

    ----------------------------------------------------------------------
    Iteration 4
      bounds       low = 56, high = 63
      mid          56 + (63 - 56) / 2 = 59
      assigning    painter 1: 10, 20             (30)
                   30 + 30 = 60 > 59   ->  painter 2: 30
                   30 + 40 = 70 > 59   ->  painter 3: 40
      result       3 painters  ->  too small
      update       low = 60

    ----------------------------------------------------------------------
    Iteration 5
      bounds       low = 60, high = 63
      mid          60 + (63 - 60) / 2 = 61
      assigning    painter 1: 10, 20, 30         (60)
                   60 + 40 = 100 > 61  ->  painter 2: 40
      result       2 painters  ->  fits
      update       high = 61

    ----------------------------------------------------------------------
    Iteration 6
      bounds       low = 60, high = 61
      mid          60 + (61 - 60) / 2 = 60
      assigning    painter 1: 10, 20, 30         (60 <= 60)
                   60 + 40 = 100 > 60  ->  painter 2: 40
      result       2 painters  ->  fits
      update       high = 60

    ----------------------------------------------------------------------
    Loop ends with low = high = 60.  RETURN 60 % 10000003 = 60

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | assignment              | painters | action     |
    |------|-----|------|-----|-------------------------|----------|------------|
    |  1   |  40 | 100  |  70 | {10,20,30} {40}         |    2     | high = 70  |
    |  2   |  40 |  70  |  55 | {10,20} {30} {40}       |    3     | low  = 56  |
    |  3   |  56 |  70  |  63 | {10,20,30} {40}         |    2     | high = 63  |
    |  4   |  56 |  63  |  59 | {10,20} {30} {40}       |    3     | low  = 60  |
    |  5   |  60 |  63  |  61 | {10,20,30} {40}         |    2     | high = 61  |
    |  6   |  60 |  61  |  60 | {10,20,30} {40}         |    2     | high = 60  |

    ======================================================================
    Notes
    ======================================================================

    Why the modulo is taken only at the end:
      the binary search compares real times, so low, high and mid must stay
      unreduced. Only the final answer is reported modulo 10000003.

    Overflow: length and unitTime can each be large, so every board time
      is computed as 1LL * length * unitTime before it is added up.

    Factoring out unitTime:
      every time is a multiple of unitTime, so an equivalent solution
      searches on board lengths (exactly 10-split_array_largest_sum.cpp)
      and multiplies the answer by unitTime at the end.

    Same problem, different names:
      09-allocate_minimum_pages.cpp (books -> students) and
      10-split_array_largest_sum.cpp (elements -> subarrays).

    Edge cases:
      - painters >= number of boards: each board gets its own painter ->
        the longest board's time (example 2: 11 * 1 = 11).
      - painters == 1: one painter paints everything -> the total time.
*/
