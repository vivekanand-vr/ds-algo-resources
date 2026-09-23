#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s, return the number of DIFFERENT non-empty palindromic
//    subsequences in s, modulo 10^9 + 7.
//    A subsequence is obtained by deleting zero or more characters. A sequence
//    is palindromic if it equals its own reverse. Two sequences are different
//    if they differ at some position - so identical strings formed from
//    different index sets count ONCE.
//
// Examples:
//   "bccb" -> 6
//        the 6 distinct palindromic subsequences are
//        'b', 'c', 'bb', 'cc', 'bcb', 'bccb'
//        note 'bcb' is counted once even though it can be picked two ways
//        (indices 0,1,3 and 0,2,3)
//
//   "abcdabcdabcdabcdabcdabcdabcdabcddcbadcbadcbadcbadcbadcbadcbadcba" -> 104860361
//        there are 3104860382 distinct ones; the answer is that modulo 10^9+7

/*
    Approach: Interval DP over substrings, with same-character de-duplication

    - The trap is the word DIFFERENT. Counting palindromic subsequences by
      index set is a simple two-line DP; counting them by STRING VALUE is not,
      because the same string can be assembled from many different index sets
      and must be collapsed to one. Every complication below exists only to
      perform that collapse.
    - State: dp[i][j] = the number of distinct non-empty palindromic
      subsequences inside the substring s[i..j]. The answer is dp[0][n-1].
      Intervals are the right state because a palindrome is defined by its two
      ends: whatever we decide about s[i] and s[j], the rest of the decision
      lives entirely in the interval strictly between them.

    - Case A: s[i] != s[j].
      No palindrome can use BOTH ends, since a palindrome's first and last
      characters must agree. So every palindrome here lives in s[i+1..j] or in
      s[i..j-1]. By inclusion-exclusion:
          dp[i][j] = dp[i+1][j] + dp[i][j-1] - dp[i+1][j-1]
      The subtraction removes the ones counted twice - exactly those that use
      neither end, i.e. those inside s[i+1..j-1].

    - Case B: s[i] == s[j] == c.
      Now split by whether a palindrome uses the outer pair:
        * Not using both ends: dp[i+1][j] + dp[i][j-1] - dp[i+1][j-1], as above.
        * Using both ends: wrap c around every palindrome of the inside, plus
          the two new ones "c" and "cc", giving dp[i+1][j-1] + 2.
      Adding these gives dp[i+1][j] + dp[i][j-1] - dp[i+1][j-1] +
      dp[i+1][j-1] + 2 = dp[i+1][j] + dp[i][j-1] + 2... but ONLY when no other
      c sits between i and j. If one does, "c" and "cc" were already counted
      by the inner terms and would be double-counted. Letting
          lo = first index in (i, j) with s[lo] == c
          hi = last  index in (i, j) with s[hi] == c
      the three sub-cases are:
          no inner c   (lo > hi) : dp[i][j] = 2*dp[i+1][j-1] + 2
          one inner c  (lo == hi): dp[i][j] = 2*dp[i+1][j-1] + 1
          two or more  (lo < hi) : dp[i][j] = 2*dp[i+1][j-1] - dp[lo+1][hi-1]
      Read the middle row as: "c" alone is already counted inside, so only
      "cc" is genuinely new. The last row: every palindrome already counted in
      s[lo+1..hi-1] gets wrapped by BOTH the outer c's and the inner c's,
      producing the identical string twice, so one copy is removed.
      The factor 2*dp[i+1][j-1] is "the inside on its own" plus "the inside
      wrapped in c on both sides".

    - Why the naive attempts fail:
        * Enumerating all 2^n subsequences into a hash set is exact but
          exponential; unusable past n ~ 25 (the constraint is n <= 1000).
        * The index-set DP dp[i][j] = dp[i+1][j] + dp[i][j-1] - dp[i+1][j-1]
          + (s[i]==s[j] ? dp[i+1][j-1]+1 : 0) counts "bcb" twice in "bccb"
          and returns 7 instead of 6.

    - Modular arithmetic: every subtraction can go negative under mod, so each
      one is followed by "+ MOD" before the final "% MOD". Skipping that is
      the single most common way this solution silently breaks on the large
      example above.

    Algorithm Steps
    ----------------
    1. Let n = s.length(); if n == 0 return 0. Allocate dp[n][n] of zeros.
    2. Base case: dp[i][i] = 1 for all i - a single character is one
       palindrome.
    3. Fill by increasing interval length L = 2..n, and for each i let
       j = i + L - 1:
       a. If s[i] != s[j]:
            dp[i][j] = dp[i+1][j] + dp[i][j-1] - dp[i+1][j-1]
       b. Else, scan lo forward from i+1 and hi backward from j-1 for the
          nearest occurrences of s[i]:
            lo > hi  -> dp[i][j] = 2*dp[i+1][j-1] + 2
            lo == hi -> dp[i][j] = 2*dp[i+1][j-1] + 1
            lo < hi  -> dp[i][j] = 2*dp[i+1][j-1] - dp[lo+1][hi-1]
       c. Normalise into [0, MOD) with ((x % MOD) + MOD) % MOD.
    4. Return dp[0][n-1].

    Time Complexity: O(n^3) worst case - O(n^2) intervals, and the lo/hi scan
                      inside case B costs O(n) per interval. It is O(n^2) on
                      strings where equal-end intervals are rare; the scan can
                      be removed entirely by precomputing nxt[i][c] and
                      prv[j][c] tables in O(26n), making it O(n^2) flat.
    Space Complexity: O(n^2) - the dp table. Every cell is read by a longer
                      interval, so no row can be dropped the way a 1D rolling
                      array would.
*/
int countPalindromicSubsequences(string s) {
    const int MOD = 1e9 + 7;
    int n = s.length();
    if (n == 0) return 0;

    // dp[i][j] = number of DISTINCT non-empty palindromic subsequences of s[i..j]
    vector<vector<long long>> dp(n, vector<long long>(n, 0));

    for (int i = 0; i < n; i++) dp[i][i] = 1;   // a lone character is one palindrome

    // intervals must grow outward: dp[i][j] reads strictly shorter intervals
    for (int L = 2; L <= n; L++) {
        for (int i = 0; i + L - 1 < n; i++) {
            int j = i + L - 1;

            if (s[i] != s[j]) {
                // no palindrome can use both ends; inclusion-exclusion over the two halves
                dp[i][j] = dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1];
            } else {
                // nearest occurrences of the same character strictly inside (i, j)
                int lo = i + 1, hi = j - 1;
                while (lo <= hi && s[lo] != s[i]) lo++;
                while (lo <= hi && s[hi] != s[i]) hi--;

                if (lo > hi) {
                    // no inner copy: "c" and "cc" are both brand new
                    dp[i][j] = 2 * dp[i + 1][j - 1] + 2;
                } else if (lo == hi) {
                    // exactly one inner copy already contributed "c", so only "cc" is new
                    dp[i][j] = 2 * dp[i + 1][j - 1] + 1;
                } else {
                    // two or more: everything between them gets wrapped twice - remove one copy
                    dp[i][j] = 2 * dp[i + 1][j - 1] - dp[lo + 1][hi - 1];
                }
            }

            dp[i][j] = ((dp[i][j] % MOD) + MOD) % MOD;   // the subtractions can go negative
        }
    }

    return (int)dp[0][n - 1];
}

