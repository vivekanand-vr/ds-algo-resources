#include <bits/stdc++.h>
using namespace std;

// Q: Given two strings s and t, return the SHORTEST substring of s that
//    contains every character of t, including duplicates. If no such
//    substring exists, return "". The characters may appear in any order.
//
// Example:
// s = "ADOBECODEBANC", t = "ABC" -> "BANC"   (s[9..12]; "ADOBEC" also
//                                             qualifies but is longer)
// s = "a",             t = "a"   -> "a"
// s = "a",             t = "aa"  -> ""       (only one 'a' available, two
//                                             are required)

/*
    Approach: Variable-size sliding window with a deficit count map

    - Keep a window [l, r] and ask a single question about it: does it
      cover all of t? Grow r until the answer is yes, then shrink from the
      left as far as possible while it stays yes - the window is minimal
      at that moment, so record it. Repeat. Every candidate answer is a
      window that is tight on both ends, and the smallest is the result.
    - The whole trick is how coverage is tracked in O(1) per step. Seed
      mp with the counts of t, then read mp[ch] as a DEFICIT:
          mp[ch] > 0   still need that many more ch
          mp[ch] == 0  exactly satisfied
          mp[ch] < 0   the window holds |mp[ch]| surplus copies
      Characters absent from t start at 0 and immediately go negative, so
      they are "surplus" from the first sight - which is exactly right,
      they are never needed.
    - On growing, `mp[s[r]]--` then `if (mp[s[r]] >= 0) c++`. The count
      landing at >= 0 means the character was still NEEDED before the
      decrement, so it closed a real gap. Landing negative means it was a
      surplus copy and must not be credited. c therefore counts characters
      matched WITH multiplicity, and full coverage is c == m where
      m = t.length() - not the number of distinct characters.
    - On shrinking, the mirror image: `mp[s[l]]++` then
      `if (mp[s[l]] > 0) c--`. A count rising above 0 means the character
      just removed was load-bearing, so coverage is broken and the shrink
      loop stops. Rising to 0 or below means a surplus copy left and the
      window is still valid, so shrinking continues.
    - `< len` is a strict comparison, so the FIRST shortest window found
      is the one kept. Any later window of equal length is ignored.
    - l and r each only move forward, so despite the nested while loop
      every index is added and removed at most once - the scan is linear.

    Algorithm Steps
    ----------------
    1. Fill mp with the character counts of t. Set len = INT_MAX, st = -1,
       l = 0, r = 0, c = 0 (matched characters).
    2. While r < n:
         a. mp[s[r]]-- (grow the window to include s[r]).
         b. If mp[s[r]] >= 0, c++ (that copy closed a real deficit).
         c. While c == m (the window covers all of t):
              - if r - l + 1 < len, record len and st = l
              - mp[s[l]]++, and if mp[s[l]] > 0 then c-- (coverage broken)
              - l++ (shrink from the left)
         d. r++.
    3. If st == -1 no window ever covered t -> return "".
       Otherwise return s.substr(st, len).

    Time Complexity: O((n + m) log k) - each index of s enters and leaves
                      the window once, with an O(log k) map operation per
                      step. unordered_map or an int[128] table makes it
                      O(n + m).
    Space Complexity: O(k) where k is the number of distinct characters in
                      s and t combined. Note mp collects keys for the
                      characters of s that are NOT in t as well, since
                      mp[s[r]]-- inserts them; an int[128] table sidesteps
                      that growth entirely.
*/
string minWindow(string s, string t) {
  int n = s.length(), m = t.length();
  string res = "";
  int len = INT_MAX, st = -1, l = 0, r = 0, c = 0;

  map<char, int> mp;
  for (char i : t) mp[i]++;  // fill up the frequency

  while (r < n) {
    mp[s[r]]--;                    // grow: one copy of s[r] is now inside
    if (mp[s[r]] >= 0) c++;        // it was still needed -> real progress

    while (c == m) {               // window covers all of t -> try to tighten
      if ((r - l + 1) < len) {
        len = (r - l + 1);         // strict <, so the first shortest wins
        st = l;
      }

      mp[s[l]]++;                  // shrink: s[l] leaves the window
      if (mp[s[l]] > 0) c--;       // it was load-bearing -> coverage broken
      l++;
    }

    r++;
  }

  if (st == -1) return "";
  return s.substr(st, len);
}

