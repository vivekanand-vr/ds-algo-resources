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
