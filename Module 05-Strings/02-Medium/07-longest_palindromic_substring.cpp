#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s, return the longest palindromic substring in s.
//    (Any one of them is acceptable when several tie on length.)
//
// Example:
// s = "babad" -> "bab"   ("aba" is equally valid)
// s = "cbbd"  -> "bb"
// s = "a"     -> "a"

/*
    Approach: Expand around every possible center

    - Every palindrome is symmetric about a center, and reading it
      outward from that center gives matching character pairs. So
      instead of testing all O(n^2) substrings for palindromicity in
      O(n) each, enumerate the CENTERS and grow outward while the
      characters still match.
    - There are two center shapes, and both must be tried:
        - odd length:  centered on a single character i     -> (i, i)
        - even length: centered between i and i + 1          -> (i, i+1)
      That is 2n - 1 centers in total.
    - Expansion from one center stops the moment the outward pair stops
      matching, because a palindrome cannot have a mismatched pair
      inside it - so no further widening from that center can succeed.
    - The two expansion loops in the original version were identical
      apart from their starting pair, so they are factored into one
      `expandAroundCenter` helper that reports back the best span it
      found. Same algorithm, half the code, and it handles the empty
      string cleanly (bestLen stays 0 -> substr(0, 0) == "").

    Algorithm Steps
    ----------------
    1. Track bestStart = 0 and bestLen = 0.
    2. For each index i in [0, n):
         a. expandAroundCenter(s, i, i)      - odd-length palindromes
         b. expandAroundCenter(s, i, i + 1)  - even-length palindromes
    3. expandAroundCenter(s, l, r): while l >= 0, r < n and s[l] == s[r]:
         - if (r - l + 1) > bestLen, record bestLen and bestStart = l
         - l--, r++ (step outward one pair)
    4. Return s.substr(bestStart, bestLen).

    Time Complexity: O(n^2) - 2n - 1 centers, each expanding O(n) in the
                      worst case (e.g. "aaaa...a")
    Space Complexity: O(1) extra, plus O(n) for the returned substring.
                       (Manacher's algorithm solves this in O(n) time but
                        is rarely needed in an interview setting.)
*/
void expandAroundCenter(const string& s, int l, int r, int& bestStart, int& bestLen) {
  int n = s.length();

  while (l >= 0 && r < n && s[l] == s[r]) {
    if ((r - l + 1) > bestLen) {         // wider palindrome found
      bestLen = r - l + 1;
      bestStart = l;
    }
    l--;                                  // step outward one pair
    r++;
  }
}

string longestPalindrome(string s) {
  int n = s.length();
  int bestStart = 0, bestLen = 0;

  for (int i = 0; i < n; ++i) {
    expandAroundCenter(s, i, i, bestStart, bestLen);       // odd length
    expandAroundCenter(s, i, i + 1, bestStart, bestLen);   // even length
  }

  return s.substr(bestStart, bestLen);
}

