#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Packages on a conveyor belt must be shipped within `days` days.
// weights[i] is the weight of the i-th package. Each day the ship is loaded
// with packages IN THE GIVEN ORDER, and the total loaded may not exceed the
// ship's capacity. Return the LEAST capacity that ships everything in time.
//
// Example:
// weights = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},  days = 5   -> 15
// weights = {3, 2, 2, 4, 1, 4},               days = 3   -> 6
// weights = {1, 2, 3, 1, 1},                  days = 4   -> 3

/*
    Approach: Binary search on the answer (the capacity)

    - For a capacity c, daysNeeded(c) is found greedily: load packages in
      order and start a new day whenever the next one does not fit. A
      bigger ship never needs more days, so the predicate
      "daysNeeded(c) <= days" is false and then true. We want the FIRST
      true.
    - The answer lies in max(weights)..sum(weights):
        below max(weights) the heaviest package never fits on the ship;
        at sum(weights) everything goes on day 1.
    - Greedy loading is optimal: packing as much as possible into each day
      never leaves more work for the later days.
    - Probe mid:
        daysNeeded(mid) <= days  ->  mid works, a smaller ship might too:
                                     high = mid.
        daysNeeded(mid) >  days  ->  mid is too small: low = mid + 1.

    Algorithm Steps
    ----------------
    1. Set low = max(weights), high = sum(weights).
    2. While low < high, compute mid = low + (high - low) / 2.
    3. Simulate: daysNeeded = 1, remaining = mid. For each weight, if it
       fits, remaining -= weight; else start a new day with
       remaining = mid - weight.
    4. If daysNeeded <= days, high = mid.
    5. Else low = mid + 1.
    6. Return high (== low).

    Time Complexity: O(n * log(sum - max)) - one O(n) simulation per probe.
    Space Complexity: O(1)
*/
int shipWithinDays(vector<int>& weights, int days) {
  int low = *max_element(begin(weights), end(weights));
  int high = accumulate(weights.begin(), weights.end(), 0);

  while (low < high) {
    int mid = low + (high - low) / 2;  // to prevent overflow
    int daysNeeded = 1;
    int remaining = mid;  // capacity left on the current day

    for (int weight : weights) {
      if (weight <= remaining) {
        remaining -= weight;  // fits on today's ship
      } else {
        daysNeeded++;  // start a new day with this package on board
        remaining = mid - weight;
      }
    }

    if (daysNeeded <= days) {
      high = mid;  // mid is enough, try a smaller ship
    } else {
      low = mid + 1;  // too small, needs too many days
    }
  }

  return high;
}

int main() {
  vector<pair<vector<int>, int>> tests = {
      {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5}, {{3, 2, 2, 4, 1, 4}, 3}, {{1, 2, 3, 1, 1}, 4}};

  for (auto& [weights, days] : tests) {
    cout << "Weights: ";
    for (int w : weights) cout << w << " ";
    cout << " days = " << days << endl;
    cout << "  least capacity: " << shipWithinDays(weights, days) << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN of shipWithinDays: weights = {3, 2, 2, 4, 1, 4}, days = 3
    (answer = 6)
    ======================================================================

    Initial state: low = max = 4, high = sum = 16

    ----------------------------------------------------------------------
    Iteration 1
      bounds       low = 4, high = 16
      mid          4 + (16 - 4) / 2 = 10
      loading      day 1: 3 2 2      (7)     4 does not fit in 3
                   day 2: 4 1 4      (9)
      daysNeeded   2 <= 3  ->  enough
      update       high = 10

    ----------------------------------------------------------------------
    Iteration 2
      bounds       low = 4, high = 10
      mid          4 + (10 - 4) / 2 = 7
      loading      day 1: 3 2 2      (7)
                   day 2: 4 1        (5)     4 does not fit in 2
                   day 3: 4          (4)
      daysNeeded   3 <= 3  ->  enough
      update       high = 7

    ----------------------------------------------------------------------
    Iteration 3
      bounds       low = 4, high = 7
      mid          4 + (7 - 4) / 2 = 5
      loading      day 1: 3 2        (5)
                   day 2: 2          (2)     4 does not fit in 3
                   day 3: 4 1        (5)
                   day 4: 4          (4)
      daysNeeded   4 > 3  ->  too small
      update       low = 6

    ----------------------------------------------------------------------
    Iteration 4
      bounds       low = 6, high = 7
      mid          6 + (7 - 6) / 2 = 6
      loading      day 1: 3 2        (5)     2 does not fit in 1
                   day 2: 2 4        (6)
                   day 3: 1 4        (5)
      daysNeeded   3 <= 3  ->  enough
      update       high = 6

    ----------------------------------------------------------------------
    Loop ends at low = high = 6.  RETURN 6

    ======================================================================
    Summary table
    ======================================================================

    | iter | low | high | mid | days loaded             | needed | action    |
    |------|-----|------|-----|-------------------------|--------|-----------|
    |  1   |  4  |  16  | 10  | [3 2 2] [4 1 4]         |   2    | high = 10 |
    |  2   |  4  |  10  |  7  | [3 2 2] [4 1] [4]       |   3    | high = 7  |
    |  3   |  4  |  7   |  5  | [3 2] [2] [4 1] [4]     |   4    | low  = 6  |
    |  4   |  6  |  7   |  6  | [3 2] [2 4] [1 4]       |   3    | high = 6  |

    ======================================================================
    Notes
    ======================================================================

    Why low starts at max(weights) and not 1:
      the else branch sets remaining = mid - weight, which assumes a single
      package always fits on an empty ship. With low = max(weights) that
      holds for every mid; with low = 1 remaining could go negative and the
      count would be wrong.

    Changes from the original (no bug fixes needed):
      - Renamed arr -> weights, d -> days, count -> daysNeeded,
        cap -> remaining and the loop variable i -> weight.
      - cap = mid; cap -= i; became remaining = mid - weight.
      - Removed the unused variable n.

    Overflow: sum(weights) is at most 5 * 10^4 * 500 = 2.5 * 10^7, so int
      is enough for high and for the accumulate with an int 0 seed.

    Edge cases:
      - days == 1: the whole belt goes at once -> sum(weights).
      - days == n: one package per day is allowed -> max(weights).
*/
