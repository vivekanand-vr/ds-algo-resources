#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s consisting only of the characters 'a', 'b' and 'c',
//    return the number of SUBSTRINGS (contiguous) that contain at least one
//    occurrence of all three characters.
//
// Example:
// s = "abcabc" -> 10  ("abc", "abca", "abcab", "abcabc", "bca", "bcab",
//                      "bcabc", "cab", "cabc", "abc")
// s = "aaacb"  -> 3   ("aaacb", "aacb", "acb")
// s = "abc"    -> 1   (the whole string)

/*
    Approach: Track the last seen index of each character, count by right end

    - Count substrings by their RIGHT endpoint: for each i, ask how many
      start positions j give a valid substring s[j..i]. Summing that over
      all i visits every substring exactly once.
    - Fix the right end at i. The substring s[j..i] contains all three
      characters exactly when it reaches back far enough to cover the most
      recent 'a', the most recent 'b' AND the most recent 'c' - any earlier
      occurrence is also inside, so only the LATEST one of each matters.
      Let a, b, c be those last-seen indices and early = min(a, b, c).
      Then s[j..i] is valid precisely when j <= early.
    - So the valid starts are j = 0, 1, ..., early, which is early + 1 of
      them. That single line is the whole algorithm - no inner loop.
    - The key insight is that validity is MONOTONE in the start position:
      if s[j..i] is valid then so is s[j'..i] for every j' < j, because a
      longer substring can only gain characters. That is what collapses
      the count for a given i into one number instead of a scan.
    - This is the same "count by right endpoint" idea as the atMost trick
      in 05/06, but here the boundary is found from the three last-seen
      indices directly, so no second pointer and no shrink loop is needed.
    - The `else` branch assigns c, relying on the problem's guarantee that
      the string holds nothing but 'a', 'b' and 'c'. Any other character
      would silently be treated as a 'c'.

    Algorithm Steps
    ----------------
    1. Initialize a = b = c = -1 (none seen yet) and res = 0.
    2. For i from 0 to n-1:
         a. Update the last-seen index of s[i]: a, b or c = i.
         b. If all three have been seen (none is still -1):
              - early = min(a, b, c)   (the leftmost of the three latest)
              - res += early + 1       (valid starts are 0 .. early)
    3. Return res.

    Time Complexity: O(n) - one pass, O(1) work per character.
    Space Complexity: O(1) - three indices and an accumulator.
*/
int numberOfSubstrings(string s) {
  int n = s.length();
  int a = -1, b = -1, c = -1, res = 0;   // last index at which each was seen

  for (int i = 0; i < n; ++i) {
    if (s[i] == 'a')
      a = i;
    else if (s[i] == 'b')
      b = i;
    else
      c = i;                             // input is guaranteed to be a/b/c

    if (a != -1 && b != -1 && c != -1) {
      int early = min({a, b, c});        // leftmost of the three latest
      res += early + 1;                  // starts 0..early all work
    }
  }

  return res;
}

