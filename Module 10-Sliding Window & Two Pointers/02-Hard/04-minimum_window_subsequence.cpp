#include <bits/stdc++.h>
using namespace std;

// Q: Given strings s and t, return the SHORTEST substring w of s such that
//    t is a SUBSEQUENCE of w. If no such window exists, return "". If there
//    is more than one shortest window, return the leftmost one.
//
//    Contrast with 03-minimum_window_substring.cpp: there the characters of
//    t could appear in any order, so a frequency count sufficed. Here the
//    ORDER of t must be preserved, which rules counting out entirely.
//
// Example:
// s = "abcdebdde", t = "bde" -> "bcde"  (s[1..4]; "bdde" at s[5..8] is also
//                                        length 4, but "bcde" starts earlier)
// s = "abcdebdde", t = "bdd" -> "bdd"   (s[5..7]; the first window found is
//                                        "bcdebd", later improved on)
// s = "abc",       t = "d"   -> ""      (no window at all)

/*
    Approach: Two-scan greedy - forward to close a match, backward to tighten

    - Order matters, so the window predicate is "t is a subsequence of
      s[l..r]", which is NOT monotone in a way a single sliding window can
      exploit: a character count tells you nothing about whether the
      sequence can be threaded in order. So instead of one window with two
      pointers, this walks s and pins down one candidate window at a time.
    - FORWARD SCAN: from the current i, greedily match t left to right.
      Advance j on every hit and stop the moment j reaches m. Matching
      greedily is safe: taking the EARLIEST possible character for each
      t[j] can never make the rest of t harder to match, so the index
      where j completes is the smallest possible END for a window starting
      at or after i. Call it `end`.
    - BACKWARD SCAN: `end` is tight but the start is not - the forward scan
      took the earliest character for each t[j], which may sit far left of
      where it needed to be (in the example, the 'b' at index 1 with a
      useless 'c' after it). So walk back from `end` matching t RIGHT TO
      LEFT. By the same greedy argument in reverse, this lands on the
      LARGEST possible start for a window ending at `end`. Call it `start`.
    - The pair (start, end) is now tight on both sides and is a genuine
      minimal candidate. Record it if shorter than the best so far.
    - RESTART: continue from i = start + 1. Stepping past `start` is what
      guarantees progress and terminates the loop, and it loses nothing:
      any window starting at or before `start` and ending at or after
      `end` is no shorter than the one just recorded, and windows starting
      before `start` with a smaller end were already considered on an
      earlier round.
    - The backward scan re-reads characters the forward scan already
      visited, so a round can cost up to O(n) and the total is O(n * m) in
      the worst case rather than O(n). The classic alternative is a DP
      table over (i, j) which is O(n * m) time as well but O(n * m) space;
      this version is O(1) space and much faster in practice.

    Algorithm Steps
    ----------------
    1. Set bestLen = INT_MAX, bestSt = -1, i = 0.
    2. While i < n:
         a. FORWARD: j = 0; while i < n:
              - if s[i] == t[j], j++
              - if j == m, break WITHOUT advancing i (so i is the last
                matched index)
              - i++
         b. If j != m, no full match remains anywhere -> break out.
         c. end = i.
         d. BACKWARD: j = m - 1; while j >= 0:
              - if s[i] == t[j], j--
              - i--
            The loop overshoots by one (i-- runs once more after j hits
            -1), so start = i + 1 is the index that matched t[0].
         e. If end - start + 1 < bestLen, record bestLen and bestSt.
         f. i = start + 1 (resume just past this window's start).
    3. Return "" if bestSt == -1, else s.substr(bestSt, bestLen).

    Time Complexity: O(n * m) worst case - each round's two scans are O(n)
                      and there can be O(m)-ish rounds in bad inputs; on
                      typical inputs it behaves close to O(n).
    Space Complexity: O(1) - only indices, no map and no DP table.
*/
string minWindowSubsequence(string s, string t) {
  int n = s.length(), m = t.length();

  int bestLen = INT_MAX;
  int bestSt = -1;

  int i = 0;

  while (i < n) {
    // ---------------- FORWARD SCAN ----------------
    int j = 0;

    while (i < n) {
      if (s[i] == t[j]) {
        j++;                    // greedy: take the earliest match for t[j]
      }

      if (j == m) {
        break;  // Complete subsequence found
      }

      i++;
    }

    // Could not find the complete subsequence
    if (j != m) {
      break;
    }

    // ---------------- BACKWARD SCAN ----------------
    int end = i;                // i stopped ON the last matched character

    j = m - 1;  // Start matching t from the end

    while (j >= 0) {
      if (s[i] == t[j]) {
        j--;                    // greedy in reverse: latest match for t[j]
      }

      i--;
    }

    int start = i + 1;          // undo the extra i-- after j hit -1

    // ---------------- UPDATE ANSWER ----------------
    if ((end - start + 1) < bestLen) {
      bestLen = end - start + 1;   // strict <, so the leftmost shortest wins
      bestSt = start;
    }

    // Start searching for the next window
    i = start + 1;              // past `start`, which guarantees progress
  }

  if (bestSt == -1) {
    return "";
  }

  return s.substr(bestSt, bestLen);
}

