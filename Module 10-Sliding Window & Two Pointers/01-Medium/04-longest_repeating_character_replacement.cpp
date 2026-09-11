#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s of uppercase letters and an integer k, you may change
//    at most k characters to any other uppercase letter. Return the length
//    of the longest substring containing a single repeated letter that you
//    can produce.
//
// Example:
// s = "ABAB",    k = 2 -> 4   (change both A's to B, or both B's to A)
// s = "AABABBA", k = 1 -> 4   ("AABA" -> "AAAA", changing the one B)
// s = "AAAA",    k = 0 -> 4   (already uniform, no changes needed)

/*
    Approach: Variable-size sliding window, keep the majority letter and
              replace the rest

    - Inside any window the cheapest plan is obvious: keep whichever letter
      occurs most often and rewrite every other character. So a window of
      width w is achievable exactly when
              w - (count of its most frequent letter)  <=  k
      i.e. the number of characters that are NOT the majority letter fits
      in the change budget.
    - Keep a window [l, r] with mp = letter -> count inside it, and mx =
      the largest count seen. Each time r grows, update mx with the count
      of the letter just added, then shrink from the left while the window
      violates (r - l + 1) - mx > k.
    - The subtle part: mx is NEVER decreased when the window shrinks, so
      it can be STALE - larger than the true maximum count of the current
      window. That does not break the answer, and it is the standard form
      of this solution. Reason: res only ever grows, so a wrong answer
      would require reporting a width LARGER than any valid one. A stale
      (too large) mx makes the shrink condition too permissive, letting a
      window survive that should have shrunk - but such a window is never
      WIDER than the window that set mx in the first place, so it cannot
      push res above an already-recorded legitimate value. In effect the
      window stops shrinking and slides at a fixed width until a genuinely
      better majority letter appears and lifts mx.
    - Recomputing the true max over the map on every shrink would also be
      correct, just slower (O(26) per step) and no more accurate in terms
      of the final result.
    - l and r each only move forward, so this is linear, not quadratic.

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = 0, mx = 0, res = 0 and an empty count map.
    2. While r < n:
         a. mp[s[r]]++ (grow the window), then mx = max(mx, mp[s[r]]).
         b. While (r - l + 1) - mx > k (too many characters to rewrite):
              - mp[s[l]]--, erase the key if its count hits 0
              - l++ (shrink from the left)
         c. The window is now fixable -> res = max(res, r - l + 1).
         d. r++.
    3. Return res.

    Time Complexity: O(n log 26) = O(n) - each index enters and leaves the
                      window once; every map operation is O(log 26), a
                      constant. An int[26] table removes the log entirely.
    Space Complexity: O(26) = O(1) - at most one key per uppercase letter.
*/
int characterReplacement(string s, int k) {
  int n = s.size();
  int l = 0, r = 0, mx = 0, res = 0;
  map<char, int> mp;

  while (r < n) {
    mp[s[r]]++;                        // grow: s[r] joins the window
    mx = max(mx, mp[s[r]]);            // best majority count seen (may go stale)

    while ((r - l + 1) - mx > k) {     // non-majority chars exceed the budget
      mp[s[l]]--;
      if (mp[s[l]] == 0) {
        mp.erase(s[l]);
      }
      l++;                             // shrink from the left
    }

    res = max(res, (r - l + 1));       // window is fixable with <= k changes
    r++;
  }

  return res;
}

