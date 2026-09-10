#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Merge all overlapping intervals in a collection of intervals

/*
    Approach: Sort by Start Time + Linear Merge
    - Sort intervals by their start time so overlapping intervals become adjacent
    - Walk through intervals, comparing each one to the last interval placed
      in the result
    - If the current interval's start is <= the last result interval's end,
      they overlap, so extend the end of the last interval; otherwise push
      the current interval as a new, separate one
    - Renamed from "merge" to "mergeIntervals" to avoid any ambiguity with
      std::merge, since this file uses `using namespace std;`

    Algorithm Steps
    ----------------
    1. Sort intervals by start time
    2. For each interval, check if it overlaps with the last one in the result
    3. If it overlaps, update the end of the last result interval to the max end
    4. If it doesn't overlap, push it as a new interval
    5. Return the merged list

    Time Complexity: O(n log n) - dominated by sorting the intervals
    Space Complexity: O(n) - storing the merged result
*/
vector<vector<int>> mergeIntervals(vector<vector<int>>& arr) {
    // First step: sort intervals by starting time
    sort(arr.begin(), arr.end());

    vector<vector<int>> list;  // result list to store merged intervals

    for(int i = 0; i < arr.size(); ++i) {
        // If list is not empty AND current interval overlaps with previous one
        // overlap happens when current_start <= previous_end
        if(!list.empty() && list.back()[1] >= arr[i][0]) {
            // merge by updating the end to the max end
            list.back()[1] = max(list.back()[1], arr[i][1]);
        } else {
            // otherwise, no overlap → push as new interval
            list.push_back(arr[i]);
        }
    }

    return list;
}

int main() {
    vector<vector<int>> intervals = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};

    cout << "Intervals:" << endl;
    for (auto& iv : intervals) cout << iv[0] << " " << iv[1] << endl;

    vector<vector<int>> result = mergeIntervals(intervals);

    cout << "Merged intervals:" << endl;
    for (auto& iv : result) cout << iv[0] << " " << iv[1] << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {{1,3}, {2,6}, {8,10}, {15,18}}
             (n = 4, answer = {{1,6}, {8,10}, {15,18}})
    ==========================================================================

      i:          0       1       2        3
      interval: [1,3]   [2,6]  [8,10]  [15,18]

    Tracked state:
      arr    - the input intervals, sorted by start time (step 0 below)
      list   - the merged result, built left to right
      back   - list.back(), the most recently placed interval; only THIS
               one can still be extended, because arr is sorted by start
      i      - index of the interval currently being considered

    Step 0 - sort(arr.begin(), arr.end()):
      vector<int> compares lexicographically, so this sorts by start
      first with end as the tie-break. This input already happens to be
      in that order, so the sort changes nothing here:

        {1,3}, {2,6}, {8,10}, {15,18}       (unchanged)

    Initial state: list = {} (empty)

    --------------------------------------------------------------------------
    i = 0, arr[i] = [1,3]
      test      list.empty() is true -> the && SHORT-CIRCUITS, so
                list.back() is never evaluated. That matters: back() on
                an empty vector is undefined behaviour, and this guard
                is the only thing preventing it.
      emit      push_back([1,3])   (nothing to merge with yet)
      list      {[1,3]}

    --------------------------------------------------------------------------
    i = 1, arr[i] = [2,6]
      compare   list.back() = [1,3], so back[1] = 3
                overlap test: 3 >= arr[1][0] = 2  ->  TRUE, they overlap
      merge     back[1] = max(back[1], arr[1][1]) = max(3, 6) = 6
                note only the END moves. The start stays 1 because the
                sort guarantees arr[1][0] >= back[0], so 1 is already the
                smaller start.

                  1   2   3   4   5   6
                  [-------]                 back  = [1,3]
                      [---------------]     arr[1]= [2,6]
                  [-------------------]     merged= [1,6]
                  ^                   ^
                 start kept        end extended to max(3,6)

      list      {[1,6]}

    --------------------------------------------------------------------------
    i = 2, arr[i] = [8,10]
      compare   back = [1,6], back[1] = 6
                overlap test: 6 >= 8  ->  FALSE, there is a gap (6 -> 8)
      emit      push_back([8,10]) as a brand new interval
                [1,6] is now FINAL - it can never be touched again,
                because every later start is >= 8 > 6.
      list      {[1,6], [8,10]}

    --------------------------------------------------------------------------
    i = 3, arr[i] = [15,18]
      compare   back = [8,10], back[1] = 10
                overlap test: 10 >= 15  ->  FALSE, gap again
      emit      push_back([15,18])
      list      {[1,6], [8,10], [15,18]}
      advance   i -> 4  ->  i == arr.size(), loop ends

    --------------------------------------------------------------------------
    RETURN list = {[1,6], [8,10], [15,18]}      (3 intervals from 4)

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i]  | back    | overlap test   | action | list after           |
    |---|---------|---------|----------------|--------|----------------------|
    | 0 | [1,3]   | (empty) | not evaluated  | push   | [1,3]                |
    | 1 | [2,6]   | [1,3]   | 3 >= 2  true   | merge  | [1,6]                |
    | 2 | [8,10]  | [1,6]   | 6 >= 8  false  | push   | [1,6],[8,10]         |
    | 3 | [15,18] | [8,10]  | 10 >= 15 false | push   | [1,6],[8,10],[15,18] |

    Why comparing against ONLY list.back() is enough:
      after sorting, starts are non-decreasing. So if arr[i] does not
      overlap the last placed interval, it cannot overlap any earlier one
      either - every earlier interval ends no later than back does. That
      single comparison per interval makes the merge pass O(n); the
      O(n log n) in the complexity claim is entirely the sort.

    The one subtlety - `>=` and not `>`:
      touching intervals like [1,3] and [3,5] have 3 >= 3, so they MERGE
      into [1,5]. If the problem treated touching intervals as separate,
      this single character would have to become `>`.

    Watch the max():
      merging [1,10] with [2,6] gives max(10, 6) = 10, NOT 6. Assigning
      arr[i][1] directly instead of the max would wrongly SHRINK an
      interval that fully contains the next one.
*/