int main() {
  vector<string> tests = {"abcabc", "aaacb", "abc"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: " << numberOfSubstrings(s) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "abcabc"   (n = 6, answer = 10)
    ==========================================================================

      index:  0  1  2  3  4  5
      char:   a  b  c  a  b  c

    Tracked state:
      a,b,c - the LAST index at which each character was seen (-1 = never)
      early - min(a, b, c), the leftmost of those three latest positions
      res   - valid substrings found so far

    The question asked at every i is always the same:
      "with the right end pinned at i, how far LEFT can the start be and
       still cover one of each character?"  The answer is early, so there
       are early + 1 choices of start (0 through early inclusive).

    Initial state: a = -1, b = -1, c = -1, res = 0

    --------------------------------------------------------------------------
    i = 0, s[i] = 'a'
      update       a = 0            -> a=0,  b=-1, c=-1
      all seen?    no ('b' and 'c' never appeared yet)
      add          nothing, res = 0

                     a  b  c  a  b  c
                     ^                    no substring ending here can work
    --------------------------------------------------------------------------
    i = 1, s[i] = 'b'
      update       b = 1            -> a=0,  b=1,  c=-1
      all seen?    no ('c' still missing)
      add          nothing, res = 0

    --------------------------------------------------------------------------
    i = 2, s[i] = 'c'      <-- all three present for the first time
      update       c = 2            -> a=0,  b=1,  c=2
      all seen?    yes
      early        min(0, 1, 2) = 0      (the 'a' at index 0 is the binding
                                          constraint - reach it or fail)
      add          res += 0 + 1 = 1  -> res = 1
                   the 1 substring ending at 2: s[0..2] = "abc"

                     a  b  c  a  b  c
                     ^-----^            start must be <= 0
    --------------------------------------------------------------------------
    i = 3, s[i] = 'a'
      update       a = 3            -> a=3,  b=1,  c=2
      early        min(3, 1, 2) = 1      (now the 'b' at index 1 is binding;
                                          the newer 'a' freed up index 0)
      add          res += 1 + 1 = 2  -> res = 3
                   the 2 substrings ending at 3:
                     s[0..3] = "abca",  s[1..3] = "bca"

                     a  b  c  a  b  c
                        ^-----^         start may be 0 or 1
    --------------------------------------------------------------------------
    i = 4, s[i] = 'b'
      update       b = 4            -> a=3,  b=4,  c=2
      early        min(3, 4, 2) = 2      ('c' at index 2 is now binding)
      add          res += 2 + 1 = 3  -> res = 6
                   the 3 substrings ending at 4:
                     s[0..4] = "abcab",  s[1..4] = "bcab",  s[2..4] = "cab"

                     a  b  c  a  b  c
                           ^-----^      start may be 0, 1 or 2
    --------------------------------------------------------------------------
    i = 5, s[i] = 'c'
      update       c = 5            -> a=3,  b=4,  c=5
      early        min(3, 4, 5) = 3      ('a' at index 3 is binding again)
      add          res += 3 + 1 = 4  -> res = 10
                   the 4 substrings ending at 5:
                     s[0..5] = "abcabc", s[1..5] = "bcabc",
                     s[2..5] = "cabc",   s[3..5] = "abc"

                     a  b  c  a  b  c
                              ^-----^   start may be 0, 1, 2 or 3
    --------------------------------------------------------------------------
    RETURN res = 10      (1 + 2 + 3 + 4, grouped by right endpoint)

    ==========================================================================
    Summary table
    ==========================================================================

    | i | s[i] |  a |  b |  c | all seen? | early | added | res |
    |---|------|----|----|----|-----------|-------|-------|-----|
    | 0 |  a   |  0 | -1 | -1 | no        |   -   |   0   |  0  |
    | 1 |  b   |  0 |  1 | -1 | no        |   -   |   0   |  0  |
    | 2 |  c   |  0 |  1 |  2 | yes       |   0   |   1   |  1  |
    | 3 |  a   |  3 |  1 |  2 | yes       |   1   |   2   |  3  |
    | 4 |  b   |  3 |  4 |  2 | yes       |   2   |   3   |  6  |
    | 5 |  c   |  3 |  4 |  5 | yes       |   3   |   4   | 10  |

    Notice how `early` never goes backwards: 0, 1, 2, 3. It cannot, because
    updating one of the three indices to i only ever raises a value, and the
    minimum of three values that only rise can only rise. That monotonicity
    is the same behaviour a left pointer would show in a shrink-based
    solution - this code just computes its position instead of walking it.

    ==========================================================================
    Notes
    ==========================================================================

    Why only the LAST occurrence of each character matters:
      a substring s[j..i] covers index p whenever j <= p. If it covers the
      latest 'a' it automatically covers every earlier 'a' too, so earlier
      occurrences add no constraint. Three numbers are enough state for the
      entire prefix.

    The all-seen guard is redundant, but keep it for clarity:
      if any of a, b, c is still -1 then early = -1 and `res += early + 1`
      adds 0, so dropping the `if` would give the same answer. The explicit
      check states the intent - "no valid substring exists yet" - rather
      than relying on -1 + 1 cancelling out.

    Counting by right endpoint:
      the totals 1 + 2 + 3 + 4 in the table are the substrings grouped by
      where they END. Every substring has exactly one right endpoint, so
      the groups are disjoint and cover everything - nothing is counted
      twice and nothing is missed. Same accounting as the `c += (r-l+1)`
      line in 05-binary_subarray_with_sum_II.cpp and 06-nice_subarrays.cpp.

    Sliding-window alternative (not applied):
      the same answer comes from a shrinking window - keep counts of a, b,
      c in the window, and while all three are present, advance l and add
      (l) to the result. It is the more general pattern and extends to an
      arbitrary character set, but for a fixed alphabet of three the
      last-seen-index version is shorter, has no map, and does one pass
      with no inner loop.
*/
