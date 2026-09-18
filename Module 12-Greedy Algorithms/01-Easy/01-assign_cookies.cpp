#include <bits/stdc++.h>
using namespace std;

// Q: There are n children and m cookies. Each child i has a greed factor
//    g[i] - the minimum cookie size that will satisfy them. Each cookie j
//    has a size s[j]. Each child can receive at most one cookie, and a
//    cookie of size s[j] satisfies child i only if s[j] >= g[i]. Return
//    the maximum number of content children.
//
// Example:
// g = [1,2,3], s = [1,1]       -> 1   (only the size-1 cookies exist, and
//                                      only the greed-1 child can be
//                                      satisfied by either of them)
// g = [1,2],   s = [1,2,3]     -> 2   (every child can be satisfied)
// g = [10,9,8,7], s = [5,6,7]  -> 1   (only the greed-7 child can be
//                                      satisfied, by the size-7 cookie)

/*
    Approach: Greedy two pointers over both arrays, sorted ascending

    - Satisfying a LESS greedy child with any given cookie never costs
      anything a more greedy child could have used instead - if a cookie
      can satisfy the least greedy remaining child, giving it to them and
      moving on is always at least as good as saving it, because any
      other still-unsatisfied child needs a cookie at least as large. So
      sort both by size and always try to satisfy the least greedy child
      first with the smallest cookie that might work.
    - Walk both sorted arrays with pointers i (into g) and j (into s).
      If the current cookie s[j] is big enough for the current child
      g[i] (s[j] >= g[i]), that child is satisfied: count it and advance
      BOTH pointers - this cookie is used up and this child is done.
      If the cookie is too small, it cannot satisfy ANY remaining child
      either, since g is sorted ascending and g[i] is already the
      smallest remaining greed factor - so just advance j and try the
      next, larger cookie.
    - i is never advanced without incrementing res, and res is only ever
      incremented alongside advancing i - so res always ends up equal to
      however far i reached, and the loop naturally stops the moment
      either every child is satisfied (i == n) or every cookie has been
      tried (j == m).

    Algorithm Steps
    ----------------
    1. Sort g and s ascending.
    2. Set i = 0, j = 0, res = 0.
    3. While i < n and j < m:
         a. If g[i] <= s[j]: this cookie satisfies this child ->
              i++, j++, res++.
         b. Else: this cookie is too small for anyone remaining -> j++.
    4. Return res.

    Time Complexity: O(n log n + m log m) - dominated by sorting both
                      arrays; the two-pointer walk itself is O(n + m).
    Space Complexity: O(1) extra - sorting is done in place (aside from
                      whatever the sort implementation uses internally).
*/
int findContentChildren(vector<int>& g, vector<int>& s) {
  sort(begin(g), end(g));   // ascending greed factors
  sort(begin(s), end(s));   // ascending cookie sizes

  int n = g.size(), m = s.size();
  int i = 0, j = 0, res = 0;

  while (i < n && j < m) {
    if (g[i] <= s[j]) {
      i++;      // this child is satisfied, move to the next child
      j++;      // this cookie is used up, move to the next cookie
      res++;
    } else {
      j++;      // too small for anyone remaining - try a bigger cookie
    }
  }

  return res;
}

