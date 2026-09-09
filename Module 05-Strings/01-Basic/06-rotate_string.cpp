#include <bits/stdc++.h>
using namespace std;

// Q: Given two strings s and goal, return true if s can become goal after
//    some number of left/right cyclic shifts (rotations) of s.
//
// Example:
// s = "abcde", goal = "cdeab" -> true
// s = "abcde", goal = "abced" -> false

/*
    Approach: Search within the doubled string

    - Every possible rotation of `goal` shows up as a contiguous
      substring of `goal + goal`. For example goal = "cdeab" doubled is
      "cdeabcdeab", and you can find "abcde" starting at index 3.
    - So instead of generating and comparing every rotation one by one
      (O(n) rotations, O(n) each to compare -> O(n^2) anyway but with
      more bookkeeping), just build goal+goal once and check whether it
      contains s as a substring.
    - Lengths must match first, otherwise s can never equal any rotation
      of goal.

    Algorithm Steps
    ----------------
    1. If s.length() != goal.length(), return false.
    2. Build doubled = goal + goal.
    3. Return true if doubled contains s as a substring, false otherwise.

    Time Complexity: O(n) on average with std::string::find's underlying
                      search (worst case O(n^2) for pathological inputs)
    Space Complexity: O(n) for the doubled string
*/
bool rotateString(string s, string goal) {
  if (s.length() != goal.length()) return false;

  string doubled = goal + goal;         // every rotation of goal is a substring of this
  return doubled.find(s) != string::npos;
}

int main() {
  vector<pair<string, string>> tests = {{"abcde", "cdeab"}, {"abcde", "abced"}};

  for (auto& [s, goal] : tests) {
    cout << "s = \"" << s << "\", goal = \"" << goal << "\" -> "
         << (rotateString(s, goal) ? "true" : "false") << endl;
  }

  return 0;
}
