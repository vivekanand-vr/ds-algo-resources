#include <bits/stdc++.h>
using namespace std;

// Q: The "beauty" of a string is the difference between the frequency of
//    its most frequent character and that of its least frequent one
//    (counting only characters that actually appear). Given a string s
//    of lowercase letters, return the sum of the beauty of all of its
//    substrings.
//
// Example:
// s = "aabcb" -> 5
//    Only these substrings have non-zero beauty, 1 each:
//    "aab" (a:2, b:1), "aabc", "aabcb", "abcb", "bcb"
// s = "aabcbaa" -> 17

/*
    Approach: Fix the left end, extend right, carry the frequency table along

    - There are O(n^2) substrings, and the beauty of each depends on the
      character frequencies inside it. Rebuilding a frequency table from
      scratch for every substring would cost O(n) per substring and
      O(n^3) overall.
    - The saving comes from the fact that substrings sharing a left end
      form a growing chain: s[i..i], s[i..i+1], s[i..i+2], ... Each step
      adds exactly ONE character, so the frequency table for the next
      substring is the previous one with a single counter incremented -
      O(1) instead of O(n) to maintain.
    - So the outer loop fixes the left end i (resetting the table), the
      inner loop extends the right end j, and after each extension the
      current substring's beauty is read off the table.
    - Reading the beauty means scanning the 26 counters for the smallest
      and largest NON-ZERO one (zero means the character is absent, and
      absent characters must not count as the minimum). That scan is a
      fixed 26 steps regardless of input size, so it is a constant
      factor rather than a third dimension of growth.

    Algorithm Steps
    ----------------
    1. res = 0.
    2. For each left end i in [0, n):
         a. Reset freq[26] to all zeros.
         b. For each right end j in [i, n):
              - freq[s[j] - 'a']++          (extend the substring by one char)
              - scan the 26 counters, taking mn = min and mx = max over
                the entries that are > 0
              - res += (mx - mn)             (the beauty of s[i..j])
    3. Return res.

    Time Complexity: O(n^2 * 26) = O(n^2), since 26 is a constant - the
                      alphabet scan does not grow with the input
    Space Complexity: O(26) = O(1) for the frequency table
*/
int beautySum(string s) {
  int n = s.length();
  int res = 0;

  for (int i = 0; i < n; ++i) {
    vector<int> freq(26, 0);              // fresh table for each left end

    for (int j = i; j < n; ++j) {
      freq[s[j] - 'a']++;                 // extend substring s[i..j] by one char

      int mn = INT_MAX, mx = 0;
      for (int k = 0; k < 26; ++k) {
        if (freq[k] > 0) {                // skip absent characters
          mn = min(mn, freq[k]);
          mx = max(mx, freq[k]);
        }
      }

      res += (mx - mn);                   // beauty of s[i..j]
    }
  }

  return res;
}