int main() {
  struct Test {
    vector<int> g, s;
  };
  vector<Test> tests = {
      {{1, 2, 3}, {1, 1}}, {{1, 2}, {1, 2, 3}}, {{10, 9, 8, 7}, {5, 6, 7}}};

  for (auto& t : tests) {
    cout << "Input:  g = [";
    for (size_t i = 0; i < t.g.size(); ++i) {
      cout << t.g[i] << (i + 1 < t.g.size() ? "," : "");
    }
    cout << "], s = [";
    for (size_t i = 0; i < t.s.size(); ++i) {
      cout << t.s[i] << (i + 1 < t.s.size() ? "," : "");
    }
    cout << "]" << endl;
    cout << "Output: " << findContentChildren(t.g, t.s) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: g = [10,9,8,7], s = [5,6,7]   (n = 4, m = 3, answer 1)
    ==========================================================================

    STEP 1: sort both ascending
      g: [10,9,8,7]  ->  [7,8,9,10]
      s: [5,6,7]     ->  [5,6,7]     (already sorted)

      index:   0   1   2   3
      g:       7   8   9  10

      index:   0   1   2
      s:       5   6   7

    Tracked state:
      i, j  - pointers into g and s
      res   - children satisfied so far

    Initial state: i = 0, j = 0, res = 0

    --------------------------------------------------------------------------
    i = 0, j = 0:  g[0] = 7,  s[0] = 5
      compare      g[i] <= s[j]?  7 <= 5?  NO - this cookie is too small
                   even for the LEAST greedy remaining child
      action       j++ (try a bigger cookie)
      state        i = 0, j = 1, res = 0

    --------------------------------------------------------------------------
    i = 0, j = 1:  g[0] = 7,  s[1] = 6
      compare      7 <= 6?  NO - still too small
      action       j++
      state        i = 0, j = 2, res = 0

    --------------------------------------------------------------------------
    i = 0, j = 2:  g[0] = 7,  s[2] = 7
      compare      7 <= 7?  YES - this cookie satisfies this child
      action       i++, j++, res++
      state        i = 1, j = 3, res = 1

    --------------------------------------------------------------------------
    i = 1, j = 3:  j == m = 3 -> loop condition fails, STOP

    --------------------------------------------------------------------------
    RETURN res = 1

      the size-7 cookie satisfies the greed-7 child; every other child
      (greed 8, 9, 10) has no cookie left big enough, and every other
      cookie (size 5, 6) was too small for anyone.

    ==========================================================================
    Summary table
    ==========================================================================

    | i | j | g[i] | s[j] | g[i]<=s[j]? | action        | res after |
    |---|---|------|------|-------------|---------------|-----------|
    | 0 | 0 |  7   |  5   |     no      | j++           |     0     |
    | 0 | 1 |  7   |  6   |     no      | j++           |     0     |
    | 0 | 2 |  7   |  7   |     yes     | i++, j++, res++ |   1     |
    | 1 | 3 |  -   |  -   |  j == m     | loop ends     |     1     |

    ==========================================================================
    Second dry run: g = [1,2], s = [1,2,3]   (answer 2, every child satisfied)
    ==========================================================================

      g sorted: [1,2]      s sorted: [1,2,3]

      i=0,j=0: g[0]=1 <= s[0]=1  -> YES -> i=1,j=1,res=1
      i=1,j=1: g[1]=2 <= s[1]=2  -> YES -> i=2,j=2,res=2
      i=2: i == n = 2 -> loop ends

      RETURN res = 2   (the size-3 cookie is never even needed)

    ==========================================================================
    Notes
    ==========================================================================

    Why advancing only j (and not i) on a mismatch is correct:
      g is sorted ascending, so g[i] is the SMALLEST greed factor among
      children not yet satisfied. If s[j] cannot satisfy even that child,
      it cannot satisfy any child with a larger greed factor either -
      there is no point checking s[j] against g[i+1], g[i+2], and so on.
      The only useful move is to try a bigger cookie.

    Why this greedy is optimal:
      suppose an optimal assignment gives some cookie s[j] to a child with
      a LARGER greed factor while a less greedy child who could also have
      used s[j] goes unsatisfied because their assigned cookie (if any)
      was bigger than necessary. Swapping the two children's cookies keeps
      the more greedy child's requirement met by an equally sized or
      larger cookie, if it was met before, and frees no fewer
      opportunities overall - so an assignment that always prefers
      satisfying the least greedy remaining child first is never worse.

    Complexity note:
      most of the cost is the two sorts. If g and s were already known to
      be sorted, this would run in O(n + m) with no extra space at all.
*/
