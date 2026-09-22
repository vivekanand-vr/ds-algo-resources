#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: A "happy prefix" is a non-empty prefix of s that is also a suffix of s,
//    excluding s itself. Return the LONGEST happy prefix of s, or "" if none.
//
// Examples:
//   "level"        -> "l"      ("l" opens and closes the string)
//   "ababab"       -> "abab"   ("ababab" = ab|abab and abab|ab)
//   "leetcodeleet" -> "leet"
//   "abcd"         -> ""       (no prefix is also a suffix)

/*
    Approach: KMP prefix function (LPS array) - the answer is lps[n-1]

    - "Longest proper prefix that is also a suffix" is not merely SOLVED by
      the LPS array - it IS the definition of the LPS array's last entry.
      So the whole problem collapses to: build the LPS, read lps[n-1], and
      slice that many characters off the front. No searching required.
    - lps[i] = the length of the longest PROPER prefix of s[0..i] that is
      also a suffix of s[0..i]. "Proper" means it cannot be the whole
      substring s[0..i] itself - otherwise the answer would trivially be
      i+1 every time, which is exactly the "excluding s itself" clause of
      this problem.
    - The construction walks i forward while tracking len, the length of
      the prefix-suffix matched so far:
        * If s[i] == s[len], the known prefix-suffix extends by one
          character, so lps[i] = ++len and i moves on.
        * If they differ and len > 0, fall back to len = lps[len-1] and try
          again WITHOUT advancing i. This is the crux (see the Notes): that
          fallback jumps to the next-longest candidate prefix-suffix, and
          every candidate it skips has already been proven impossible.
        * If they differ and len == 0, there is no candidate at all, so
          lps[i] = 0 and i moves on.
    - The naive alternative compares every prefix against every suffix with
      substr, which is O(n^2) time AND allocates two fresh strings per
      iteration. For LeetCode 1392's limit of n = 10^5 that is ~10^10
      character comparisons - far too slow. The LPS build is O(n).

    Algorithm Steps
    ----------------
    1. Let n = s.length() and allocate lps[0..n-1], all zero. lps[0] is
       always 0: a single character has no proper prefix.
    2. Set len = 0 and i = 1.
    3. While i < n:
       a. If s[i] == s[len]: lps[i] = ++len, then i++.
       b. Else if len > 0: len = lps[len-1]   (retry the same i).
       c. Else: lps[i] = 0, then i++.
    4. Return s.substr(0, lps[n-1]).

    Time Complexity: O(n) - i advances at most n times, and len strictly
                      decreases on every fallback while only ever growing by
                      1 per advance, so total fallbacks are bounded by total
                      increments. See the amortised note at the bottom.
    Space Complexity: O(n) - the lps array (plus the returned substring).
*/
string longestPrefix(string s) {
    int n = s.length();
    vector<int> lps(n, 0);   // lps[i] = longest proper prefix of s[0..i] that is also its suffix

    int len = 0;   // length of the prefix-suffix currently matched
    int i = 1;     // lps[0] is always 0, so the scan starts at index 1

    while (i < n) {
        if (s[i] == s[len]) {
            lps[i] = ++len;      // the known prefix-suffix grows by one character
            i++;
        } else if (len > 0) {
            len = lps[len - 1];  // fall back to the next-best candidate; i does NOT move
        } else {
            lps[i] = 0;          // no prefix-suffix ends here at all
            i++;
        }
    }

    return s.substr(0, lps[n - 1]);   // the last entry is exactly the answer
}

// Baseline for cross-checking only - the O(n^2) brute force this improves on.
// Compares every prefix against the equal-length suffix and keeps the longest
// match. Correct, but quadratic and allocation-heavy.
string longestPrefixBrute(string s) {
    int n = s.length();
    int mx = 0;

    for(int i=1; i<n; ++i){
        string pref = s.substr(0, i);
        string suff = s.substr(n-i, i);
        if(pref == suff){
            mx = pref.length();
        }
    }

    return s.substr(0, mx);
}

