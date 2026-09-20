#include <bits/stdc++.h>
using namespace std;

// Q: Given an array of CPU tasks, each represented by an uppercase letter,
//    and a non-negative cooldown n, return the minimum number of time
//    intervals (including idle slots) needed to finish all the tasks. The
//    constraint is that two occurrences of the SAME task must be separated
//    by at least n intervals (idle or running some other task).
//
// Example:
// tasks = [A,A,A,B,B,B], n = 2   -> 8   (e.g. A B idle A B idle A B - the
//                                        most frequent tasks A and B tie
//                                        at 3 occurrences each, forcing 2
//                                        idle slots)
// tasks = [A,A,A,B,B,B], n = 0   -> 6   (no cooldown needed, so the tasks
//                                        can just run back to back)

/*
    Approach: Greedy closed-form counting (frequency-slot formula)

    - The most frequent task(s) are the bottleneck: arrange max_f copies
      of the task(s) tied for the highest frequency, each occurrence
      followed by a gap of size n that must be filled with other tasks
      or, failing that, idle slots. This creates (max_f - 1) full gaps of
      size (n + 1) [1 slot for the task itself + n cooldown slots], plus
      the final occurrence of the max-frequency task(s) at the end.
    - If c distinct tasks are tied for the max frequency, all c of them
      can occupy the last "row" together (one per column), so the total
      length of this skeleton is (max_f - 1) * (n + 1) + c.
    - Every other, less-frequent task can always be slotted into the idle
      gaps of this skeleton without ever needing to extend it further,
      because by definition it occurs at most max_f times and there is
      always room somewhere in the (max_f - 1) rows of width n. So this
      skeleton length is a valid schedule length whenever it is >= the
      total task count m.
    - However, if there are so many distinct low-frequency tasks that
      they don't fit into the idle gaps at all, the cooldown constraint
      becomes irrelevant - you can simply run every task back to back
      with no idle slots, giving a lower bound of m. Hence the answer is
      max(m, (max_f - 1) * (n + 1) + c).

    Algorithm Steps
    ----------------
    1. Count the frequency of each task in a map.
    2. Track max_f, the highest frequency seen.
    3. Count c, how many distinct tasks have frequency exactly max_f.
    4. Return max(m, (max_f - 1) * (n + 1) + c), where m is the total
       number of tasks.

    Time Complexity: O(m log k) - one pass over the m tasks to build the
                      frequency map (k distinct task letters, k <= 26 in
                      practice), plus one pass over the map's k entries.
    Space Complexity: O(k) - the frequency map holds at most k distinct
                      task letters (k <= 26 in practice).
*/
int leastInterval(vector<char>& arr, int n) {
  int m = arr.size();
  int res = m, max_f = 0, c = 0;
  map<char, int> mp;

  for (char i : arr) {
    mp[i]++;
    if (mp[i] > max_f) {
      max_f = mp[i];
    }
  }

  for (auto i : mp) {
    if (i.second == max_f) {
      c++;
    }
  }

  res = max(m, ((max_f - 1) * (n + 1) + c));
  return res;
}