int main() {
  vector<pair<string, string>> tests = {
      {"ADOBECODEBANC", "ABC"}, {"a", "a"}, {"a", "aa"}};

  for (auto& p : tests) {
    cout << "Input:  s = \"" << p.first << "\", t = \"" << p.second << "\""
         << endl;
    cout << "Output: \"" << minWindow(p.first, p.second) << "\"" << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "ADOBECODEBANC", t = "ABC"   (n = 13, m = 3, answer "BANC")
    ==========================================================================

      index:  0  1  2  3  4  5  6  7  8  9 10 11 12
      char:   A  D  O  B  E  C  O  D  E  B  A  N  C

    Tracked state:
      mp    - character -> DEFICIT (how many more copies the window needs)
              positive = still needed, 0 = satisfied, negative = surplus
      c     - characters of t matched so far, counted with multiplicity
      l, r  - the window bounds; the window is s[l..r]
      len, st - length and start index of the best window found so far

    Coverage means c == m == 3. Only keys that matter are shown in mp.

    Initial state: mp = {A:1, B:1, C:1}, c = 0, l = 0, r = 0,
                   len = INT_MAX, st = -1

    --------------------------------------------------------------------------
    GROW PHASE 1: r = 0..5, hunting for the first covering window
    --------------------------------------------------------------------------

    r = 0, 'A':  mp[A] 1 -> 0,  0 >= 0 -> c = 1     (A satisfied)
    r = 1, 'D':  mp[D] 0 -> -1, negative -> no credit (D is not in t)
    r = 2, 'O':  mp[O] 0 -> -1, no credit
    r = 3, 'B':  mp[B] 1 -> 0,  0 >= 0 -> c = 2     (B satisfied)
    r = 4, 'E':  mp[E] 0 -> -1, no credit
    r = 5, 'C':  mp[C] 1 -> 0,  0 >= 0 -> c = 3     <-- COVERED

                   A  D  O  B  E  C  O  D  E  B  A  N  C
                   ^              ^
                   l              r        "ADOBEC" covers A, B, C

    SHRINK at r = 5:
      iter 1: window s[0..5] len 6 < INT_MAX -> len = 6, st = 0 ("ADOBEC")
              drop s[0] = 'A': mp[A] 0 -> 1,  1 > 0 -> c = 2, l -> 1
              coverage broken (the only A is gone) -> stop shrinking

      mp = {A:1, B:0, C:0, D:-1, O:-1, E:-1},  c = 2,  l = 1

    --------------------------------------------------------------------------
    GROW PHASE 2: r = 6..10, waiting for another 'A'
    --------------------------------------------------------------------------

    r = 6, 'O':  mp[O] -1 -> -2, no credit  (a second useless O)
    r = 7, 'D':  mp[D] -1 -> -2, no credit
    r = 8, 'E':  mp[E] -1 -> -2, no credit
    r = 9, 'B':  mp[B]  0 -> -1, NEGATIVE -> no credit
                 this is the surplus rule doing its job: a second B adds
                 nothing, because B was already satisfied. Crediting it
                 would falsely report coverage without an A present.
    r = 10, 'A': mp[A]  1 -> 0,  0 >= 0 -> c = 3    <-- COVERED again

    SHRINK at r = 10:
      iter 1: window s[1..10] len 10, not < 6 -> no record
              drop s[1] = 'D': mp[D] -2 -> -1, not > 0 -> c stays 3, l -> 2
      iter 2: window s[2..10] len 9, no record
              drop s[2] = 'O': mp[O] -2 -> -1, not > 0 -> l -> 3
      iter 3: window s[3..10] len 8, no record
              drop s[3] = 'B': mp[B] -1 -> 0,  not > 0 -> l -> 4
              the FIRST B leaves and coverage survives, because the second
              B at index 9 is still inside. This is precisely what the
              negative bookkeeping is for.
      iter 4: window s[4..10] len 7, no record
              drop s[4] = 'E': mp[E] -2 -> -1, not > 0 -> l -> 5
      iter 5: window s[5..10] len 6, NOT < 6 (strict) -> no record
              drop s[5] = 'C': mp[C] 0 -> 1,  1 > 0 -> c = 2, l -> 6
              coverage broken -> stop

      mp = {A:0, B:0, C:1, D:-1, O:-1, E:-1},  c = 2,  l = 6
      best is still len = 6, st = 0

    --------------------------------------------------------------------------
    GROW PHASE 3: r = 11..12, waiting for another 'C'
    --------------------------------------------------------------------------

    r = 11, 'N': mp[N] 0 -> -1, no credit
    r = 12, 'C': mp[C] 1 -> 0,  0 >= 0 -> c = 3     <-- COVERED again

    SHRINK at r = 12:
      iter 1: window s[6..12] len 7, not < 6 -> no record
              drop s[6] = 'O': mp[O] -1 -> 0, not > 0 -> l -> 7
      iter 2: window s[7..12] len 6, NOT < 6 -> no record
              drop s[7] = 'D': mp[D] -1 -> 0, not > 0 -> l -> 8
      iter 3: window s[8..12] len 5 < 6 -> len = 5, st = 8 ("EBANC")
              drop s[8] = 'E': mp[E] -1 -> 0, not > 0 -> l -> 9
      iter 4: window s[9..12] len 4 < 5 -> len = 4, st = 9 ("BANC")  <-- BEST
              drop s[9] = 'B': mp[B] 0 -> 1, 1 > 0 -> c = 2, l -> 10
              coverage broken -> stop

                   A  D  O  B  E  C  O  D  E  B  A  N  C
                                              ^        ^
                                              l        r      "BANC"

    r -> 13 == n, loop ends.

    --------------------------------------------------------------------------
    RETURN s.substr(9, 4) = "BANC"

    ==========================================================================
    Summary table
    ==========================================================================

    |  r | s[r] | mp[s[r]] after-- | credit? |  c | shrink steps | len | st |
    |----|------|------------------|---------|----|--------------|-----|----|
    |  0 |  A   |        0         | yes     |  1 |      -       |  -  | -1 |
    |  1 |  D   |       -1         | no      |  1 |      -       |  -  | -1 |
    |  2 |  O   |       -1         | no      |  1 |      -       |  -  | -1 |
    |  3 |  B   |        0         | yes     |  2 |      -       |  -  | -1 |
    |  4 |  E   |       -1         | no      |  2 |      -       |  -  | -1 |
    |  5 |  C   |        0         | yes     |  3 |  1 (l 0->1)  |  6  |  0 |
    |  6 |  O   |       -2         | no      |  2 |      -       |  6  |  0 |
    |  7 |  D   |       -2         | no      |  2 |      -       |  6  |  0 |
    |  8 |  E   |       -2         | no      |  2 |      -       |  6  |  0 |
    |  9 |  B   |       -1         | no      |  2 |      -       |  6  |  0 |
    | 10 |  A   |        0         | yes     |  3 |  5 (l 1->6)  |  6  |  0 |
    | 11 |  N   |       -1         | no      |  2 |      -       |  6  |  0 |
    | 12 |  C   |        0         | yes     |  3 |  4 (l 6->10) |  4  |  9 |

    Why this is O(n) and not O(n^2):
      r advanced 13 times and l advanced 10 times in total (1 + 5 + 4 shrink
      steps). That is 23 pointer steps for a 13-character string, never
      13 x 13. The inner while loop is nested in the source but not in the
      cost - l only moves forward and never passes r.

    ==========================================================================
    Notes
    ==========================================================================

    Why c is compared to m = t.length() and not to the distinct count:
      duplicates in t must each be found. For t = "AABC" the window needs
      TWO A's, and since mp[A] starts at 2, the first A lands mp[A] at 1
      (>= 0, credited) and the second lands it at 0 (>= 0, credited too) -
      two separate credits, so c must reach 4, not 3. The third A would
      land at -1 and be correctly ignored.

    Why `>= 0` on growing but `> 0` on shrinking:
      the two tests are asymmetric on purpose, because the count is read
      AFTER the update in both cases.
        growing:   deficit was d, becomes d-1. It was needed iff d > 0,
                   i.e. iff d-1 >= 0.
        shrinking: deficit was d, becomes d+1. It was needed iff d+1 > 0
                   is a strict gap, i.e. the window now lacks a copy.
      Writing `>= 0` in both places would decrement c on surplus removals
      and collapse the window far too early.

    The third example, s = "a", t = "aa":
      mp starts {a:2}. r = 0 lands mp[a] at 1, which is >= 0 so c = 1, but
      m = 2 so the shrink loop never runs. st stays -1 and "" is returned.
      The st == -1 sentinel is what distinguishes "no window found" from a
      legitimately empty answer.

    Related problems in this module:
      the grow-then-tighten skeleton here is the same one used in
      01-longest_substring_with_k_uniques.cpp; what changes is only the
      window predicate and whether the goal is the longest or the shortest
      window satisfying it.
*/
