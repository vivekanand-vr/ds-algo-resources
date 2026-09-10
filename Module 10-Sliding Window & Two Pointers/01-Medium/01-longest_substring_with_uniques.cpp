#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s, find the length of the longest substring that
//    contains no repeating characters.
//
// Example:
// s = "abcabcbb" -> 3   ("abc")
// s = "bbbbb"    -> 1   ("b")
// s = "pwwkew"   -> 3   ("wke", not "pwke" - that is a subsequence)

/*
    Approach: Variable-size sliding window with a character count map

    - A substring without repeats is exactly a window in which every
      character count is 1. So keep a window [l, r] and a map of how
      many times each character inside it occurs.
    - Extend the window one character at a time by moving r right. The
      only character that can possibly break the "no repeats" invariant
      is the one just added, so it is enough to check whether
      count[s[r]] became 2: if so, shrink from the left (dropping
      counts as l advances) until that duplicate is gone.
    - Because the invariant is restored before measuring, every
      r - l + 1 seen is the length of a valid duplicate-free substring,
      and the largest of those is the answer.
    - l and r each only ever move forward, so despite the nested while
      loop every index is added and removed at most once - the scan is
      linear, not quadratic.

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = 0, best = 0 and an empty count map.
    2. While r < n:
         a. Increment the count of s[r] (grow the window).
         b. While count[s[r]] > 1 (the new character is a duplicate):
              - decrement count[s[l]], erase the key if it hits 0
              - l++ (shrink from the left)
         c. The window is now duplicate-free -> best = max(best, r - l + 1).
         d. r++.
    3. Return best.

    Time Complexity: O(n log k) - each index enters and leaves the window
                      once, and every map operation costs O(log k) for k
                      distinct characters. Swapping map for unordered_map
                      (or a fixed int[256] table) makes it O(n).
    Space Complexity: O(k) for the count map, k = distinct characters
*/
int lengthOfLongestSubstring(string s) {
  int n = s.length();
  int l = 0, r = 0, c = 0;
  map<char, int> mp;

  while (r < n) {
    mp[s[r]]++;                    // grow the window to include s[r]

    while (mp[s[r]] > 1) {         // s[r] duplicated -> shrink from the left
      mp[s[l]]--;
      if (mp[s[l]] == 0) {
        mp.erase(s[l]);            // drop keys at zero so size() = distinct count
      }
      l++;
    }

    c = max(c, r - l + 1);         // window is duplicate-free here
    r++;
  }

  return c;
}

