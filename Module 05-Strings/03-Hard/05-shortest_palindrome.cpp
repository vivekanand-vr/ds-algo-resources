#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s, you may only add characters IN FRONT of it to turn it
//    into a palindrome. Return the shortest palindrome obtainable this way.
//
// Examples:
//   "aacecaaa" -> "aaacecaaa"  (prepend "a")
//   "abcd"     -> "dcbabcd"    (prepend "dcb")
//   "abab"     -> "babab"      (prepend "b")
//   "aabba"    -> "abbaabba"   (prepend "abb")
//   ""         -> ""           (already a palindrome)

/*
    Approach: KMP prefix function on s + '#' + reverse(s)

    - Only the FRONT may grow, so the tail of s must stay put. If we prepend
      k characters, the final string has length n + k and its last n
      characters are s itself. For it to be a palindrome, the first n
      characters must be reverse(s) - which forces the prepended block to be
      the reverse of the LAST k characters of s, and forces the remaining
      s[0..n-k-1] to be a palindrome on its own.
    - So minimising k is the same as MAXIMISING the length of a palindromic
      PREFIX of s. Call that length p. Then the answer is
          reverse(s.substr(p)) + s
      i.e. mirror everything after the palindromic prefix and glue it on top.
    - Finding the longest palindromic prefix is where KMP comes in. A prefix
      s[0..p-1] is a palindrome exactly when it equals its own reverse - and
      the reverse of that prefix is a SUFFIX of reverse(s). So a palindromic
      prefix of s is precisely a prefix of s that is also a suffix of
      reverse(s), and "longest prefix that is also a suffix" is the very
      thing the LPS array computes.
    - Build t = s + '#' + reverse(s) and take lps[t.size()-1]. The '#'
      separator is not optional: it is a sentinel that appears in neither
      half, so no matched prefix-suffix can ever straddle the two halves.
      Without it, s = "aaa" would give t = "aaaaaa" and lps = 5 > n, a
      nonsense answer. With the sentinel the match is capped at n by
      construction.
    - Brute force instead scans p from n downwards checking each prefix for
      palindromicity: O(n^2). For LeetCode 214's limit of n = 5*10^4 that is
      ~2.5*10^9 comparisons. The KMP build is O(n).

    Algorithm Steps
    ----------------
    1. If s is empty, return "" (nothing to mirror).
    2. Build rev = reverse(s) and t = s + '#' + rev; let m = t.length().
    3. Build the LPS array of t:
       a. lps[0] = 0, len = 0, i = 1.
       b. If t[i] == t[len]: lps[i] = ++len, then i++.
       c. Else if len > 0: len = lps[len-1]   (retry the same i).
       d. Else: lps[i] = 0, then i++.
    4. Let p = lps[m-1] - the length of the longest palindromic prefix of s.
    5. Return reverse(s.substr(p)) + s.

    Time Complexity: O(n) - t has length 2n+1 and the LPS build is linear in
                      that, since i advances at most m times and total
                      fallbacks are bounded by total increments of len.
    Space Complexity: O(n) - t and its lps array, both O(2n+1), plus the
                      returned string.
*/
string shortestPalindrome(string s) {
    int n = s.length();
    if (n == 0) return "";           // empty string is already a palindrome

    string rev(s.rbegin(), s.rend());     // reverse of s
    string t = s + '#' + rev;             // '#' is a sentinel absent from s
    int m = t.length();                   // m = 2n + 1

    vector<int> lps(m, 0);   // lps[i] = longest proper prefix of t[0..i] that is also its suffix

    int len = 0;   // length of the prefix-suffix currently matched
    int i = 1;     // lps[0] is always 0, so the scan starts at index 1

    while (i < m) {
        if (t[i] == t[len]) {
            lps[i] = ++len;      // the known prefix-suffix grows by one character
            i++;
        } else if (len > 0) {
            len = lps[len - 1];  // fall back to the next-best candidate; i does NOT move
        } else {
            lps[i] = 0;          // no prefix-suffix ends here at all
            i++;
        }
    }

    int p = lps[m - 1];   // prefix of s that is also a suffix of rev = longest palindromic prefix

    string tail = s.substr(p);                      // the part NOT covered by that palindrome
    return string(tail.rbegin(), tail.rend()) + s;  // mirror it onto the front
}

// Baseline for cross-checking only - the O(n^2) brute force this improves on.
// Walks the prefix length down from n and returns as soon as one is a
// palindrome. Correct, but quadratic in the worst case (e.g. "aaaa...aab").
string shortestPalindromeBrute(string s) {
    int n = s.length();

    for(int p=n; p>=0; --p){
        bool pal = true;
        for(int a=0, b=p-1; a<b; ++a, --b){
            if(s[a] != s[b]){ pal = false; break; }
        }
        if(pal){
            string tail = s.substr(p);
            return string(tail.rbegin(), tail.rend()) + s;
        }
    }

    return s;   // unreachable: p = 0 always succeeds
}