int main() {
  vector<pair<string, int>> tests = {{"ABAB", 2}, {"AABABBA", 1}, {"AAAA", 0}};

  for (auto& t : tests) {
    cout << "Input:  s = \"" << t.first << "\", k = " << t.second << endl;
    cout << "Output: " << characterReplacement(t.first, t.second) << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "AABABBA", k = 1   (n = 7, answer = 4)
    ==========================================================================

      index:  0  1  2  3  4  5  6
      char:   A  A  B  A  B  B  A

    Tracked state:
      mp    - letter -> how many times it occurs inside the window [l..r]
      mx    - the largest count ever seen (never decreased; may be stale)
      cost  - (r - l + 1) - mx, the characters that would need rewriting
      l, r  - the window bounds; the window is s[l..r], width = r - l + 1
      res   - the best (largest) width seen so far

    Initial state: l = 0, r = 0, mx = 0, res = 0, mp = {}

    --------------------------------------------------------------------------
    r = 0, s[r] = 'A'
      add          mp['A']++ -> mp = {A:1},  mx = max(0, 1) = 1
      cost         width 1 - mx 1 = 0 <= 1  -> no shrink
      window       s[0..0] = "A", width = 1
      measure      res = max(0, 1) = 1
      advance      l = 0, r -> 1

    --------------------------------------------------------------------------
    r = 1, s[r] = 'A'
      add          mp['A']++ -> mp = {A:2},  mx = max(1, 2) = 2
      cost         width 2 - mx 2 = 0 <= 1  -> no shrink
      window       s[0..1] = "AA", width = 2
      measure      res = max(1, 2) = 2
      advance      l = 0, r -> 2

    --------------------------------------------------------------------------
    r = 2, s[r] = 'B'
      add          mp['B']++ -> mp = {A:2, B:1},  mx = max(2, 1) = 2
      cost         width 3 - mx 2 = 1 <= 1  -> no shrink (rewrite that one B)
      window       s[0..2] = "AAB", width = 3   -> "AAA" with 1 change
      measure      res = max(2, 3) = 3
      advance      l = 0, r -> 3

    --------------------------------------------------------------------------
    r = 3, s[r] = 'A'
      add          mp['A']++ -> mp = {A:3, B:1},  mx = max(2, 3) = 3
      cost         width 4 - mx 3 = 1 <= 1  -> no shrink
      window       s[0..3] = "AABA", width = 4  -> "AAAA" with 1 change
      measure      res = max(3, 4) = 4                    <-- ANSWER FOUND
      advance      l = 0, r -> 4

    --------------------------------------------------------------------------
    r = 4, s[r] = 'B'      <-- OVER BUDGET: two B's to rewrite, k = 1
      add          mp['B']++ -> mp = {A:3, B:2},  mx = max(3, 3) = 3
      cost         width 5 - mx 3 = 2 > 1  -> shrink
                   step 1: drop s[l=0] = 'A'
                             mp['A']-- -> 2, not zero, key stays
                             mp = {A:2, B:2},  l -> 1
                             re-check: width 4 - mx 3 = 1 <= 1 -> stop

                     A  A  B  A  B  B  A
                        ^        ^
                        l        r        window = "ABAB"

      HERE IS THE STALE mx:
          the window "ABAB" really has a max count of 2 (A:2, B:2), so its
          true cost is 4 - 2 = 2, which does NOT fit in k = 1. But mx is
          still 3 from the earlier "AABA", so the loop stopped early and
          this invalid window survives.
          It does no damage: the window is width 4, and res is ALREADY 4
          from the genuinely valid "AABA". A stale mx can only let through
          windows no wider than the one that set mx, so res never rises
          above a width that was legitimately achieved.

      window       s[1..4] = "ABAB", width = 4
      measure      res = max(4, 4) = 4    (no change)
      advance      l = 1, r -> 5

    --------------------------------------------------------------------------
    r = 5, s[r] = 'B'
      add          mp['B']++ -> mp = {A:2, B:3},  mx = max(3, 3) = 3
      cost         width 5 - mx 3 = 2 > 1  -> shrink
                   step 1: drop s[l=1] = 'A'
                             mp['A']-- -> 1, mp = {A:1, B:3},  l -> 2
                             re-check: width 4 - mx 3 = 1 <= 1 -> stop
      window       s[2..5] = "BABB", width = 4   -> "BBBB" with 1 change,
                   and this time mx = 3 is genuine (B really occurs 3 times)
      measure      res = max(4, 4) = 4
      advance      l = 2, r -> 6

    --------------------------------------------------------------------------
    r = 6, s[r] = 'A'
      add          mp['A']++ -> mp = {A:2, B:3},  mx = max(3, 2) = 3
      cost         width 5 - mx 3 = 2 > 1  -> shrink
                   step 1: drop s[l=2] = 'B'
                             mp['B']-- -> 2, mp = {A:2, B:2},  l -> 3
                             re-check: width 4 - mx 3 = 1 <= 1 -> stop
      window       s[3..6] = "ABBA", width = 4
      measure      res = max(4, 4) = 4
      advance      l = 3, r -> 7   ->   r == n, loop ends

    --------------------------------------------------------------------------
    RETURN res = 4      ("AABA" at s[0..3], one B changed to A -> "AAAA";
                         "BABB" at s[2..5] is an equally good answer)

    ==========================================================================
    Summary table
    ==========================================================================

    | r | s[r] | mp after add | mx | width-mx | shrunk? | l | window | w | res |
    |---|------|--------------|----|----------|---------|---|--------|---|-----|
    | 0 |  A   | {A:1}        | 1  |    0     | no      | 0 | "A"    | 1 |  1  |
    | 1 |  A   | {A:2}        | 2  |    0     | no      | 0 | "AA"   | 2 |  2  |
    | 2 |  B   | {A:2,B:1}    | 2  |    1     | no      | 0 | "AAB"  | 3 |  3  |
    | 3 |  A   | {A:3,B:1}    | 3  |    1     | no      | 0 | "AABA" | 4 |  4  |
    | 4 |  B   | {A:3,B:2}    | 3  |    2     | 1 step  | 1 | "ABAB" | 4 |  4  |
    | 5 |  B   | {A:2,B:3}    | 3  |    2     | 1 step  | 2 | "BABB" | 4 |  4  |
    | 6 |  A   | {A:2,B:3}    | 3  |    2     | 1 step  | 3 | "ABBA" | 4 |  4  |

    Read the last three rows as a unit: once mx reached 3 the window stopped
    growing and simply SLID rightwards at a fixed width of 4, shedding one
    character on the left for each one gained on the right. That is exactly
    the behaviour the never-decreasing mx produces - the window can only
    widen again if some letter's count actually beats the old record.

    Why this is O(n) and not O(n^2):
      r advanced 7 times and l advanced 3 times in total. Every index is
      added to the window once and removed at most once, so all the shrink
      steps put together cost at most n, rather than n per outer iteration.

    Optimisation note (not applied, to keep the code as written):
      replacing map<char,int> with int cnt[26] drops the log factor and the
      erase bookkeeping - mp.size() is never used in this problem, so there
      is nothing the erase is protecting here (unlike in fruits-into-baskets,
      where mp.size() is the whole condition).
*/
