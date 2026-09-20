#include <bits/stdc++.h>
using namespace std;

// Q: You are given an array of non-overlapping intervals `arr` sorted in
//    ascending order by start time, and a new interval `interval`. Insert
//    the new interval into `arr` such that the result is still sorted in
//    ascending order by start time, and any overlapping intervals are
//    merged into one. Return the resulting list of intervals.
//
// Example:
// arr = [[1,3],[6,9]], interval = [2,5]
//   -> [[1,5],[6,9]]
//   (the new interval [2,5] overlaps [1,3], merging into [1,5]; [6,9] is
//    untouched since it starts after the merged interval ends)
//
// arr = [[1,2],[3,5],[6,7],[8,10],[12,16]], interval = [4,8]
//   -> [[1,2],[3,10],[12,16]]
//   ([1,2] is untouched; [3,5], [6,7], and [8,10] all overlap the new
//    interval at some point and get merged into a single [3,10]; [12,16]
//    is untouched since it starts after the merged interval ends)

/*
    Approach: Single left-to-right scan, partitioning intervals into three
    buckets relative to the new interval.

    - Because `arr` is already sorted and non-overlapping, every interval
      falls into exactly one of three cases relative to the (possibly still
      growing) new interval:
        1. Ends before the new interval starts  -> keep as-is, it can never
           overlap anything that comes later.
        2. Starts after the new interval ends    -> the new interval is
           finalized (nothing later can overlap it either, since arr is
           sorted), so insert it now, then copy over all remaining
           intervals unchanged.
        3. Otherwise, it overlaps the new interval -> absorb it into the
           new interval by expanding the new interval's bounds to the min
           of the starts and the max of the ends.
    - The new interval itself is mutated in place as we absorb overlapping
      intervals, so by the time we hit an interval that starts after it,
      or fall off the end of the loop, it already reflects the full merge
      of everything it overlapped.
    - If we exit the loop normally (every interval either came before or
      overlapped), the new interval belongs at the very end, so it is
      appended after the loop.

    Algorithm Steps
    ----------------
    1. Walk through `arr` from left to right.
    2. If the current interval ends before the new interval starts, it
       cannot overlap; copy it to the result unchanged.
    3. Else if the current interval starts after the new interval ends, no
       further interval can overlap the new interval either (sorted
       order), so push the new interval, then push all remaining
       intervals unchanged, and return immediately.
    4. Otherwise the current interval overlaps the new interval; expand
       the new interval's start/end to cover both.
    5. If the loop finishes without hitting case 3, the (possibly merged)
       new interval is the last one and is appended after the loop.

    Time Complexity: O(n) - each interval is visited once.
    Space Complexity: O(n) - for the output list (ignoring the input/output
                       space, the extra space used is O(1)).
*/
vector<vector<int>> insert(vector<vector<int>>& arr, vector<int>& interval) {
  vector<vector<int>> res;

  for (int i = 0; i < arr.size(); i++) {

    // Current interval is completely before the new interval
    if (arr[i][1] < interval[0]) {
      res.push_back(arr[i]);
    }

    // Current interval is completely after the new interval
    else if (arr[i][0] > interval[1]) {
      res.push_back(interval);

      // Add all remaining intervals
      while (i < arr.size()) {
        res.push_back(arr[i]);
        i++;
      }

      return res;
    }

    // Current interval overlaps with the new interval
    else {
      interval[0] = min(interval[0], arr[i][0]);
      interval[1] = max(interval[1], arr[i][1]);
    }
  }

  // New interval belongs at the end
  res.push_back(interval);

  return res;
}

string toString(vector<vector<int>>& v) {
  string out = "[";
  for (int i = 0; i < (int)v.size(); i++) {
    out += "[" + to_string(v[i][0]) + "," + to_string(v[i][1]) + "]";
    if (i + 1 < (int)v.size()) out += ",";
  }
  out += "]";
  return out;
}

