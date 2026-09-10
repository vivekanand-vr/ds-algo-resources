#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s and an integer k, return the length of the longest
//    substring that contains EXACTLY k distinct characters. If no such
//    substring exists, return -1.
//
// Example:
// s = "aabacbebebe", k = 3 -> 7   ("cbebebe")
// s = "aaaa",        k = 2 -> -1  (only one distinct character exists)
// s = "aabaaab",     k = 2 -> 7   (the whole string has exactly a and b)

/*
    Approach: "At most k" sliding window, measured only when it hits exactly k

    - A window with EXACTLY k distinct characters is hard to maintain
      directly, because dropping a character can take the count from k
      straight to k-1 and there is no clean way to grow back. A window
      with AT MOST k distinct characters, on the other hand, is easy:
      grow on the right, and whenever the distinct count exceeds k,
      shrink from the left until it is back within budget.
    - The trick is that the longest "exactly k" substring is always
      visible from inside such an at-most-k window: at the moment the
      window holds precisely k distinct characters, it is as wide as it
      can be for that right endpoint (we only ever shrank because we
      were over budget). So measuring the width whenever
      distinct == k is enough - no separate pass needed.
    - The count map's size() IS the distinct count, which is why keys
      are erased the moment their count reaches 0.
    - `c` starts at -1 so that a string which never reaches k distinct
      characters naturally reports "no such substring".

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = 0, c = -1 and an empty count map.
    2. While r < n:
         a. Increment the count of s[r] (grow the window).
         b. While the number of distinct characters exceeds k:
              - decrement count[s[l]], erase the key if it hits 0
              - l++ (shrink from the left)
         c. The window now holds AT MOST k distinct characters. If it
            holds exactly k, update c = max(c, r - l + 1).
         d. r++.
    3. Return c (still -1 if exactly k was never reached).

    Time Complexity: O(n log k) - each index enters and leaves the window
                      once, with O(log k) per map operation. Using
                      unordered_map (or an int[256] table) makes it O(n).
    Space Complexity: O(k) - the map never holds more than k + 1 keys
*/
int longestKSubstr(string& s, int k) {
  int n = s.length();
  int l = 0, r = 0, c = -1;
  map<char, int> mp;

  while (r < n) {
    mp[s[r]]++;                          // grow the window to include s[r]

    while ((int)mp.size() > k) {         // over budget -> shrink from the left
      mp[s[l]]--;
      if (mp[s[l]] == 0) {
        mp.erase(s[l]);                  // drop keys at zero so size() = distinct count
      }
      l++;
    }

    // Window currently has AT MOST k distinct characters
    if ((int)mp.size() == k) {
      c = max(c, r - l + 1);             // widest window for this right end
    }

    r++;
  }

  return c;
}