int main() {
    vector<string> tests = {
        "aacecaaa", "abcd", "abab", "aabba", "aaaa", "a", "", "abcba", "aba"
    };

    for (const string& s : tests) {
        string fast = shortestPalindrome(s);
        string slow = shortestPalindromeBrute(s);

        cout << "s = \"" << s << "\"" << endl;
        cout << "  shortest palindrome = \"" << fast << "\""
             << "   (brute-force baseline: \"" << slow << "\""
             << (fast == slow ? "\" - matches)" : "\" - MISMATCH)") << endl;
        cout << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "abab"      (n = 4, answer = "babab")
    ==========================================================================

    rev = "baba"
    t   = "abab" + '#' + "baba" = "abab#baba"      (m = 9)

      index:   0   1   2   3   4   5   6   7   8
       char:   a   b   a   b   #   b   a   b   a
               \--- s ----/   ^   \--- rev ---/
                           sentinel

    Tracked state:
      i    - position being filled in lps
      len  - length of the prefix-suffix matched so far; it also doubles as
             the index of the NEXT character to compare against, since the
             matched prefix is always t[0..len-1]
      lps  - the array being built

    Initial state: lps = [0,0,0,0,0,0,0,0,0], len = 0, i = 1

    --------------------------------------------------------------------------
    i = 1:  t[1]='b'  vs  t[len=0]='a'   -> DIFFER, and len == 0
      action    lps[1] = 0, i -> 2
      state     len = 0
      meaning   "ab" has no proper prefix that is also a suffix

    --------------------------------------------------------------------------
    i = 2:  t[2]='a'  vs  t[len=0]='a'   -> EQUAL
      action    len -> 1, lps[2] = 1, i -> 3
      state     lps = [0,0,1,0,0,0,0,0,0], len = 1
      meaning   "aba" opens and closes with "a"

    --------------------------------------------------------------------------
    i = 3:  t[3]='b'  vs  t[len=1]='b'   -> EQUAL
      action    len -> 2, lps[3] = 2, i -> 4
      state     lps = [0,0,1,2,0,0,0,0,0], len = 2
      meaning   "abab" opens and closes with "ab"

    --------------------------------------------------------------------------
    i = 4:  t[4]='#'  vs  t[len=2]='a'   -> DIFFER, and len > 0
      action    len = lps[len-1] = lps[1] = 0     (i STAYS at 4)
      state     len = 0
      meaning   the candidate "ab" cannot be extended by '#'. Fall back to
                the next-shortest candidate - here, the empty string.

    --------------------------------------------------------------------------
    i = 4:  t[4]='#'  vs  t[len=0]='a'   -> DIFFER, and len == 0
      action    lps[4] = 0, i -> 5
      state     lps = [0,0,1,2,0,0,0,0,0], len = 0
      meaning   the sentinel kills every match - which is exactly its job.
                From here on, any prefix-suffix must END inside rev and
                START inside s, so it can never exceed n = 4 characters.

    --------------------------------------------------------------------------
    i = 5:  t[5]='b'  vs  t[len=0]='a'   -> DIFFER, and len == 0
      action    lps[5] = 0, i -> 6
      state     len = 0

    --------------------------------------------------------------------------
    i = 6:  t[6]='a'  vs  t[len=0]='a'   -> EQUAL
      action    len -> 1, lps[6] = 1, i -> 7
      state     lps = [0,0,1,2,0,0,1,0,0], len = 1
      meaning   rev ends (so far) with "a" = s's first character, so "a" is
                a palindromic prefix of s

    --------------------------------------------------------------------------
    i = 7:  t[7]='b'  vs  t[len=1]='b'   -> EQUAL
      action    len -> 2, lps[7] = 2, i -> 8
      state     lps = [0,0,1,2,0,0,1,2,0], len = 2
      meaning   "ab" matches - but this is only a running state, not the
                answer; only the FINAL entry counts

    --------------------------------------------------------------------------
    i = 8:  t[8]='a'  vs  t[len=2]='a'   -> EQUAL
      action    len -> 3, lps[8] = 3, i -> 9
      state     lps = [0,0,1,2,0,0,1,2,3], len = 3
      meaning   s starts with "aba" and rev ends with "aba", so "aba" is a
                palindromic prefix of s - and it is the longest one

    --------------------------------------------------------------------------
    i = 9:  i == m = 9 -> loop ends

    FINAL LPS = [0, 0, 1, 2, 0, 0, 1, 2, 3]

      char:   a   b   a   b   #   b   a   b   a
      lps:    0   0   1   2   0   0   1   2   3

    p = lps[8] = 3   ->  longest palindromic prefix of s is s[0..2] = "aba"

      tail    = s.substr(3)      = "b"
      mirror  = reverse("b")     = "b"
      RETURN    "b" + "abab"     = "babab"

      verify:   b a b a b   reads the same backwards
                only 1 character was prepended, and 0 is impossible since
                "abab" itself is not a palindrome

        [b] a b a b
         ^   \-----/
      added     s kept intact at the back

    ==========================================================================
    Summary table
    ==========================================================================

    | i | t[i] | len before | t[len] | result   | action           | lps[i] | len after |
    |---|------|------------|--------|----------|------------------|--------|-----------|
    | 1 |  b   |     0      |   a    | differ   | lps[1]=0, i++    |   0    |     0     |
    | 2 |  a   |     0      |   a    | equal    | lps[2]=++len, i++|   1    |     1     |
    | 3 |  b   |     1      |   b    | equal    | lps[3]=++len, i++|   2    |     2     |
    | 4 |  #   |     2      |   a    | differ   | len=lps[1]=0     |   -    |     0     |
    | 4 |  #   |     0      |   a    | differ   | lps[4]=0, i++    |   0    |     0     |
    | 5 |  b   |     0      |   a    | differ   | lps[5]=0, i++    |   0    |     0     |
    | 6 |  a   |     0      |   a    | equal    | lps[6]=++len, i++|   1    |     1     |
    | 7 |  b   |     1      |   b    | equal    | lps[7]=++len, i++|   2    |     2     |
    | 8 |  a   |     2      |   a    | equal    | lps[8]=++len, i++|   3    |     3     |

    Row 4 appears twice: that is the fallback, and it is the only row where
    i does not advance.

    ==========================================================================
    Notes
    ==========================================================================

    Why "longest palindromic prefix" is the right subproblem:
      the answer always looks like X + s for some prepended block X of
      length k. Reading the result backwards, its first n characters must
      spell reverse(s), so X is forced to be the reverse of the last k
      characters of s, and the leftover middle s[0..n-k-1] must itself be a
      palindrome. Every palindromic prefix therefore yields a valid answer
      of length 2n - p, and that is minimised by the LARGEST p. There is no
      cleverer construction to miss - the structure is forced.

    Why the '#' sentinel is mandatory:
      the LPS of t must not report a match longer than n, or s.substr(p)
      would be empty (or p would index past the string) and the result
      would silently be wrong. Take s = "aaa": without a separator,
      t = "aaaaaa" and lps[5] = 5, claiming a palindromic prefix of length
      5 in a string of length 3. Inserting a character that occurs in
      neither half makes any prefix-suffix spanning the boundary
      impossible, capping the value at n. Any character outside the input
      alphabet works; '#' is the convention. If the real input can contain
      '#', pick something else (a '\1' byte, say).

    Why the fallback is lps[len-1] and not len-1:
      when t[i] != t[len], the candidate prefix-suffix of length len has
      failed. The next candidate must be a prefix of t that is also a
      suffix of the len characters already matched - i.e. exactly
      lps[len-1]. Trying len-1 would test a prefix not known to be a
      suffix of what precedes i, so it could not be a valid alignment.
      Every length strictly between lps[len-1] and len is skipped, which
      is safe: any such length would itself have been a prefix-suffix of
      the matched block, contradicting lps[len-1] being the longest.

    Why i must not advance on a fallback:
      t[i] has not been placed yet. The fallback only changes WHICH prefix
      we try to extend with it; t[i] still has to be compared against the
      new, shorter candidate. Advancing here would leave lps[i] at 0 and
      corrupt every later entry - including the final one, which is the
      whole answer.

    Why the O(n) bound holds despite the inner fallback:
      len grows by exactly 1 on each "equal" branch, and each of those also
      advances i - so len is incremented at most m times across the run.
      Every fallback strictly decreases len, and len never drops below 0.
      A quantity cannot be decreased more times than it was increased, so
      there are at most m fallbacks. Each iteration does either an advance
      or a fallback: at most 2m steps, and m = 2n + 1, so O(n).

    Edge cases:
      - s already a palindrome: p = n, tail is empty, and the function
        returns s unchanged. E.g. "aba" -> t = "aba#aba", lps last = 3.
      - s with no palindromic prefix beyond one character: p = 1 (a single
        character is always a palindrome), so n-1 characters get prepended.
        E.g. "abcd" -> "dcb" + "abcd" = "dcbabcd", the worst case.
      - empty s: guarded at the top. Without that guard m would be 1 and
        lps[0] = 0 would still give "", but the guard documents the intent
        and keeps the reasoning about t's two halves honest.

    A common wrong turn - reading the wrong lps entry:
      the running maximum of the lps array is NOT the answer. Only the
      FINAL entry lps[m-1] describes a match that ends at the very last
      character of rev, which is what makes the matched block a genuine
      PREFIX of s aligned with a SUFFIX of rev. Earlier entries (like
      lps[7] = 2 above) match a suffix of a truncated rev, which
      corresponds to nothing in the original problem.

    On the brute force kept above:
      it is correct and far easier to read - scan prefix lengths from
      longest to shortest and stop at the first palindrome - but a string
      like "aaaa...aab" forces it to test ~n prefixes of ~n/2 comparisons
      each, giving O(n^2). It is retained only so main() can cross-check
      the KMP result on every test case.
*/