string toString(vector<int>& v) {
  return "[" + to_string(v[0]) + "," + to_string(v[1]) + "]";
}

int main() {
  struct Test {
    vector<vector<int>> arr;
    vector<int> interval;
  };
  vector<Test> tests = {
    { {{1,3},{6,9}}, {2,5} },
    { {{1,2},{3,5},{6,7},{8,10},{12,16}}, {4,8} },
    { {}, {5,7} },
    { {{1,5}}, {2,3} },
    { {{1,5}}, {6,8} },
    { {{3,5},{12,15}}, {6,6} },
  };

  for (auto& t : tests) {
    vector<vector<int>> arr = t.arr;
    vector<int> interval = t.interval;
    cout << "Input:  arr = " << toString(arr) << ", interval = " << toString(interval) << endl;
    vector<vector<int>> result = insert(arr, interval);
    cout << "Output: " << toString(result) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = [[1,3],[6,9]], interval = [2,5]
    ==========================================================================
    Initial: res = [], interval = [2,5]

    i = 0 -> arr[0] = [1,3]
      arr[0][1] = 3 < interval[0] = 2?  No (3 is not < 2)
      arr[0][0] = 1 > interval[1] = 5?  No (1 is not > 5)
      -> overlap case:
         interval[0] = min(2, 1) = 1
         interval[1] = max(5, 3) = 5
         interval is now [1,5]
      res is still []

    i = 1 -> arr[1] = [6,9]
      arr[1][1] = 9 < interval[0] = 1?  No (9 is not < 1)
      arr[1][0] = 6 > interval[1] = 5?  Yes (6 > 5)
      -> "completely after" case:
         res.push_back(interval) -> res = [[1,5]]
         enter while (i < arr.size()):
           i = 1 < 2 -> true: push arr[1] = [6,9] -> res = [[1,5],[6,9]]; i becomes 2
           i = 2 < 2 -> false: exit while
         return res immediately -> res = [[1,5],[6,9]]

    Final result: [[1,5],[6,9]]

    ==========================================================================
    Summary table
    ==========================================================================
    | i | arr[i] | comparison                          | action                    | interval | res                  |
    |---|--------|--------------------------------------|---------------------------|----------|----------------------|
    | 0 | [1,3]  | 3<2? no, 1>5? no -> overlap          | merge into interval       | [1,5]    | []                   |
    | 1 | [6,9]  | 9<1? no, 6>5? yes -> completely after| push interval, then [6,9] | [1,5]    | [[1,5],[6,9]] (return)|

    ==========================================================================
    Notes
    ==========================================================================
    - The three-way split (before / after / overlapping) is exhaustive and
      mutually exclusive for any interval compared against the current new
      interval, because the input is sorted and non-overlapping: an
      interval either ends strictly before the new interval starts, starts
      strictly after the new interval ends, or touches/overlaps it.
    - The "completely after" branch is safe to return from immediately
      (after copying the rest of `arr` unchanged) precisely because `arr`
      is sorted by start time: once one interval starts after the
      (finalized) new interval ends, every later interval starts even
      later, so none of them can overlap the new interval, and none of
      them can overlap each other differently than they already don't
      (they were non-overlapping to begin with).
    - The overlap branch grows `interval` monotonically (min of starts, max
      of ends), so by the time a later interval is compared against it, the
      comparison correctly reflects everything absorbed so far.
    - If the new interval never triggers the "completely after" branch (it
      overlaps everything remaining, or nothing remains to compare against
      after it), it is appended once at the end, after the loop - this
      covers the case where the new interval belongs at the very end of
      the result, or where `arr` was empty to begin with.
    - Time Complexity: O(n), where n = arr.size() - the outer for loop and
      the inner while loop together visit each interval of `arr` exactly
      once across the whole execution (the while loop only runs after the
      for loop returns, consuming the remaining unvisited indices).
    - Space Complexity: O(n) for the output list `res`; O(1) extra
      auxiliary space beyond the input/output.
*/