int main() {
  vector<string> tests = {"aabcb", "aabcbaa"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: " << beautySum(s) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "aabcb"   (n = 5, answer = 5)
    ==========================================================================

      index:   0   1   2   3   4
      char:    a   a   b   c   b

    Tracked state:
      i    - the fixed LEFT end of the substring; freq is reset here
      j    - the RIGHT end, extending one character at a time
      freq - 26-slot table for the current substring s[i..j]; only the
             non-zero slots are shown below
      mn   - smallest count among the characters PRESENT (zero slots skipped)
      mx   - largest count among the characters present
      res  - running sum of (mx - mn) over every substring

    Initial state: res = 0

    There are 15 substrings in a 5-character string, and all 15 appear
    below - grouped by their left end, since that is how the loops nest.
    Only 5 of them have non-zero beauty.

    ==========================================================================
    OUTER i = 0   (freq reset to all zeros)
    ==========================================================================
      j = 0   "a"      freq  a:1            mn=1 mx=1  ->  +0    res = 0
      j = 1   "aa"     freq  a:2            mn=2 mx=2  ->  +0    res = 0
      j = 2   "aab"    freq  a:2 b:1        mn=1 mx=2  ->  +1    res = 1
      j = 3   "aabc"   freq  a:2 b:1 c:1    mn=1 mx=2  ->  +1    res = 2
      j = 4   "aabcb"  freq  a:2 b:2 c:1    mn=1 mx=2  ->  +1    res = 3

              Note how cheap each step is: adding s[j] bumps ONE counter.
              The table is never rebuilt - that is the whole saving.

              At j = 4 the scan over the 26 slots sees:
                a=2, b=2, c=1, and 23 zeros which are SKIPPED
                mn = 1 (from c), mx = 2 (from a and b), beauty = 1
              If the zero slots were not skipped, mn would be 0 and the
              beauty would be wrong (2 instead of 1) for every substring.

    ==========================================================================
    OUTER i = 1   (freq reset)
    ==========================================================================
      j = 1   "a"      freq  a:1            mn=1 mx=1  ->  +0    res = 3
      j = 2   "ab"     freq  a:1 b:1        mn=1 mx=1  ->  +0    res = 3
      j = 3   "abc"    freq  a:1 b:1 c:1    mn=1 mx=1  ->  +0    res = 3
      j = 4   "abcb"   freq  a:1 b:2 c:1    mn=1 mx=2  ->  +1    res = 4

              "abc" has three distinct characters all appearing once, so
              its most and least frequent counts are equal - beauty 0.
              Beauty measures IMBALANCE, not variety.

    ==========================================================================
    OUTER i = 2   (freq reset)
    ==========================================================================
      j = 2   "b"      freq  b:1            mn=1 mx=1  ->  +0    res = 4
      j = 3   "bc"     freq  b:1 c:1        mn=1 mx=1  ->  +0    res = 4
      j = 4   "bcb"    freq  b:2 c:1        mn=1 mx=2  ->  +1    res = 5

    ==========================================================================
    OUTER i = 3   (freq reset)
    ==========================================================================
      j = 3   "c"      freq  c:1            mn=1 mx=1  ->  +0    res = 5
      j = 4   "cb"     freq  c:1 b:1        mn=1 mx=1  ->  +0    res = 5

    ==========================================================================
    OUTER i = 4   (freq reset)
    ==========================================================================
      j = 4   "b"      freq  b:1            mn=1 mx=1  ->  +0    res = 5

    --------------------------------------------------------------------------
    RETURN res = 5

    ==========================================================================
    Summary: only the contributing substrings
    ==========================================================================

    | i | j | substring | counts        | mn | mx | beauty |
    |---|---|-----------|---------------|----|----|--------|
    | 0 | 2 | "aab"     | a:2 b:1       |  1 |  2 |   1    |
    | 0 | 3 | "aabc"    | a:2 b:1 c:1   |  1 |  2 |   1    |
    | 0 | 4 | "aabcb"   | a:2 b:2 c:1   |  1 |  2 |   1    |
    | 1 | 4 | "abcb"    | a:1 b:2 c:1   |  1 |  2 |   1    |
    | 2 | 4 | "bcb"     | b:2 c:1       |  1 |  2 |   1    |
                                          total beauty  =   5

    The other 10 substrings all have every present character appearing the
    same number of times, so mn == mx and they contribute nothing.

    Step count behind the complexity claim:
      15 substrings were examined, each costing one counter increment plus
      a 26-slot scan: 15 * 26 = 390 slot reads. In general that is
      n(n+1)/2 substrings times 26, i.e. O(n^2 * 26) = O(n^2), because 26
      is a constant that does not grow with the input. Space is O(26) =
      O(1) - one table, reused and reset n times.

    Why the frequency table cannot be carried across the OUTER loop:
      moving i rightwards REMOVES a character from the left of every
      substring, and the table would have to be decremented for it. That
      is possible, but the reset is O(26) and happens only n times, so it
      costs nothing measurable and keeps the logic obviously correct.

    Why min cannot be maintained incrementally:
      mx only ever grows as j extends, so it could be tracked in O(1). mn
      cannot: a brand-new character arrives with count 1 and pushes mn down
      to 1, while incrementing the character that HELD the minimum may push
      mn up - and finding the new minimum then requires looking at the
      other counters anyway. The 26-slot rescan is the simple, correct
      answer, and it is why the 26 factor is in the bound at all.
*/
