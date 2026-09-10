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

/*
    ==========================================================================
    DRY RUN: s = "abcde", goal = "cdeab"   (answer = true)
    ==========================================================================

      s:       a  b  c  d  e        goal:    c  d  e  a  b
      index:   0  1  2  3  4        index:   0  1  2  3  4

    Tracked state:
      doubled - goal concatenated with itself; every rotation of goal
                appears inside it as a contiguous substring
      find    - the index where s first matches, or string::npos

    --------------------------------------------------------------------------
    Step 1 - length guard
      s.length() = 5, goal.length() = 5, equal -> continue
      (a length mismatch would return false immediately: no rotation can
       change how many characters a string has)

    --------------------------------------------------------------------------
    Step 2 - build the doubled string
      doubled = goal + goal = "cdeab" + "cdeab" = "cdeabcdeab"

          index:   0  1  2  3  4  5  6  7  8  9
          char:    c  d  e  a  b  c  d  e  a  b
                   |<-- goal --->|<-- goal --->|

      Every rotation of goal starts somewhere in the first half and reads
      forward 5 characters, so all 5 rotations are visible as windows:

          start 0: "cdeab"   (goal itself)
          start 1: "deabc"
          start 2: "eabcd"
          start 3: "abcde"   <-- this is s
          start 4: "bcdea"

    --------------------------------------------------------------------------
    Step 3 - search
      doubled.find("abcde") scans left to right for the first match:

          start 0: 'c' vs 'a'  -> mismatch at once
          start 1: 'd' vs 'a'  -> mismatch at once
          start 2: 'e' vs 'a'  -> mismatch at once
          start 3: 'a','b','c','d','e' all match  -> HIT at index 3

               c  d  e  a  b  c  d  e  a  b
                        ^-----------^
                        index 3, length 5 = "abcde"

      returns 3, which is != string::npos

    --------------------------------------------------------------------------
    RETURN true

    ==========================================================================
    Contrast: s = "abcde", goal = "abced"   (answer = false)
    ==========================================================================

      doubled = "abcedabced"

          index:   0  1  2  3  4  5  6  7  8  9
          char:    a  b  c  e  d  a  b  c  e  d

      find("abcde") tries every start and fails:
          start 0: 'a','b','c' match, then 'e' vs 'd'  -> mismatch
          start 5: 'a','b','c' match, then 'e' vs 'd'  -> mismatch
          every other start fails on the first character

      returns string::npos  ->  RETURN false

    Why the doubling trick is valid:
      rotating goal left by i produces goal[i..n-1] + goal[0..i-1], which is
      precisely the n-character window of goal+goal beginning at index i.
      So "is s a rotation of goal" becomes one substring search, and there
      is no need to generate the rotations one at a time.

    Time / space:
      building `doubled` is O(n) time and O(n) space; the find is O(n) on
      average and O(n^2) worst case for a pathological input. Note the
      search only needs to consider starts 0..n-1 - a match beginning in
      the second half would run off the end - which std::find handles
      naturally since no such match can complete.
*/