int main() {
  struct Test {
    vector<char> tasks;
    int n;
  };
  vector<Test> tests = {{{'A', 'A', 'A', 'B', 'B', 'B'}, 2},
                         {{'A', 'A', 'A', 'B', 'B', 'B'}, 0},
                         {{'A', 'A', 'A', 'A', 'A', 'A', 'B', 'C', 'D', 'E',
                           'F', 'G'},
                          2}};

  for (auto& t : tests) {
    cout << "Input:  tasks = [";
    for (size_t i = 0; i < t.tasks.size(); ++i) {
      cout << t.tasks[i] << (i + 1 < t.tasks.size() ? "," : "");
    }
    cout << "], n = " << t.n << endl;
    cout << "Output: " << leastInterval(t.tasks, t.n) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: tasks = [A,A,A,B,B,B], n = 2   (m = 6, answer 8)
    ==========================================================================

    Tracked state:
      mp     - frequency map, char -> count
      max_f  - highest frequency seen so far
      c      - number of distinct tasks tied at max_f

    Initial state: mp = {}, max_f = 0, c = 0

    --- Pass 1: build the frequency map, tracking max_f -------------------
    i=0: char = 'A'
      mp['A']++  ->  mp = {A:1}
      mp['A'] = 1 > max_f = 0?  YES -> max_f = 1

    i=1: char = 'A'
      mp['A']++  ->  mp = {A:2}
      mp['A'] = 2 > max_f = 1?  YES -> max_f = 2

    i=2: char = 'A'
      mp['A']++  ->  mp = {A:3}
      mp['A'] = 3 > max_f = 2?  YES -> max_f = 3

    i=3: char = 'B'
      mp['B']++  ->  mp = {A:3, B:1}
      mp['B'] = 1 > max_f = 3?  NO

    i=4: char = 'B'
      mp['B']++  ->  mp = {A:3, B:2}
      mp['B'] = 2 > max_f = 3?  NO

    i=5: char = 'B'
      mp['B']++  ->  mp = {A:3, B:3}
      mp['B'] = 3 > max_f = 3?  NO (strictly greater required)

    After pass 1: mp = {A:3, B:3}, max_f = 3

    --- Pass 2: count c, how many entries equal max_f ----------------------
    entry (A, 3):  3 == max_f(3)?  YES -> c = 1
    entry (B, 3):  3 == max_f(3)?  YES -> c = 2

    After pass 2: c = 2

    --- Final computation ---------------------------------------------------
    m = 6
    (max_f - 1) * (n + 1) + c
      = (3 - 1) * (2 + 1) + 2
      = 2 * 3 + 2
      = 8

    res = max(m, 8) = max(6, 8) = 8

    RETURN res = 8

      Skeleton with the 2 tied tasks (A and B) filling every column:
        A B _   A B _   A B
        (each "row" is n+1 = 3 wide; the last row has no trailing gap)
      slots used: 3 rows of width 3, minus the 1 trailing gap that isn't
      needed after the very last occurrence = (3-1)*3 + 2 = 8, matching
      the schedule "A B idle A B idle A B" (8 intervals total).

    ==========================================================================
    Summary table
    ==========================================================================

    | step        | char | mp after op        | max_f | c |
    |-------------|------|---------------------|-------|---|
    | pass1, i=0  |  A   | {A:1}               |   1   | - |
    | pass1, i=1  |  A   | {A:2}               |   2   | - |
    | pass1, i=2  |  A   | {A:3}               |   3   | - |
    | pass1, i=3  |  B   | {A:3,B:1}           |   3   | - |
    | pass1, i=4  |  B   | {A:3,B:2}           |   3   | - |
    | pass1, i=5  |  B   | {A:3,B:3}           |   3   | - |
    | pass2, A:3  |  -   | -                   |   3   | 1 |
    | pass2, B:3  |  -   | -                   |   3   | 2 |

    final: res = max(6, (3-1)*(2+1)+2) = max(6, 8) = 8

    ==========================================================================
    Notes
    ==========================================================================

    Why (max_f - 1) * (n + 1) + c is the right skeleton length:
      the task(s) tied for the highest frequency max_f are unavoidably
      spread across max_f "rows", since no two occurrences of the same
      task can be closer together than n + 1 slots apart. That gives
      (max_f - 1) full rows of width (n + 1) plus one final row that only
      needs to be as wide as the number of tasks placed in it. Since c
      distinct tasks are tied for max_f, all c of them land in that final
      row together (one slot each), so the final row contributes exactly
      c slots instead of a full (n + 1).

    Why max(m, ...) is needed:
      the skeleton above assumes the idle gaps inside the (max_f - 1)
      rows are wide enough to absorb every remaining task. If there are
      many distinct tasks with lower frequency, they may fill (or
      overflow) those gaps entirely, in which case the cooldown
      constraint stops being the bottleneck and the tasks can simply be
      packed back-to-back with zero idle time - a schedule of length m.
      Taking the max of the two guarantees correctness in both regimes.

    Complexity note:
      the frequency map holds at most 26 entries (uppercase letters), so
      both passes and the final arithmetic are effectively O(m) overall,
      with negligible O(1)-ish overhead from the small map.
*/
