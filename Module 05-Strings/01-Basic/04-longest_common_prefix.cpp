#include <bits/stdc++.h>
using namespace std;

// Q: Given an array of strings arr, return the longest common prefix
//    shared by all of them. Return "" if there is no common prefix.
//
// Example:
// arr = {"flower", "flow", "flight"} -> "fl"
// arr = {"dog", "racecar", "car"}    -> ""

/*
    Approach: Column-by-column scan bounded by the shortest string

    - The original version walked index i up to arr[0].length() and
      compared arr[j][i] for every other string WITHOUT checking that i
      is still within arr[j]'s bounds. If arr[0] happens to be longer
      than some other string, that reads out of bounds - undefined
      behavior. Fixing this requires capping the scan at the length of
      the SHORTEST string in the array, since the common prefix can
      never be longer than the shortest word.
    - With that bound in place: treat arr[0] as the reference and, for
      every column index i, compare arr[0][i] against the same index in
      every other string. The moment any string disagrees (or we run
      past the shortest string), the prefix built so far is the answer.

    Algorithm Steps
    ----------------
    1. If arr is empty, return "".
    2. Compute minLen = length of the shortest string in arr.
    3. For i from 0 to minLen - 1:
         a. c = arr[0][i]
         b. For every other string j, if arr[j][i] != c, return the
            prefix accumulated so far.
         c. Append c to the result.
    4. Return the result (it will equal the shortest string if every
       string turned out to share it entirely as a prefix).

    Time Complexity: O(n * minLen) worst case, where n = number of strings
                      (bounded above by the total character count S)
    Space Complexity: O(minLen) for the result string
*/
string longestCommonPrefix(vector<string>& arr) {
  if (arr.empty()) return "";

  int minLen = INT_MAX;
  for (const string& s : arr) minLen = min(minLen, (int)s.length());

  string res;
  for (int i = 0; i < minLen; ++i) {
    char c = arr[0][i];
    for (int j = 1; j < (int)arr.size(); ++j) {
      if (arr[j][i] != c) return res;   // mismatch found, stop here
    }
    res.push_back(c);
  }

  return res;
}

int main() {
  vector<vector<string>> tests = {
      {"flower", "flow", "flight"},
      {"dog", "racecar", "car"},
      {"interview", "interstellar", "internet"},
  };

  for (auto& arr : tests) {
    cout << "Input:  [";
    for (size_t i = 0; i < arr.size(); ++i) cout << "\"" << arr[i] << "\"" << (i + 1 < arr.size() ? ", " : "");
    cout << "]" << endl;
    cout << "Output: \"" << longestCommonPrefix(arr) << "\"" << endl << endl;
  }

  return 0;
}
