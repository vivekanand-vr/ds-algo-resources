#include <bits/stdc++.h>
using namespace std;

// Q: Given a list of intervals, return the minimum number of intervals
//    that must be removed so that the remaining intervals are pairwise
//    non-overlapping. Two intervals [a,b] and [c,d] overlap when they
//    share more than a single touching point (i.e. intervals that only
//    touch at an endpoint, like [1,2] and [2,3], do NOT count as
//    overlapping).
//
// Example:
// intervals = [[1,2],[2,3],[3,4],[1,3]]  -> 1  (remove [1,3]; the rest,
//                                                [1,2],[2,3],[3,4], only
//                                                touch at endpoints)
// intervals = [[1,2],[1,2],[1,2]]        -> 2  (only one [1,2] can be
//                                                kept, the other two
//                                                duplicates overlap it
//                                                and must be removed)

/*
    Approach: Greedy, sort by END time and keep the earliest finisher
    (activity-selection style)

    - Whenever two intervals overlap, one of them has to go. Among any
      group of mutually overlapping intervals, keeping the one that ends
      EARLIEST is always at least as good as keeping any other, because
      an earlier end time leaves the most room for everything that comes
      after it - any interval that could coexist with a later-ending
      choice could also have coexisted with the earlier-ending one, but
      not necessarily the other way around. So sorting by end time and
      always greedily keeping the earliest-ending non-conflicting
      interval never removes more than necessary.
    - After sorting by end ascending, walk the intervals left to right
      while tracking `last`, the end time of the most recently KEPT
      interval (starts as the end of the first interval, which is always
      kept). For each subsequent interval, if its start is >= last it
      does not overlap the last kept interval, so it is kept and `last`
      is updated to its end. Otherwise it overlaps - since it ends no
      earlier than the interval already kept (sort order guarantees
      arr[i][1] >= last), discarding THIS one and keeping the
      earlier-ending one already tracked in `last` is the better (or
      equal) choice, so it is counted for removal and `last` is left
      unchanged.
    - `count` only ever increases when an interval is rejected, and every
      kept interval strictly extends the non-overlapping chain built so
      far, so by the end `count` is exactly the minimum number of
      intervals removed to make everything else non-overlapping.

    Algorithm Steps
    ----------------
    1. Sort intervals ascending by end time (arr[i][1]).
    2. Set last = arr[0][1] (the first interval is always kept), count = 0.
    3. For i = 1 .. n-1:
         a. If arr[i][0] >= last: no overlap with the last kept interval
              -> keep it, set last = arr[i][1].
         b. Else: overlaps -> discard it, count++ (last stays the same).
    4. Return count.

    Time Complexity: O(n log n) - dominated by the sort; the single pass
                      afterward is O(n).
    Space Complexity: O(1) extra (aside from whatever space the sort
                      implementation itself uses internally).
*/
int eraseOverlapIntervals(vector<vector<int>>& arr) {
  sort(arr.begin(), arr.end(),
       [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
  int n = arr.size();
  int last = arr[0][1];
  int count = 0;
  for (int i = 1; i < n; ++i) {
    if (arr[i][0] >= last) {
      last = arr[i][1];
    } else {
      count++;
    }
  }

  return count;
}

int main() {
  struct Test {
    vector<vector<int>> intervals;
  };
  vector<Test> tests = {
      {{{1, 2}, {2, 3}, {3, 4}, {1, 3}}},
      {{{1, 2}, {1, 2}, {1, 2}}},
      {{{1, 2}, {2, 3}}},
      {{{1, 100}, {11, 22}, {1, 11}, {2, 12}}},
      {{{1, 5}}}};

  for (auto& t : tests) {
    cout << "Input:  [";
    for (size_t i = 0; i < t.intervals.size(); ++i) {
      cout << "[" << t.intervals[i][0] << "," << t.intervals[i][1] << "]"
           << (i + 1 < t.intervals.size() ? "," : "");
    }
    cout << "]" << endl;
    cout << "Output: " << eraseOverlapIntervals(t.intervals) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: intervals = [[1,2],[2,3],[3,4],[1,3]]   (n = 4, answer 1)
    ==========================================================================

    STEP 1: sort ascending by end time (arr[i][1])
      original: [1,2](end 2), [2,3](end 3), [3,4](end 4), [1,3](end 3)

      end times: 2, 3, 3, 4 - two intervals tie on end = 3, namely
      [2,3] and [1,3]. Either relative order of a tie is valid for the
      comparator (it only looks at index 1); we take the order in which
      they already appeared, giving:

        index:   0     1     2     3
        interval:[1,2] [2,3] [1,3] [3,4]
        end:      2     3     3     4

    Tracked state:
      last   - end time of the most recently kept interval
      count  - number of intervals discarded so far

    Initial state: last = arr[0][1] = 2, count = 0   ([1,2] is kept)

    --------------------------------------------------------------------------
    i = 1: arr[1] = [2,3]
      compare      arr[1][0] >= last?   2 >= 2?  YES - no overlap
      action       keep it, last = arr[1][1] = 3
      state        last = 3, count = 0

    --------------------------------------------------------------------------
    i = 2: arr[2] = [1,3]
      compare      arr[2][0] >= last?   1 >= 3?  NO - overlaps the kept
                   interval [2,3] (which currently ends at 3)
      action       discard it, count++
      state        last = 3, count = 1

    --------------------------------------------------------------------------
    i = 3: arr[3] = [3,4]
      compare      arr[3][0] >= last?   3 >= 3?  YES - no overlap
      action       keep it, last = arr[3][1] = 4
      state        last = 4, count = 1

    --------------------------------------------------------------------------
    RETURN count = 1

      the kept chain is [1,2] -> [2,3] -> [3,4] (each only touches the
      next at a single point, so none of them overlap); [1,3] is the one
      interval removed.

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i] | last (before) | arr[i][0]>=last? | action        | count after |
    |---|--------|----------------|-------------------|---------------|-------------|
    | - | [1,2]  |       -        |         -         | kept (init)   |      0      |
    | 1 | [2,3]  |       2        |       yes         | keep, last=3  |      0      |
    | 2 | [1,3]  |       3        |       no          | discard       |      1      |
    | 3 | [3,4]  |       3        |       yes         | keep, last=4  |      1      |

    ==========================================================================
    Second dry run: intervals = [[1,2],[1,2],[1,2]]   (answer 2)
    ==========================================================================

      all three intervals already have end = 2, so the sort leaves them
      in place: [1,2], [1,2], [1,2]

      last = 2, count = 0   (first [1,2] kept)
      i=1: [1,2][0]=1 >= last=2?  NO  -> discard, count = 1
      i=2: [1,2][0]=1 >= last=2?  NO  -> discard, count = 2

      RETURN count = 2   (only one of the three identical intervals can
      ever be kept, the other two must both be removed)

    ==========================================================================
    Notes
    ==========================================================================

    Why sorting by END time (not start time) is the key idea:
      this is the same family of greedy as classic activity selection.
      Once intervals are sorted by end time, the earliest-ending interval
      is always a safe choice to keep, because no other interval that
      could ever coexist with a later-ending choice would fail to also
      coexist with this earlier-ending one - it never blocks more future
      intervals than any alternative choice would.

    Exchange argument for optimality:
      suppose an optimal solution keeps some interval X instead of the
      earliest-ending candidate E from the same overlapping group, where
      E ends no later than X. Swapping E in for X cannot cause any new
      overlap with whatever comes after, since E ends at or before X's
      end time - so the swap keeps the solution feasible and no worse.
      Repeating this argument across the whole sequence shows the greedy
      choice (always keep the earliest-ending non-conflicting interval)
      never removes more intervals than necessary.

    Why the comparison is arr[i][0] >= last, not >:
      the problem only counts intervals as overlapping when they share
      more than a touching point, so [2,3] following a kept [1,2] (start
      2 == last 2) is NOT an overlap and both are kept.

    Complexity note:
      the sort is O(n log n) and dominates; the single left-to-right
      pass that follows is O(n), so the overall complexity is
      O(n log n) time and O(1) extra space.
*/
