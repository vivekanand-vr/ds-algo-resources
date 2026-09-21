#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given two strings needle and haystack, return the index of the first
//    occurrence of needle in haystack, or -1 if needle is not part of
//    haystack. Do NOT use the inbuilt search functions (no string::find).
//
// Example:
// haystack = "sadbutsad", needle = "sad"    -> 0   (also occurs at 6, but 0 is first)
// haystack = "leetcode",  needle = "leeto"  -> -1  (never occurs)
// haystack = "abaxabab",  needle = "abab"   -> 4

/*
    Approach: KMP (Knuth-Morris-Pratt) - prefix-function search

    - The naive search restarts the pattern from scratch after every
      mismatch, re-reading characters of the haystack it has already
      examined. That costs O(n * m) in the worst case (think
      haystack = "aaaaaaaab", needle = "aaab").
    - KMP's insight: when a mismatch happens after j characters matched,
      those j characters are ALREADY KNOWN - they are exactly needle[0..j-1].
      So we can work out, in advance and from the needle alone, how far the
      pattern can slide without missing a possible match. The haystack
      pointer never has to move backwards.
    - That precomputation is the LPS array (Longest proper Prefix which is
      also a Suffix). lps[i] = the length of the longest proper prefix of
      needle[0..i] that is also a suffix of needle[0..i]. "Proper" means it
      cannot be the whole substring.
    - On a mismatch at pattern position j, the next position to try is
      lps[j-1]: the longest prefix of the needle that still matches the
      text we just consumed. Everything shorter would be a wasted attempt,
      and everything longer has already been ruled out - so no possible
      occurrence is skipped.
    - Because i only ever advances and j only ever decreases on a mismatch
      (and j <= i always), the total work is O(n + m). See the amortised
      argument in the Notes.

    Algorithm Steps
    ----------------
    1. If needle is empty, return 0. If needle is longer than haystack,
       return -1.
    2. Build the LPS array for needle:
       a. lps[0] = 0 always (a single character has no proper prefix).
       b. Walk i from 1, tracking len = current candidate prefix length.
       c. If needle[i] == needle[len], extend: lps[i] = ++len, i++.
       d. Else if len > 0, fall back to len = lps[len-1] WITHOUT advancing
          i - try the next-best shorter candidate.
       e. Else lps[i] = 0, i++.
    3. Scan the haystack with i (text) and j (pattern), both from 0:
       a. If characters match, advance both; if j reaches m, the match
          started at i - m, so return it.
       b. Else if j > 0, slide the pattern: j = lps[j-1] (i stays put).
       c. Else advance i (nothing matched, move the text along).
    4. If the scan finishes, return -1.

    Time Complexity: O(n + m) - O(m) to build the LPS, O(n) to scan; the
                      text pointer i never moves backwards.
    Space Complexity: O(m) - the LPS array, one entry per needle character.
*/

// Builds the prefix function: lps[i] = length of the longest proper prefix
// of p[0..i] that is also a suffix of p[0..i].
vector<int> buildLPS(const string& p) {
    int m = p.size();
    vector<int> lps(m, 0);
    int len = 0;   // length of the prefix-suffix matched so far
    int i = 1;     // lps[0] is always 0, so start at 1

    while (i < m) {
        if (p[i] == p[len]) {
            lps[i] = ++len;   // extend the current prefix-suffix by one
            i++;
        } else if (len > 0) {
            len = lps[len - 1];   // fall back; deliberately do NOT advance i
        } else {
            lps[i] = 0;   // no prefix-suffix at all at this position
            i++;
        }
    }

    return lps;
}

int strStr(string haystack, string needle) {
    int n = haystack.size(), m = needle.size();
    if (m == 0) return 0;    // empty needle matches at index 0 by convention
    if (m > n) return -1;    // cannot possibly fit

    vector<int> lps = buildLPS(needle);

    int i = 0, j = 0;   // i indexes haystack, j indexes needle
    while (i < n) {
        if (haystack[i] == needle[j]) {
            i++;
            j++;
            if (j == m) return i - m;   // consumed the whole needle
        } else if (j > 0) {
            j = lps[j - 1];   // slide the pattern, keep i where it is
        } else {
            i++;              // nothing matched at all, advance the text
        }
    }

    return -1;
}

