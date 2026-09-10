#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s, find the length of the longest substring that
//    contains no repeating characters.
//
// Example:
// s = "abcabcbb" -> 3   ("abc")
// s = "bbbbb"    -> 1   ("b")
// s = "pwwkew"   -> 3   ("wke", not "pwke" - that is a subsequence)

/*
    Approach: Variable-size sliding window with a character count map

    - A substring without repeats is exactly a window in which every
      character count is 1. So keep a window [l, r] and a map of how
      many times each character inside it occurs.
    - Extend the window one character at a time by moving r right. The
      only character that can possibly break the "no repeats" invariant
      is the one just added, so it is enough to check whether
      count[s[r]] became 2: if so, shrink from the left (dropping
      counts as l advances) until that duplicate is gone.
    - Because the invariant is restored before measuring, every
      r - l + 1 seen is the length of a valid duplicate-free substring,
      and the largest of those is the answer.
    - l and r each only ever move forward, so despite the nested while
      loop every index is added and removed at most once - the scan is
      linear, not quadratic.

    Algorithm Steps
    ----------------
    1. Initialize l = 0, r = 0, best = 0 and an empty count map.
    2. While r < n:
         a. Increment the count of s[r] (grow the window).
         b. While count[s[r]] > 1 (the new character is a duplicate):
              - decrement count[s[l]], erase the key if it hits 0
              - l++ (shrink from the left)
         c. The window is now duplicate-free -> best = max(best, r - l + 1).
         d. r++.
    3. Return best.

    Time Complexity: O(n log k) - each index enters and leaves the window
                      once, and every map operation costs O(log k) for k
                      distinct characters. Swapping map for unordered_map
                      (or a fixed int[256] table) makes it O(n).
    Space Complexity: O(k) for the count map, k = distinct characters
*/
int lengthOfLongestSubstring(string s) {
  int n = s.length();
  int l = 0, r = 0, c = 0;
  map<char, int> mp;

  while (r < n) {
    mp[s[r]]++;                    // grow the window to include s[r]

    while (mp[s[r]] > 1) {         // s[r] duplicated -> shrink from the left
      mp[s[l]]--;
      if (mp[s[l]] == 0) {
        mp.erase(s[l]);            // drop keys at zero so size() = distinct count
      }
      l++;
    }

    c = max(c, r - l + 1);         // window is duplicate-free here
    r++;
  }

  return c;
}

int main() {
  vector<string> tests = {"abcabcbb", "bbbbb", "pwwkew"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: " << lengthOfLongestSubstring(s) << endl << endl;
  }

  return 0;
}