int main() {
  vector<string> tests = {"abcabcbb", "bbbbb", "pwwkew"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: " << lengthOfLongestSubstring(s) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "pwwkew"   (n = 6, answer = 3)
    ==========================================================================

      index:   0   1   2   3   4   5
      char:    p   w   w   k   e   w

    Tracked state:
      mp    - character -> how many times it occurs inside the window [l..r]
      l, r  - the window bounds; the window is s[l..r], width = r - l + 1
      c     - the best (largest) width seen so far

    Initial state: l = 0, r = 0, c = 0, mp = {}

    --------------------------------------------------------------------------
    r = 0, s[r] = 'p'
      add          mp['p']++            -> mp = {p:1}
      shrink?      mp['p'] = 1, not > 1 -> no shrink needed
      window       s[0..0] = "p", width = 1
      measure      c = max(0, 1) = 1
      advance      l = 0, r -> 1

    --------------------------------------------------------------------------
    r = 1, s[r] = 'w'
      add          mp['w']++            -> mp = {p:1, w:1}
      shrink?      mp['w'] = 1, not > 1 -> no shrink needed
      window       s[0..1] = "pw", width = 2
      measure      c = max(1, 2) = 2                      <-- best so far
      advance      l = 0, r -> 2

    --------------------------------------------------------------------------
    r = 2, s[r] = 'w'      <-- DUPLICATE: 'w' is already in the window
      add          mp['w']++            -> mp = {p:1, w:2}
      shrink       mp['w'] = 2 > 1, so drop characters from the left until
                   the second 'w' is gone. Note this takes TWO steps: 'p' is
                   not the duplicate, but it sits to the left of the old 'w'
                   and so has to leave first.

                   step 1: drop s[l=0] = 'p'
                             mp['p']-- -> 0, count hit zero -> erase key 'p'
                             mp = {w:2},  l -> 1
                             re-check: mp['w'] = 2 > 1, keep shrinking
                   step 2: drop s[l=1] = 'w'     (the OLD 'w')
                             mp['w']-- -> 1, not zero, key stays
                             mp = {w:1},  l -> 2
                             re-check: mp['w'] = 1, not > 1 -> stop

                       p   w   w   k   e   w
                               ^
                              l,r          window is back to a single 'w'

      window       s[2..2] = "w", width = 1
      measure      c = max(2, 1) = 2      (unchanged - this window is smaller)
      advance      l = 2, r -> 3

    --------------------------------------------------------------------------
    r = 3, s[r] = 'k'
      add          mp['k']++            -> mp = {w:1, k:1}
      shrink?      mp['k'] = 1, not > 1 -> no shrink needed
      window       s[2..3] = "wk", width = 2
      measure      c = max(2, 2) = 2
      advance      l = 2, r -> 4

    --------------------------------------------------------------------------
    r = 4, s[r] = 'e'
      add          mp['e']++            -> mp = {w:1, k:1, e:1}
      shrink?      mp['e'] = 1, not > 1 -> no shrink needed
      window       s[2..4] = "wke", width = 3
      measure      c = max(2, 3) = 3                      <-- new best
      advance      l = 2, r -> 5

    --------------------------------------------------------------------------
    r = 5, s[r] = 'w'      <-- DUPLICATE again
      add          mp['w']++            -> mp = {w:2, k:1, e:1}
      shrink       mp['w'] = 2 > 1
                   step 1: drop s[l=2] = 'w'
                             mp['w']-- -> 1, not zero, key stays
                             mp = {w:1, k:1, e:1},  l -> 3
                             re-check: mp['w'] = 1, not > 1 -> stop
                   (only ONE step this time - the duplicate happened to be
                    sitting exactly at the left edge of the window)

                       p   w   w   k   e   w
                                   ^       ^
                                   l       r        window = "kew"

      window       s[3..5] = "kew", width = 3
      measure      c = max(3, 3) = 3
      advance      l = 3, r -> 6   ->   r == n, loop ends

    --------------------------------------------------------------------------
    RETURN c = 3        ("wke" at s[2..4], or equally "kew" at s[3..5])

    ==========================================================================
    Summary table
    ==========================================================================

    | r | s[r] | mp after add     | shrunk? | l | window | width | c |
    |---|------|------------------|---------|---|--------|-------|---|
    | 0 |  p   | {p:1}            | no      | 0 | "p"    |   1   | 1 |
    | 1 |  w   | {p:1, w:1}       | no      | 0 | "pw"   |   2   | 2 |
    | 2 |  w   | {p:1, w:2}       | 2 steps | 2 | "w"    |   1   | 2 |
    | 3 |  k   | {w:1, k:1}       | no      | 2 | "wk"   |   2   | 2 |
    | 4 |  e   | {w:1, k:1, e:1}  | no      | 2 | "wke"  |   3   | 3 |
    | 5 |  w   | {w:2, k:1, e:1}  | 1 step  | 3 | "kew"  |   3   | 3 |

    Why this is O(n) and not O(n^2):
      r advanced 6 times (once per character) and l advanced 3 times in total
      (0 -> 2 at r = 2, then 2 -> 3 at r = 5). That is 9 pointer steps for a
      6-character string - never 6 x 6. The inner while loop looks nested, but
      l can only ever move forward and never past r, so all the shrinking put
      together costs at most n steps for the whole run.

    Note on the erase:
      at r = 2 step 1, mp['p'] dropped to 0 and the key was ERASED. Leaving a
      zero-count key behind would not break THIS problem (the check is on
      mp[s[r]], not on mp.size()), but it is the same habit that the distinct-
      count variants depend on, where mp.size() must equal the number of
      distinct characters actually inside the window.
*/
