#include <bits/stdc++.h>
using namespace std;

// Q: There are n activities, each with a start time s[i] and a finish time
//    f[i]. A single person can perform only one activity at a time, and two
//    activities i and j are considered to conflict if their intervals
//    overlap OR merely touch (i.e. one starts exactly when the other
//    finishes is still treated as a conflict here - this implementation
//    requires the next activity's start to be strictly greater than the
//    previous activity's finish). Return the maximum number of
//    non-conflicting activities the person can perform.
//
// Example:
// s = [1,2,3], f = [2,3,4]                     -> 2   (touching activities
//                                                       (1,2) and (2,3) are
//                                                       NOT both selectable
//                                                       here; e.g. (1,2) and
//                                                       (3,4) are)
// s = [1,2,3], f = [10,10,10]                   -> 1   (all three overlap
//                                                       heavily, only one
//                                                       can be done)
// s = [5,1,8,0,3], f = [7,2,9,6,4]              -> 4   ((1,2),(3,4),(5,7),
//                                                       (8,9) chosen; (0,6)
//                                                       is rejected)

/*
    Approach: Greedy - sort by finish time, then pick earliest-finishing
    compatible activity

    - Pairing each activity's start and finish time and sorting by finish
      time (ascending) means the activity that frees up the person soonest
      is always considered first.
    - Greedily keep track of the finish time of the last accepted activity
      (lastFinish). An activity is accepted only if its start time is
      strictly greater than lastFinish, i.e. it does not overlap (and does
      not merely touch) the previously accepted activity.
    - Whenever an activity is accepted, lastFinish is updated to its finish
      time and the count of accepted activities is incremented.
    - Choosing the activity that finishes earliest among the compatible
      ones is always at least as good as any other choice, because it
      leaves the most room for future activities (standard exchange-
      argument greedy for interval scheduling).

    Algorithm Steps
    ----------------
    1. Build a vector of (start, finish) pairs from s and f.
    2. Sort the pairs by finish time in ascending order.
    3. Always accept the first activity (after sorting) and initialize
       lastFinish to its finish time; count = 1.
    4. For every subsequent activity, if its start time is strictly greater
       than lastFinish, accept it: update lastFinish to its finish time and
       increment count.
    5. Return count after scanning all activities.

    Time Complexity: O(n log n) - dominated by sorting the n (start,
                      finish) pairs; the single linear scan afterwards is
                      O(n).
    Space Complexity: O(n) - for the vector of (start, finish) pairs.
*/
int activitySelection(vector<int>& s, vector<int>& f) {
  int n = s.size();
  vector<pair<int, int>> v;
  for (int i = 0; i < n; ++i) {
    v.push_back({s[i], f[i]});
  }

  sort(v.begin(), v.end(),
       [&](pair<int, int> a, pair<int, int> b) { return a.second < b.second; });

  int lastInd = 0;
  int count = 1;
  int lastFinish = v[0].second;

  for (int i = 1; i < n; ++i) {
    if (lastFinish < v[i].first) {
      lastInd = i;
      lastFinish = v[i].second;
      count++;
    }
  }

  return count;
}

int main() {
  struct Test {
    vector<int> s, f;
  };
  vector<Test> tests = {
      {{1, 3, 0, 5, 8, 5}, {2, 4, 6, 7, 9, 9}},
      {{5, 1, 8, 0, 3}, {7, 2, 9, 6, 4}},
      {{1, 2, 3}, {2, 3, 4}},
      {{1, 2, 3}, {10, 10, 10}},
  };

  for (auto& t : tests) {
    cout << "Input:  s = [";
    for (size_t i = 0; i < t.s.size(); ++i) cout << t.s[i] << (i + 1 < t.s.size() ? "," : "");
    cout << "], f = [";
    for (size_t i = 0; i < t.f.size(); ++i) cout << t.f[i] << (i + 1 < t.f.size() ? "," : "");
    cout << "]" << endl;
    cout << "Output: " << activitySelection(t.s, t.f) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = [5,1,8,0,3], f = [7,2,9,6,4]
    ==========================================================================
    n = 5

    Step 1 - build pairs (start, finish) in input order:
    v = [(5,7), (1,2), (8,9), (0,6), (3,4)]

    Step 2 - sort v by finish time ascending:
    v = [(1,2), (3,4), (0,6), (5,7), (8,9)]
          f=2     f=4    f=6    f=7    f=9

    Step 3 - initialize:
    lastInd = 0, count = 1, lastFinish = v[0].second = 2

    Step 4 - scan i = 1..4:

    i=1: v[1] = (3,4)
         lastFinish(2) < v[1].first(3)? 2 < 3 -> true
         ACCEPT: lastInd = 1, lastFinish = 4, count = 2

    i=2: v[2] = (0,6)
         lastFinish(4) < v[2].first(0)? 4 < 0 -> false
         REJECT (activity (0,6) overlaps the accepted (3,4))

    i=3: v[3] = (5,7)
         lastFinish(4) < v[3].first(5)? 4 < 5 -> true
         ACCEPT: lastInd = 3, lastFinish = 7, count = 3

    i=4: v[4] = (8,9)
         lastFinish(7) < v[4].first(8)? 7 < 8 -> true
         ACCEPT: lastInd = 4, lastFinish = 9, count = 4

    Loop ends (i = 5 == n). Return count = 4.

    Activities effectively selected: (1,2), (3,4), (5,7), (8,9)

    ==========================================================================
    Summary table
    ==========================================================================
    | i | v[i]  | lastFinish (before) | condition (lastFinish < start) | action | lastFinish (after) | count (after) |
    |---|-------|----------------------|---------------------------------|--------|---------------------|---------------|
    | - | (1,2) | -                    | -                               | init   | 2                   | 1             |
    | 1 | (3,4) | 2                    | 2 < 3 -> true                   | accept | 4                   | 2             |
    | 2 | (0,6) | 4                    | 4 < 0 -> false                  | reject | 4                   | 2             |
    | 3 | (5,7) | 4                    | 4 < 5 -> true                   | accept | 7                   | 3             |
    | 4 | (8,9) | 7                    | 7 < 8 -> true                   | accept | 9                   | 4             |

    Final answer: 4

    ==========================================================================
    Notes
    ==========================================================================
    - Why sort by finish time: among all activities that are still
      compatible with what has been picked so far, the one that finishes
      earliest leaves the largest possible window of time free for
      subsequent activities. Picking any other compatible activity first
      can never lead to a strictly better solution - if an optimal solution
      picks some activity A as its first activity, swapping A for the
      earliest-finishing activity E (E.finish <= A.finish) still leaves
      every activity that was compatible with A compatible with E, so the
      exchange never reduces the total count. Applying this argument
      inductively at every step proves the greedy choice is optimal.
    - Strict inequality nuance: this implementation uses lastFinish <
      v[i].first (strict), so an activity whose start time exactly equals
      the previous activity's finish time is treated as a conflict rather
      than being allowed to run back-to-back. This is a deliberate
      characteristic of this code (kept as-is), as opposed to the more
      common variant that uses <= and allows immediate back-to-back
      scheduling.
    - `lastInd` tracks the index of the most recently accepted activity but
      is never read after being assigned - it is dead state in this
      function (harmless, left untouched here since it doesn't affect
      correctness).
    - Time Complexity: O(n log n), dominated by the sort; the scan that
      follows is O(n).
    - Space Complexity: O(n) for the vector of (start, finish) pairs built
      from the input.
*/
