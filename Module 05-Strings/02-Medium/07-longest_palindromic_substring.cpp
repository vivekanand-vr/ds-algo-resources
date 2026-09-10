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