int main() {
    vector<string> tests = {
        "level", "ababab", "leetcodeleet", "aabaaba", "aaaa", "abcd", "a"
    };

    for (const string& s : tests) {
        string fast = longestPrefix(s);
        string slow = longestPrefixBrute(s);

        cout << "s = \"" << s << "\"" << endl;
        cout << "  longest happy prefix = \"" << fast << "\""
             << "   (brute-force baseline: \"" << slow << "\""
             << (fast == slow ? "\" - matches)" : "\" - MISMATCH)") << endl;
        cout << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "aabaaba"      (n = 7, answer = "aaba")
    ==========================================================================

      index:   0   1   2   3   4   5   6
       char:   a   a   b   a   a   b   a

    Tracked state:
      i    - position being filled in lps
      len  - length of the prefix-suffix matched so far; it also doubles as
             the index of the NEXT pattern character to compare against,
             since the matched prefix is always s[0..len-1]
      lps  - the array being built

    Initial state: lps = [0,0,0,0,0,0,0], len = 0, i = 1

    --------------------------------------------------------------------------
    i = 1:  s[1]='a'  vs  s[len=0]='a'   -> EQUAL
      action    len -> 1, lps[1] = 1, i -> 2
      state     lps = [0,1,0,0,0,0,0], len = 1
      meaning   "aa" has proper prefix "a" equal to its suffix "a"

    --------------------------------------------------------------------------
    i = 2:  s[2]='b'  vs  s[len=1]='a'   -> DIFFER, and len > 0
      action    len = lps[len-1] = lps[0] = 0     (i STAYS at 2)
      state     lps = [0,1,0,0,0,0,0], len = 0
      meaning   "a" was the candidate, but it cannot be extended by 'b'.
                Fall back to the next-shortest candidate, which is the empty
                string. Note i did not move - index 2 gets retried.

    --------------------------------------------------------------------------
    i = 2:  s[2]='b'  vs  s[len=0]='a'   -> DIFFER, and len == 0
      action    lps[2] = 0, i -> 3
      state     lps = [0,1,0,0,0,0,0], len = 0
      meaning   "aab" has no proper prefix that is also a suffix

    --------------------------------------------------------------------------
    i = 3:  s[3]='a'  vs  s[len=0]='a'   -> EQUAL
      action    len -> 1, lps[3] = 1, i -> 4
      state     lps = [0,1,0,1,0,0,0], len = 1
      meaning   "aaba" ends with "a", which is also its opening character

    --------------------------------------------------------------------------
    i = 4:  s[4]='a'  vs  s[len=1]='a'   -> EQUAL
      action    len -> 2, lps[4] = 2, i -> 5
      state     lps = [0,1,0,1,2,0,0], len = 2
      meaning   "aabaa" ends with "aa" = its first two characters

    --------------------------------------------------------------------------
    i = 5:  s[5]='b'  vs  s[len=2]='b'   -> EQUAL
      action    len -> 3, lps[5] = 3, i -> 6
      state     lps = [0,1,0,1,2,3,0], len = 3
      meaning   "aabaab" ends with "aab" = its first three characters

    --------------------------------------------------------------------------
    i = 6:  s[6]='a'  vs  s[len=3]='a'   -> EQUAL
      action    len -> 4, lps[6] = 4, i -> 7
      state     lps = [0,1,0,1,2,3,4], len = 4
      meaning   "aabaaba" ends with "aaba" = its first four characters

    --------------------------------------------------------------------------
    i = 7:  i == n = 7 -> loop ends

    FINAL LPS = [0, 1, 0, 1, 2, 3, 4]

      char:   a   a   b   a   a   b   a
      lps:    0   1   0   1   2   3   4

    RETURN s.substr(0, lps[6]) = s.substr(0, 4) = "aaba"

      verify:   prefix s[0..3] = a a b a
                suffix s[3..6] = a a b a      identical, and length 4 < 7 ✓

      a   a   b   a   a   b   a
      |-----------|
        prefix "aaba"
                  |-----------|
                    suffix "aaba"
      (they overlap at index 3 - that is allowed, a happy prefix only has
       to be shorter than the whole string, not disjoint from the suffix)

    ==========================================================================
    Summary table
    ==========================================================================

    | i | s[i] | len before | s[len] | result   | action           | lps[i] | len after |
    |---|------|------------|--------|----------|------------------|--------|-----------|
    | 1 |  a   |     0      |   a    | equal    | lps[1]=++len, i++|   1    |     1     |
    | 2 |  b   |     1      |   a    | differ   | len=lps[0]=0     |   -    |     0     |
    | 2 |  b   |     0      |   a    | differ   | lps[2]=0, i++    |   0    |     0     |
    | 3 |  a   |     0      |   a    | equal    | lps[3]=++len, i++|   1    |     1     |
    | 4 |  a   |     1      |   a    | equal    | lps[4]=++len, i++|   2    |     2     |
    | 5 |  b   |     2      |   b    | equal    | lps[5]=++len, i++|   3    |     3     |
    | 6 |  a   |     3      |   a    | equal    | lps[6]=++len, i++|   4    |     4     |

    Row 2 appears twice: that is the fallback, and it is the only row where
    i does not advance.

    ==========================================================================
    Notes
    ==========================================================================

    Why the answer is just lps[n-1]:
      lps[n-1] is by definition the length of the longest proper prefix of
      s[0..n-1] - that is, of the whole string - which is also a suffix of
      it. "Proper" is precisely the problem's "excluding s itself" clause.
      So no search is needed; the array's final entry already answers the
      question, and the first lps[n-1] characters are the happy prefix.

    Why the fallback is lps[len-1] and not len-1:
      when s[i] != s[len], the candidate prefix-suffix of length len has
      failed. The next candidate must be a prefix of s that is also a
      suffix of the len characters we had already matched - i.e. exactly
      lps[len-1]. Simply trying len-1 would test a prefix that is NOT
      known to be a suffix of what precedes i, so it could not be a valid
      alignment anyway. Every length strictly between lps[len-1] and len is
      skipped, and that is safe: any such length would itself have been a
      prefix-suffix of the matched block, contradicting lps[len-1] being
      the longest one.

    Why i must not advance on a fallback:
      the character s[i] has not yet been placed. The fallback only changes
      WHICH prefix we are trying to extend with it; s[i] still needs to be
      compared against the new, shorter candidate. Advancing i here would
      silently leave lps[i] at 0 and corrupt every later entry - and it
      would also break the complexity argument below, since i would then
      absorb work that the len counter is supposed to pay for.

    Why the O(n) bound holds despite the inner fallback:
      len grows by exactly 1 on each of the "equal" branches, and each of
      those also advances i - so len is incremented at most n times in
      total across the entire run. Every fallback strictly decreases len,
      and len never drops below 0. A quantity cannot be decreased more
      times than it was increased, so there are at most n fallbacks
      overall. Each loop iteration performs either an advance or a
      fallback, giving at most 2n iterations - O(n).

    A bonus the LPS gives for free - the smallest period:
      period = n - lps[n-1] is the length of the smallest repeating block
      of s, and s is a whole number of copies of that block exactly when
      n % period == 0. Here n = 7 and lps[6] = 4, so period = 3 and
      7 % 3 = 1 != 0 - correct, since "aabaaba" is "aab" + "aab" + "a",
      not a clean repetition. By contrast "ababab" has n = 6, lps[5] = 4,
      period 2, and 6 % 2 == 0, so it is "ab" repeated three times.

    Degenerate case - empty s:
      if s were empty then n = 0 and the final lps[n-1] would index lps[-1],
      which is out of bounds. LeetCode 1392 guarantees 1 <= s.length, so
      this cannot arise there, but add an `if (n == 0) return "";` guard
      before the return if you ever reuse this function somewhere the input
      is not constrained. (The brute force below happens to survive an
      empty string, returning "".)

    On the brute force kept above:
      it is correct and much easier to read, but it builds two fresh
      strings per iteration and compares them in full, giving O(n^2) time
      and O(n) transient allocations per step. It is retained only so that
      main() can cross-check the KMP result on every test case.
*/
