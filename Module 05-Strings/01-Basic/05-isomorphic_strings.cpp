#include <bits/stdc++.h>
using namespace std;

// Q: Given two strings s and t of the same length, return true if s is
//    isomorphic to t - i.e. there exists a bijective (one-to-one and
//    onto) mapping from every character in s to a character in t such
//    that replacing each character of s per the mapping yields t.
//
// Example:
// s = "egg", t = "add" -> true   (e->a, g->d)
// s = "foo", t = "bar" -> false  (o would need to map to both a and r)
// s = "paper", t = "title" -> true

/*
    Approach: Two fixed-size lookup tables (no ordered map needed)

    - The original solution used map<char, char>, which does an O(log n)
      tree lookup/insert per character. Since characters are just bytes
      (0-255), a plain array indexed by the character itself gives O(1)
      access - no hashing or tree balancing needed.
    - We need the mapping to be a true bijection, so we maintain it in
      BOTH directions: mapS2T[s[i]] must consistently point to t[i], and
      mapT2S[t[i]] must consistently point back to s[i]. Checking only
      one direction would wrongly allow two different source characters
      to map to the same target character.

    Algorithm Steps
    ----------------
    1. If s.length() != t.length(), return false.
    2. Allocate mapS2T[256] and mapT2S[256], both initialized to -1
       (meaning "unmapped" - safe since actual char values are 0-255).
    3. For each index i:
         a. If mapS2T[s[i]] is unmapped, set it to t[i].
            Else if it doesn't equal t[i], return false (inconsistent).
         b. If mapT2S[t[i]] is unmapped, set it to s[i].
            Else if it doesn't equal s[i], return false (not a bijection).
    4. If the loop completes, return true.

    Time Complexity: O(n) - single pass, O(1) work per character
    Space Complexity: O(1) - two fixed 256-size tables regardless of input
*/
bool isIsomorphic(string s, string t) {
  if (s.length() != t.length()) return false;

  vector<int> mapS2T(256, -1), mapT2S(256, -1);
  int n = (int)s.length();

  for (int i = 0; i < n; ++i) {
    unsigned char cs = s[i], ct = t[i];

    if (mapS2T[cs] == -1) {
      mapS2T[cs] = ct;
    } else if (mapS2T[cs] != ct) {
      return false;                 // s[i] already maps to a different char
    }

    if (mapT2S[ct] == -1) {
      mapT2S[ct] = cs;
    } else if (mapT2S[ct] != cs) {
      return false;                 // t[i] is already claimed by another char
    }
  }

  return true;
}

int main() {
  vector<pair<string, string>> tests = {{"egg", "add"}, {"foo", "bar"}, {"paper", "title"}};

  for (auto& [s, t] : tests) {
    cout << "s = \"" << s << "\", t = \"" << t << "\" -> "
         << (isIsomorphic(s, t) ? "true" : "false") << endl;
  }

  return 0;
}
