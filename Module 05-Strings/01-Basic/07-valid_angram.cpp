#include <bits/stdc++.h>
using namespace std;

// Q: Given two strings s and t, return true if t is an anagram of s
//    (i.e. t is a rearrangement of exactly the same letters as s).
//
// Example:
// s = "anagram", t = "nagaram" -> true
// s = "rat", t = "car"          -> false

/*
    Approach: Letter frequency count (assuming lowercase a-z input)

    - Two strings are anagrams of each other iff they have identical
      letter frequency counts. Different lengths can never be anagrams.
    - Use a single fixed-size array of 26 counters (one per letter):
      increment the counter for each letter seen in s, decrement it for
      each letter seen in t. If s and t are anagrams, every counter ends
      up back at exactly 0.

    Algorithm Steps
    ----------------
    1. If s.length() != t.length(), return false.
    2. Allocate freq[26] initialized to 0.
    3. For each index i: freq[s[i]-'a']++ and freq[t[i]-'a']--.
    4. Return true only if every entry in freq is 0.

    Time Complexity: O(n) - one pass to count, one fixed 26-length pass to verify
    Space Complexity: O(1) - a fixed 26-size array regardless of input length
*/
bool isAnagram(string s, string t) {
  if (s.length() != t.length()) return false;

  vector<int> freq(26, 0);
  int n = (int)s.length();
  for (int i = 0; i < n; ++i) {
    freq[s[i] - 'a']++;
    freq[t[i] - 'a']--;
  }

  return all_of(freq.begin(), freq.end(), [](int c) { return c == 0; });
}

int main() {
  vector<pair<string, string>> tests = {{"anagram", "nagaram"}, {"rat", "car"}};

  for (auto& [s, t] : tests) {
    cout << "s = \"" << s << "\", t = \"" << t << "\" -> "
         << (isAnagram(s, t) ? "true" : "false") << endl;
  }

  return 0;
}