// Baseline for cross-checking only - the O(n*m) approach KMP improves on.
int strStrNaive(string haystack, string needle) {
    int n = haystack.size(), m = needle.size();
    if (m == 0) return 0;

    for (int i = 0; i + m <= n; ++i) {
        int j = 0;
        while (j < m && haystack[i + j] == needle[j]) ++j;
        if (j == m) return i;
    }

    return -1;
}

int main() {
    struct Test {
        string haystack, needle;
    };
    vector<Test> tests = {
        {"sadbutsad", "sad"},
        {"leetcode", "leeto"},
        {"abaxabab", "abab"},
        {"aaaaaaaab", "aaab"},
        {"mississippi", "issip"},
        {"abc", "abcd"},
        {"hello", "ll"}
    };

    for (auto& t : tests) {
        int fast = strStr(t.haystack, t.needle);
        int slow = strStrNaive(t.haystack, t.needle);

        cout << "haystack = \"" << t.haystack << "\", needle = \"" << t.needle
             << "\"" << endl;
        cout << "Output: " << fast
             << "   (naive baseline: " << slow
             << (fast == slow ? " - matches)" : " - MISMATCH)") << endl
             << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: haystack = "abaxabab", needle = "abab"   (n = 8, m = 4, answer 4)
    ==========================================================================

      haystack index:  0   1   2   3   4   5   6   7
      haystack char:   a   b   a   x   a   b   a   b

      needle index:    0   1   2   3
      needle char:     a   b   a   b

    --------------------------------------------------------------------------
    PHASE 1: build the LPS array for "abab"
    --------------------------------------------------------------------------

    Initial: lps = [0, 0, 0, 0], len = 0, i = 1

    i=1: p[1]='b' vs p[len=0]='a'   -> not equal, and len == 0
         action   lps[1] = 0, i -> 2
         state    lps = [0,0,0,0], len = 0

    i=2: p[2]='a' vs p[len=0]='a'   -> EQUAL
         action   len -> 1, lps[2] = 1, i -> 3
         state    lps = [0,0,1,0], len = 1
         meaning  "aba" has proper prefix "a" == its suffix "a"

    i=3: p[3]='b' vs p[len=1]='b'   -> EQUAL
         action   len -> 2, lps[3] = 2, i -> 4
         state    lps = [0,0,1,2], len = 2
         meaning  "abab" has proper prefix "ab" == its suffix "ab"

    i=4: i == m = 4 -> stop

    FINAL LPS = [0, 0, 1, 2]

      needle:   a   b   a   b
      lps:      0   0   1   2

    --------------------------------------------------------------------------
    PHASE 2: scan the haystack
    --------------------------------------------------------------------------

    Tracked state:
      i - position in haystack (never decreases)
      j - how many needle characters currently matched

    Initial state: i = 0, j = 0

    --------------------------------------------------------------------------
    i=0, j=0:  h[0]='a'  vs  nd[0]='a'   -> MATCH
      action    i -> 1, j -> 1     (j != 4, keep going)

    --------------------------------------------------------------------------
    i=1, j=1:  h[1]='b'  vs  nd[1]='b'   -> MATCH
      action    i -> 2, j -> 2

    --------------------------------------------------------------------------
    i=2, j=2:  h[2]='a'  vs  nd[2]='a'   -> MATCH
      action    i -> 3, j -> 3

    --------------------------------------------------------------------------
    i=3, j=3:  h[3]='x'  vs  nd[3]='b'   -> MISMATCH, and j > 0
      action    j = lps[j-1] = lps[2] = 1        (i STAYS at 3)
      meaning   we had matched "aba"; its longest prefix-that-is-also-a-suffix
                is "a", so the pattern slides so that its "a" lines up with
                the haystack's 'a' at index 2. No need to recheck index 2.

    --------------------------------------------------------------------------
    i=3, j=1:  h[3]='x'  vs  nd[1]='b'   -> MISMATCH, and j > 0
      action    j = lps[j-1] = lps[0] = 0        (i STAYS at 3)

    --------------------------------------------------------------------------
    i=3, j=0:  h[3]='x'  vs  nd[0]='a'   -> MISMATCH, and j == 0
      action    i -> 4      (nothing matched, so the text pointer moves)

    --------------------------------------------------------------------------
    i=4, j=0:  h[4]='a'  vs  nd[0]='a'   -> MATCH
      action    i -> 5, j -> 1

    --------------------------------------------------------------------------
    i=5, j=1:  h[5]='b'  vs  nd[1]='b'   -> MATCH
      action    i -> 6, j -> 2

    --------------------------------------------------------------------------
    i=6, j=2:  h[6]='a'  vs  nd[2]='a'   -> MATCH
      action    i -> 7, j -> 3

    --------------------------------------------------------------------------
    i=7, j=3:  h[7]='b'  vs  nd[3]='b'   -> MATCH
      action    i -> 8, j -> 4
      check     j == m = 4  ->  RETURN i - m = 8 - 4 = 4

    --------------------------------------------------------------------------
    RETURN 4

      a   b   a   x   a   b   a   b
                      ^---------------^
                      index 4, "abab"

    ==========================================================================
    Summary table
    ==========================================================================

    | i | j | h[i] | nd[j] | result   | action              | i,j after |
    |---|---|------|-------|----------|---------------------|-----------|
    | 0 | 0 |  a   |   a   | match    | i++, j++            |   1, 1    |
    | 1 | 1 |  b   |   b   | match    | i++, j++            |   2, 2    |
    | 2 | 2 |  a   |   a   | match    | i++, j++            |   3, 3    |
    | 3 | 3 |  x   |   b   | mismatch | j = lps[2] = 1      |   3, 1    |
    | 3 | 1 |  x   |   b   | mismatch | j = lps[0] = 0      |   3, 0    |
    | 3 | 0 |  x   |   a   | mismatch | i++ (j already 0)   |   4, 0    |
    | 4 | 0 |  a   |   a   | match    | i++, j++            |   5, 1    |
    | 5 | 1 |  b   |   b   | match    | i++, j++            |   6, 2    |
    | 6 | 2 |  a   |   a   | match    | i++, j++            |   7, 3    |
    | 7 | 3 |  b   |   b   | match    | i++, j++ -> j == m  | return 4  |

    Note that i NEVER decreases across the whole table - index 3 is examined
    three times in a row, but only because j is shrinking each time.

    ==========================================================================
    Notes
    ==========================================================================

    What lps[i] actually means:
      the length of the longest string that is BOTH a proper prefix and a
      suffix of needle[0..i]. For "abab": lps[3] = 2 because "ab" opens the
      string and also closes it. "Proper" excludes the whole substring
      itself, which is what stops lps[i] from trivially being i+1.

    Why falling back to lps[j-1] loses nothing:
      at a mismatch we have matched needle[0..j-1] against the text ending
      at i-1. Any occurrence of the needle that starts inside that matched
      stretch must begin with a prefix of the needle that is also a SUFFIX
      of what we just matched. The longest such candidate has length
      lps[j-1]; every longer alignment has already been disproved by the
      mismatch. So the jump skips only alignments that could not have
      matched, and the first occurrence is never missed.

    Why it is O(n + m) and not O(n * m):
      i never decreases, so it advances at most n times overall. j grows by
      exactly 1 on each match (at most n times total, since every j++ is
      paired with an i++) and strictly decreases on every fallback. A value
      cannot be decreased more often than it was increased, so the total
      number of fallbacks across the whole run is also at most n. Each
      iteration does either an advance or a fallback, so the loop body runs
      O(n) times. Building the LPS is the same argument on the needle,
      giving O(m).

    The naive alternative, for contrast:
      for (int i = 0; i + m <= n; ++i) { compare needle against
      haystack[i..i+m-1]; }  - simple and often fine in practice, but
      O(n * m) worst case. "aaaaaaaab" / "aaab" is the classic killer: each
      of the n-m+1 start positions matches m-1 characters before failing on
      the last one. It is included above as strStrNaive purely so main()
      can cross-check the KMP results.

    Edge cases handled:
      empty needle returns 0 (the conventional answer, matching how
      string::find behaves); a needle longer than the haystack returns -1
      immediately without building anything; a needle equal to the whole
      haystack returns 0 through the normal path.
*/