int main() {
  vector<string> tests = {"babad", "cbbd", "a", "forgeeksskeegfor"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: \"" << longestPalindrome(s) << "\"" << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "babad"   (n = 5, answer = "bab")
    ==========================================================================

      index:   0   1   2   3   4
      char:    b   a   b   a   d

    Tracked state:
      bestStart - start index of the widest palindrome found so far
      bestLen   - its width; starts at 0, so an empty string returns ""
      l, r      - the expanding pair inside expandAroundCenter, moving
                  OUTWARD (l decreasing, r increasing) - the opposite of
                  a converging two-pointer walk

    Initial state: bestStart = 0, bestLen = 0

    Every index is tried as TWO centers:
      expand(i, i)     - odd-length palindromes, centred on a character
      expand(i, i + 1) - even-length palindromes, centred on the gap
    That is 2n - 1 = 9 centers for this input.

    --------------------------------------------------------------------------
    i = 0
      odd  expand(0, 0)
             step 1: l=0, r=0, s[0]='b' == s[0]='b'  -> width 1 > 0
                     RECORD bestLen = 1, bestStart = 0      ("b")
                     l -> -1, r -> 1
             step 2: l = -1 fails l >= 0  -> stop
      even expand(0, 1)
             step 1: s[0]='b' vs s[1]='a'  -> mismatch, loop never runs
                     nothing recorded

    --------------------------------------------------------------------------
    i = 1      <-- the center that finds the answer
      odd  expand(1, 1)
             step 1: l=1, r=1, s[1]='a' == s[1]='a'  -> width 1, not > 1
                     no record (ties do not replace)
                     l -> 0, r -> 2
             step 2: l=0, r=2, s[0]='b' == s[2]='b'  -> width 3 > 1
                     RECORD bestLen = 3, bestStart = 0    ("bab")
                     l -> -1, r -> 3
             step 3: l = -1 fails  -> stop

                     b   a   b   a   d
                     ^   ^   ^
                     l  ctr  r      expanded outward from 'a' at index 1
                     |<-"bab" ->|

      even expand(1, 2)
             step 1: s[1]='a' vs s[2]='b'  -> mismatch, nothing

    --------------------------------------------------------------------------
    i = 2
      odd  expand(2, 2)
             step 1: s[2]='b' == s[2]='b'  -> width 1, not > 3, no record
                     l -> 1, r -> 3
             step 2: s[1]='a' == s[3]='a'  -> width 3, not > 3, NO RECORD
                     l -> 0, r -> 4
             step 3: s[0]='b' vs s[4]='d'  -> mismatch, stop

                     This center finds "aba", also 3 wide. Because the test
                     is strictly `>` and not `>=`, the earlier "bab" is
                     kept. Both are valid answers; the strict comparison is
                     what makes the choice deterministic.

      even expand(2, 3)
             step 1: s[2]='b' vs s[3]='a'  -> mismatch, nothing

    --------------------------------------------------------------------------
    i = 3
      odd  expand(3, 3)
             step 1: s[3]='a' == s[3]='a'  -> width 1, no record
                     l -> 2, r -> 4
             step 2: s[2]='b' vs s[4]='d'  -> mismatch, stop
      even expand(3, 4)
             step 1: s[3]='a' vs s[4]='d'  -> mismatch, nothing

    --------------------------------------------------------------------------
    i = 4
      odd  expand(4, 4)
             step 1: s[4]='d' == s[4]='d'  -> width 1, no record
                     l -> 3, r -> 5
             step 2: r = 5 fails r < n = 5  -> stop
      even expand(4, 5)
             step 1: r = 5 fails r < n  -> loop never runs

    --------------------------------------------------------------------------
    RETURN s.substr(bestStart, bestLen) = s.substr(0, 3) = "bab"

    ==========================================================================
    Summary table  (one row per center)
    ==========================================================================

    | i | center     | expansions | widest here | recorded?        |
    |---|------------|------------|-------------|------------------|
    | 0 | (0,0) odd  |     1      | "b"    (1)  | yes, bestLen = 1 |
    | 0 | (0,1) even |     0      | none        | no               |
    | 1 | (1,1) odd  |     2      | "bab"  (3)  | yes, bestLen = 3 |
    | 1 | (1,2) even |     0      | none        | no               |
    | 2 | (2,2) odd  |     2      | "aba"  (3)  | no, ties         |
    | 2 | (2,3) even |     0      | none        | no               |
    | 3 | (3,3) odd  |     1      | "a"    (1)  | no               |
    | 3 | (3,4) even |     0      | none        | no               |
    | 4 | (4,4) odd  |     1      | "d"    (1)  | no               |
    | 4 | (4,5) even |     0      | none        | no               |

    Step count behind the O(n^2) claim:
      9 centers were tried and 10 character-pair comparisons succeeded or
      failed in total here - cheap because this string has no long
      palindrome. The worst case is a string like "aaaaa", where every
      center expands to the ends: that is ~n/2 comparisons per center
      across 2n-1 centers, i.e. O(n^2). Space stays O(1) beyond the
      returned substring, since only two integers are carried.

    Why "stop at the first mismatch" is safe:
      a palindrome cannot contain a mismatched symmetric pair. So once
      s[l] != s[r], no WIDER window around this same center can be a
      palindrome either - every one of them contains that same bad pair.
      Nothing is lost by abandoning the center immediately.

    Why both center shapes are needed:
      odd centers alone would miss every even-length palindrome. On "cbbd"
      the odd pass finds only single characters, and it is expand(1, 2)
      that discovers "bb" - so dropping the even call would return "c"
      instead of "bb".
*/
