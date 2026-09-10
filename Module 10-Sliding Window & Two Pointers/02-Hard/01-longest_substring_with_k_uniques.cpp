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