int main() {
  vector<pair<string, string>> tests = {
      {"abcdebdde", "bde"}, {"abcdebdde", "bdd"}, {"abc", "d"}};

  for (auto& p : tests) {
    cout << "Input:  s = \"" << p.first << "\", t = \"" << p.second << "\""
         << endl;
    cout << "Output: \"" << minWindowSubsequence(p.first, p.second) << "\""
         << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "abcdebdde", t = "bde"   (n = 9, m = 3, answer "bcde")
    ==========================================================================

      index:  0  1  2  3  4  5  6  7  8
      char:   a  b  c  d  e  b  d  d  e

    Tracked state:
      i     - the roving cursor into s (shared by both scans)
      j     - how much of t is matched; forward it counts up to m,
              backward it counts down to -1
      end, start - the tight bounds of the candidate window this round
      bestLen, bestSt - the best window found so far

    ==========================================================================
    ROUND 1: starting from i = 0
    ==========================================================================

    FORWARD SCAN (match t = "bde" left to right, j = 0):
      i = 0, 'a' vs t[0]='b'  no match       -> i -> 1
      i = 1, 'b' vs t[0]='b'  MATCH, j -> 1  -> i -> 2
      i = 2, 'c' vs t[1]='d'  no match       -> i -> 3
      i = 3, 'd' vs t[1]='d'  MATCH, j -> 2  -> i -> 4
      i = 4, 'e' vs t[2]='e'  MATCH, j -> 3 == m -> BREAK, i stays 4

      end = 4.  A window ending at 4 exists; nothing ending earlier can
      work, because the scan took the earliest possible character each time.

                   a  b  c  d  e  b  d  d  e
                      ^        ^
                  matched b   matched e      j completed at index 4

    BACKWARD SCAN (match t right to left from i = 4, j = 2):
      i = 4, 'e' vs t[2]='e'  MATCH, j -> 1  -> i -> 3
      i = 3, 'd' vs t[1]='d'  MATCH, j -> 0  -> i -> 2
      i = 2, 'c' vs t[0]='b'  no match       -> i -> 1
      i = 1, 'b' vs t[0]='b'  MATCH, j -> -1 -> i -> 0
      j < 0, loop ends with i = 0 (one step past the match)

      start = i + 1 = 1

      Here the backward scan changed nothing about the start - the forward
      scan's 'b' at index 1 was already the latest usable one. The useless
      'c' at index 2 sits INSIDE the window and cannot be trimmed away.

    RECORD:
      window s[1..4] = "bcde", length 4 - 1 + 1 = 4
      4 < INT_MAX -> bestLen = 4, bestSt = 1

                   a  b  c  d  e  b  d  d  e
                      ^--------^
                       "bcde"

    RESTART: i = start + 1 = 2

    ==========================================================================
    ROUND 2: starting from i = 2
    ==========================================================================

    FORWARD SCAN (j = 0):
      i = 2, 'c' vs 'b'  no   -> i -> 3
      i = 3, 'd' vs 'b'  no   -> i -> 4
      i = 4, 'e' vs 'b'  no   -> i -> 5
      i = 5, 'b' vs 'b'  MATCH, j -> 1 -> i -> 6
      i = 6, 'd' vs 'd'  MATCH, j -> 2 -> i -> 7
      i = 7, 'd' vs 'e'  no   -> i -> 8
      i = 8, 'e' vs 'e'  MATCH, j -> 3 == m -> BREAK, i stays 8

      end = 8

    BACKWARD SCAN (j = 2, from i = 8):
      i = 8, 'e' vs t[2]='e'  MATCH, j -> 1  -> i -> 7
      i = 7, 'd' vs t[1]='d'  MATCH, j -> 0  -> i -> 6
      i = 6, 'd' vs t[0]='b'  no match       -> i -> 5
      i = 5, 'b' vs t[0]='b'  MATCH, j -> -1 -> i -> 4

      start = 5

      Note the backward scan picked the 'd' at index 7, not the one at
      index 6 that the forward scan used. Taking the LATEST 'd' is what
      keeps the start as far right as possible.

    RECORD:
      window s[5..8] = "bdde", length 4
      4 < 4 is FALSE (strict comparison) -> nothing recorded

      This is what makes the answer the LEFTMOST shortest window: an equal
      length never displaces the earlier one.

    RESTART: i = start + 1 = 6

    ==========================================================================
    ROUND 3: starting from i = 6
    ==========================================================================

    FORWARD SCAN (j = 0):
      i = 6, 'd' vs 'b'  no   -> i -> 7
      i = 7, 'd' vs 'b'  no   -> i -> 8
      i = 8, 'e' vs 'b'  no   -> i -> 9
      i = 9 -> i < n fails, inner loop exits with j = 0

      j != m -> BREAK the outer loop. No 'b' remains, so no further window
      can exist and the search is over.

    --------------------------------------------------------------------------
    RETURN s.substr(1, 4) = "bcde"

    ==========================================================================
    Summary table
    ==========================================================================

    | round | i at start | end | start | window   | len | bestLen | bestSt |
    |-------|------------|-----|-------|----------|-----|---------|--------|
    |   1   |     0      |  4  |   1   | "bcde"   |  4  |    4    |   1    |
    |   2   |     2      |  8  |   5   | "bdde"   |  4  |    4    |   1    |
    |   3   |     6      |  -  |   -   | no match |  -  |    4    |   1    |

    ==========================================================================
    A second dry run, abridged: s = "abcdebdde", t = "bdd" -> "bdd"
    ==========================================================================

      This one shows a later round genuinely IMPROVING the answer.

      Round 1: forward from i = 0 matches b@1, d@3, d@6 -> end = 6
               backward from 6 matches d@6, d@3, b@1   -> start = 1
               window s[1..6] = "bcdebd", len 6 -> bestLen = 6, bestSt = 1
               restart at i = 2
      Round 2: forward from i = 2 matches b@5, d@6, d@7 -> end = 7
               backward from 7 matches d@7, d@6, b@5   -> start = 5
               window s[5..7] = "bdd", len 3 < 6 -> bestLen = 3, bestSt = 5
               restart at i = 6
      Round 3: forward from i = 6 finds no 'b' -> j = 0 != m -> break

      RETURN s.substr(5, 3) = "bdd"

    ==========================================================================
    Notes
    ==========================================================================

    Why the forward scan does not advance i on completion:
      the `if (j == m) break;` sits BEFORE the `i++`, so i is left pointing
      AT the character that completed the match. That is what makes
      `end = i` correct. Moving the break after the increment would put
      end one past the window and silently lengthen every answer by one.

    Why the backward scan overshoots by one:
      `i--` runs unconditionally, including on the iteration that drops j
      from 0 to -1. So when the loop exits, i is one BEFORE the character
      that matched t[0] - hence `start = i + 1`. It cannot run off the
      front of the array, because a complete match is known to exist
      within [start..end] and j reaches -1 before i can pass it.

    Why restarting at start + 1 is safe and not start:
      restarting AT start would re-find the exact same window forever -
      the loop would never terminate. Stepping one past it is what forces
      progress. Nothing is lost: any other window containing [start..end]
      is at least as long, and shorter windows further right are found on
      later rounds.

    Why a frequency-count sliding window cannot be reused here:
      03-minimum_window_substring.cpp gets away with counts because order
      is irrelevant there. For s = "ba", t = "ab" the counts match
      perfectly, yet "ab" is not a subsequence of "ba" - so any
      count-based predicate would report a false window. Order forces an
      actual left-to-right threading, which is what the forward scan does.

    On the constraints:
      both scans index t[j] directly and assume t is non-empty; with
      m = 0 the forward loop would compare against t[0] on an empty
      string. The problem guarantees non-empty inputs, so no guard is
      added, but it is worth knowing the code leans on that.

    Alternative worth knowing:
      the DP formulation defines dp[i][j] as the start index of the
      shortest window of s[0..i] that contains t[0..j] as a subsequence.
      Same O(n * m) time, but O(n * m) space and far more code. The
      two-scan version above is the one to remember.
*/