int main() {
  vector<pair<string, int>> tests = {{"aabacbebebe", 3}, {"aaaa", 2}, {"aabaaab", 2}};

  for (auto& [s, k] : tests) {
    string str = s;                       // longestKSubstr takes a non-const reference
    cout << "s = \"" << s << "\", k = " << k << " -> " << longestKSubstr(str, k) << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "aabaaab", k = 2   (n = 7, answer = 7)
    ==========================================================================

      index:   0   1   2   3   4   5   6
      char:    a   a   b   a   a   a   b

    Tracked state:
      mp        - character -> how many times it occurs inside s[l..r]
      mp.size() - the DISTINCT character count of the window (keys at zero
                  are erased, which is what keeps size() honest)
      l, r      - the window bounds; window is s[l..r], width = r - l + 1
      c         - best width seen while the window held EXACTLY k distinct;
                  starts at -1 so "never reached k" reports -1

    Initial state: l = 0, r = 0, c = -1, mp = {}

    --------------------------------------------------------------------------
    r = 0, s[r] = 'a'
      add          mp['a']++            -> mp = {a:1}
      size         mp.size() = 1
      shrink?      1 > k = 2 is false   -> no shrink
      record?      size 1 != 2          -> NOT recorded (only 1 distinct)
      window       s[0..0] = "a", width = 1, but width is ignored here
      advance      l = 0, c = -1, r -> 1

    --------------------------------------------------------------------------
    r = 1, s[r] = 'a'
      add          mp['a']++            -> mp = {a:2}
      size         mp.size() = 1        (count grew, distinct did not)
      shrink?      1 > 2 is false       -> no shrink
      record?      size 1 != 2          -> NOT recorded
      window       s[0..1] = "aa", width = 2
      advance      l = 0, c = -1, r -> 2

                   This is the subtle bit: the window "aa" is 2 wide but
                   holds only ONE distinct character, so it is not a valid
                   answer for k = 2 even though its width beats nothing.

    --------------------------------------------------------------------------
    r = 2, s[r] = 'b'      <-- first moment the window hits exactly k
      add          mp['b']++            -> mp = {a:2, b:1}
      size         mp.size() = 2
      shrink?      2 > 2 is false       -> no shrink (at budget, not over)
      record?      size == k = 2        -> c = max(-1, 2 - 0 + 1) = 3
      window       s[0..2] = "aab", width = 3

                       a   a   b   a   a   a   b
                       ^       ^
                       l       r        mp = {a:2, b:1}, size = 2 = k

      advance      l = 0, c = 3, r -> 3

    --------------------------------------------------------------------------
    r = 3, s[r] = 'a'
      add          mp['a']++            -> mp = {a:3, b:1}
      size         mp.size() = 2
      shrink?      2 > 2 is false       -> no shrink, l STAYS 0
      record?      size == 2            -> c = max(3, 3 - 0 + 1) = 4
      window       s[0..3] = "aaba", width = 4                  <-- new best
      advance      l = 0, c = 4, r -> 4

    --------------------------------------------------------------------------
    r = 4, s[r] = 'a'
      add          mp['a']++            -> mp = {a:4, b:1}
      size         mp.size() = 2
      shrink?      no
      record?      size == 2            -> c = max(4, 4 - 0 + 1) = 5
      window       s[0..4] = "aabaa", width = 5                 <-- new best
      advance      l = 0, c = 5, r -> 5

    --------------------------------------------------------------------------
    r = 5, s[r] = 'a'
      add          mp['a']++            -> mp = {a:5, b:1}
      size         mp.size() = 2
      shrink?      no
      record?      size == 2            -> c = max(5, 5 - 0 + 1) = 6
      window       s[0..5] = "aabaaa", width = 6                <-- new best
      advance      l = 0, c = 6, r -> 6

    --------------------------------------------------------------------------
    r = 6, s[r] = 'b'
      add          mp['b']++            -> mp = {a:5, b:2}
      size         mp.size() = 2        (the second 'b' adds a count, not a
                                         new key - distinct is unchanged)
      shrink?      2 > 2 is false       -> no shrink
      record?      size == 2            -> c = max(6, 6 - 0 + 1) = 7
      window       s[0..6] = "aabaaab", width = 7               <-- new best

                       a   a   b   a   a   a   b
                       ^                       ^
                       l                       r
                       whole string, mp = {a:5, b:2}, size = 2 = k

      advance      l = 0, c = 7, r -> 7   ->   r == n, loop ends

    --------------------------------------------------------------------------
    RETURN c = 7        (the entire string "aabaaab")

    ==========================================================================
    Summary table
    ==========================================================================

    | r | s[r] | mp after add | size | shrunk? | l | window    | recorded |
    |---|------|--------------|------|---------|---|-----------|----------|
    | 0 |  a   | {a:1}        |  1   | no      | 0 | "a"       | no       |
    | 1 |  a   | {a:2}        |  1   | no      | 0 | "aa"      | no       |
    | 2 |  b   | {a:2, b:1}   |  2   | no      | 0 | "aab"     | c = 3    |
    | 3 |  a   | {a:3, b:1}   |  2   | no      | 0 | "aaba"    | c = 4    |
    | 4 |  a   | {a:4, b:1}   |  2   | no      | 0 | "aabaa"   | c = 5    |
    | 5 |  a   | {a:5, b:1}   |  2   | no      | 0 | "aabaaa"  | c = 6    |
    | 6 |  b   | {a:5, b:2}   |  2   | no      | 0 | "aabaaab" | c = 7    |

    Step count behind the O(n log k) claim:
      r advanced 7 times and l advanced 0 times - the inner while loop was
      entered zero times on this input, because the string contains only
      two distinct characters in total and k = 2, so the window could
      never go over budget. 7 pointer steps for a 7-character string.
      That is the extreme case of the usual argument: l and r only move
      forward, so together they take at most 2n steps no matter what.

    The one subtlety to hold onto - two different tests, one after the other:
      the SHRINK test is size > k (strictly over budget) and the RECORD
      test is size == k (exactly at budget). They are not the same test,
      and both are needed:
        - shrinking on size >= k would destroy the very windows we want;
        - recording on size <= k would count under-full windows such as
          "aa" at r = 1, which has 2 characters but only 1 distinct, and
          would wrongly answer 2 for s = "aaaa", k = 2 instead of -1.
      Because the shrink loop stops the instant size is back to k, the
      window measured at each r is the WIDEST one ending at r with at most
      k distinct - so taking the max over r is enough, with no second pass.
*/