// Baseline for cross-checking only - exponential, so keep n small (<= 20).
// Enumerates every subsequence via a bitmask and drops duplicates in a set,
// which is the definition of the problem transcribed directly into code.
int countPalindromicSubsequencesBrute(string s) {
    int n = s.length();
    unordered_set<string> seen;

    for(int mask=1; mask < (1<<n); ++mask){
        string sub;
        for(int b=0; b<n; ++b){
            if(mask & (1<<b)) sub += s[b];
        }
        string rev(sub.rbegin(), sub.rend());
        if(sub == rev) seen.insert(sub);
    }

    return (int)(seen.size() % 1000000007);
}

int main() {
    vector<string> tests = {
        "bccb", "aaa", "abc", "abcb", "bbcbb", "a", ""
    };

    for (const string& s : tests) {
        int fast = countPalindromicSubsequences(s);
        int slow = countPalindromicSubsequencesBrute(s);

        cout << "s = \"" << s << "\"" << endl;
        cout << "  distinct palindromic subsequences = " << fast
             << "   (brute-force baseline: " << slow
             << (fast == slow ? " - matches)" : " - MISMATCH)") << endl;
        cout << endl;
    }

    // The large example from the statement - far past what the brute force can do.
    string big = "abcdabcdabcdabcdabcdabcdabcdabcddcbadcbadcbadcbadcbadcbadcbadcba";
    cout << "s = <the 64-character example>" << endl;
    cout << "  distinct palindromic subsequences mod 1e9+7 = "
         << countPalindromicSubsequences(big) << "   (expected 104860361)" << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "bccb"      (n = 4, answer = 6)
    ==========================================================================

      index:   0   1   2   3
       char:   b   c   c   b

    Tracked state:
      dp[i][j] - number of DISTINCT non-empty palindromic subsequences of
                 s[i..j]. Only the upper triangle (i <= j) is ever used; the
                 cells below the diagonal stay 0 and stand in for the empty
                 interval, which is exactly what dp[i+1][j-1] means when
                 i+1 > j-1.
      L        - the interval length currently being filled
      lo, hi   - nearest occurrences of s[i] strictly inside (i, j)

    Initial state (L = 1, the base case): dp[i][i] = 1 for every i

        dp |  j=0   j=1   j=2   j=3
      -----+------------------------
       i=0 |   1     .     .     .
       i=1 |   -     1     .     .
       i=2 |   -     -     1     .
       i=3 |   -     -     -     1

      ("." = not yet filled, "-" = never used / reads as the empty interval)

    --------------------------------------------------------------------------
    L = 2
    --------------------------------------------------------------------------
    (i=0, j=1): s[0]='b', s[1]='c'  -> DIFFER, case A
      compute   dp[1][1] + dp[0][0] - dp[1][0] = 1 + 1 - 0 = 2
      note      dp[1][0] is the empty interval (i+1 > j-1), hence 0
      dp[0][1] = 2      the set is { "b", "c" }

    (i=1, j=2): s[1]='c', s[2]='c'  -> EQUAL, case B
      scan      lo starts at 2, hi starts at 1  ->  lo > hi immediately,
                so the interval strictly inside is empty: no inner 'c'
      compute   2*dp[2][1] + 2 = 2*0 + 2 = 2
      dp[1][2] = 2      the set is { "c", "cc" }
                        - "c" and "cc" are the two brand-new palindromes

    (i=2, j=3): s[2]='c', s[3]='b'  -> DIFFER, case A
      compute   dp[3][3] + dp[2][2] - dp[3][2] = 1 + 1 - 0 = 2
      dp[2][3] = 2      the set is { "c", "b" }

        dp |  j=0   j=1   j=2   j=3
      -----+------------------------
       i=0 |   1     2     .     .
       i=1 |   -     1     2     .
       i=2 |   -     -     1     2
       i=3 |   -     -     -     1

    --------------------------------------------------------------------------
    L = 3
    --------------------------------------------------------------------------
    (i=0, j=2): s[0]='b', s[2]='c'  -> DIFFER, case A
      compute   dp[1][2] + dp[0][1] - dp[1][1] = 2 + 2 - 1 = 3
      why -1    "c" is counted in both dp[1][2] ("cc" region) and dp[0][1];
                it is the single palindrome living in s[1..1], which is
                exactly what dp[1][1] measures
      dp[0][2] = 3      the set is { "b", "c", "cc" }

    (i=1, j=3): s[1]='c', s[3]='b'  -> DIFFER, case A
      compute   dp[2][3] + dp[1][2] - dp[2][2] = 2 + 2 - 1 = 3
      dp[1][3] = 3      the set is { "c", "cc", "b" }

        dp |  j=0   j=1   j=2   j=3
      -----+------------------------
       i=0 |   1     2     3     .
       i=1 |   -     1     2     3
       i=2 |   -     -     1     2
       i=3 |   -     -     -     1

    --------------------------------------------------------------------------
    L = 4
    --------------------------------------------------------------------------
    (i=0, j=3): s[0]='b', s[3]='b'  -> EQUAL, case B     <-- the interesting one
      scan      lo = 1: s[1]='c' != 'b', lo -> 2: s[2]='c' != 'b', lo -> 3,
                and now lo > hi stops the first loop (hi is still 2)
                the second loop does not run, since lo <= hi is already false
      result    lo = 3 > hi = 2  ->  NO inner 'b'
      compute   2*dp[1][2] + 2 = 2*2 + 2 = 6
      reading   dp[1][2] = 2 is { "c", "cc" }. The factor 2 splits as:
                  - those 2 on their own                    -> "c", "cc"
                  - those 2 wrapped in 'b' on both sides    -> "bcb", "bccb"
                and the +2 adds the two palindromes made from the outer pair
                alone                                       -> "b", "bb"
                total { "c", "cc", "bcb", "bccb", "b", "bb" } = 6 distinct
      dp[0][3] = 6

    FINAL TABLE

        dp |  j=0   j=1   j=2   j=3
      -----+------------------------
       i=0 |   1     2     3     6
       i=1 |   -     1     2     3
       i=2 |   -     -     1     2
       i=3 |   -     -     -     1

    RETURN dp[0][3] = 6

      verify against the statement: 'b', 'c', 'bb', 'cc', 'bcb', 'bccb' - 6,
      with 'bcb' counted once although indices (0,1,3) and (0,2,3) both
      spell it. The DP never enumerated those index sets, which is precisely
      why it did not double-count them.

    ==========================================================================
    Summary table
    ==========================================================================

    | L | i | j | s[i],s[j] | case                | formula                      | dp[i][j] |
    |---|---|---|-----------|---------------------|------------------------------|----------|
    | 1 | 0 | 0 | b , b     | base                | 1                            |    1     |
    | 1 | 1 | 1 | c , c     | base                | 1                            |    1     |
    | 1 | 2 | 2 | c , c     | base                | 1                            |    1     |
    | 1 | 3 | 3 | b , b     | base                | 1                            |    1     |
    | 2 | 0 | 1 | b , c     | A: differ           | 1 + 1 - 0                    |    2     |
    | 2 | 1 | 2 | c , c     | B: no inner c       | 2*0 + 2                      |    2     |
    | 2 | 2 | 3 | c , b     | A: differ           | 1 + 1 - 0                    |    2     |
    | 3 | 0 | 2 | b , c     | A: differ           | 2 + 2 - 1                    |    3     |
    | 3 | 1 | 3 | c , b     | A: differ           | 2 + 2 - 1                    |    3     |
    | 4 | 0 | 3 | b , b     | B: no inner b       | 2*2 + 2                      |    6     |

    ==========================================================================
    Notes
    ==========================================================================

    Why intervals, and why they must be filled by increasing length:
      dp[i][j] is written in terms of dp[i+1][j], dp[i][j-1], dp[i+1][j-1]
      and dp[lo+1][hi-1] - every one of them a STRICTLY shorter interval. So
      a plain i-then-j loop order would read cells that are still zero.
      Looping on L guarantees each dependency is already final. (Equivalently,
      loop i from n-1 down to 0 and j from i+1 up to n-1.)

    Why dp[i+1][j-1] is subtracted in case A:
      dp[i+1][j] counts everything avoiding index i, dp[i][j-1] everything
      avoiding index j. A palindrome avoiding BOTH is in both totals, and
      those are exactly the palindromes of s[i+1..j-1]. Inclusion-exclusion
      removes the overlap once. Nothing is lost by ignoring palindromes that
      use both ends, because s[i] != s[j] makes them impossible.

    Where the three sub-cases of B actually come from:
      write the answer as (palindromes not using both ends) + (palindromes
      using both ends). The second group is "wrap c around any palindrome of
      the inside", plus "c" and "cc" themselves - that is dp[i+1][j-1] + 2.
      The first group is dp[i+1][j] + dp[i][j-1] - dp[i+1][j-1]. Adding them
      and then removing whatever the inner c's have ALREADY contributed
      collapses to the three compact forms used in the code. The sub-cases
      differ only in how much of that "already contributed" there is:
        zero inner c  -> nothing was pre-counted, add both "c" and "cc"
        one inner c   -> "c" was pre-counted, add only "cc"
        many inner c  -> "c" and "cc" pre-counted, AND every palindrome
                         strictly between the first and last inner c got
                         wrapped by the inner pair as well as the outer
                         pair, spelling identical strings - subtract
                         dp[lo+1][hi-1] to keep one copy of each.

    Why the modulo needs the "+ MOD":
      case A and the lo < hi branch both subtract. After a % MOD the left
      operand can be smaller than the right, so the difference is negative,
      and C++ keeps the sign of the dividend on %. A negative dp cell then
      poisons every longer interval that reads it. ((x % MOD) + MOD) % MOD
      normalises back into [0, MOD). On "bccb" no subtraction ever goes
      negative - the bug only shows up on large inputs like the 64-character
      example, which is why it is easy to ship broken.

    Why longs:
      2*dp[i+1][j-1] + 2 with dp just under 10^9 exceeds a signed 32-bit int.
      The table is declared long long so the intermediate survives until the
      reduction on the next line.

    Edge cases:
      - empty s: guarded at the top, returns 0. Without the guard,
        dp[0][n-1] would index dp[0][-1].
      - all identical characters, e.g. "aaa": the answer is n (namely "a",
        "aa", "aaa"), not 2^n - 1, because distinct STRINGS are counted.
        This is the case that most cleanly separates a correct solution from
        the index-counting one.
      - n = 1: the L loop never runs and dp[0][0] = 1 is returned directly.

    On the brute force kept above:
      it is the problem statement transcribed literally - build every one of
      the 2^n - 1 subsequences, keep the palindromic ones in a set, report
      the set size. Correct and impossible to get subtly wrong, but it is
      O(2^n * n) time and stores up to 2^n strings, so it only exists to
      cross-check main()'s small cases against the DP. The 64-character
      example is run through the DP alone.
*/
