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

/*
    ==========================================================================
    DRY RUN: arr = {"flower", "flow", "flight"}   (n = 3, answer = "fl")
    ==========================================================================

      column:     0   1   2   3  | 4   5
      arr[0]:     f   l   o   w  | e   r      len 6  (the reference string)
      arr[1]:     f   l   o   w  | .   .      len 4  <-- SHORTEST
      arr[2]:     f   l   i   g  | h   t      len 6
                                 |
                           minLen = 4; columns 4 and 5 are never read

    Tracked state:
      minLen - length of the shortest string; the hard upper bound on the
               loop, because a common prefix can never be longer than the
               shortest member
      i      - the column being compared across all strings
      c      - arr[0][i], the reference character for column i
      j      - the inner index walking over arr[1], arr[2], ...
      res    - the prefix confirmed so far, grown one column at a time

    Pre-pass (computing the bound):
      arr not empty -> continue
      minLen = INT_MAX
        see "flower" (6)  -> minLen = 6
        see "flow"   (4)  -> minLen = 4
        see "flight" (6)  -> minLen = 4   (unchanged)
      minLen = 4, res = ""

    --------------------------------------------------------------------------
    i = 0    (column 0)
      reference  c = arr[0][0] = 'f'
      compare    step 1: j = 1, arr[1][0] = 'f' == 'f'  -> ok
                 step 2: j = 2, arr[2][0] = 'f' == 'f'  -> ok
      agreed     whole column matches
      append     res -> "f"

    --------------------------------------------------------------------------
    i = 1    (column 1)
      reference  c = arr[0][1] = 'l'
      compare    step 1: j = 1, arr[1][1] = 'l' == 'l'  -> ok
                 step 2: j = 2, arr[2][1] = 'l' == 'l'  -> ok
      agreed     whole column matches
      append     res -> "fl"

    --------------------------------------------------------------------------
    i = 2    (column 2)     <-- MISMATCH, the scan stops here
      reference  c = arr[0][2] = 'o'
      compare    step 1: j = 1, arr[1][2] = 'o' == 'o'  -> ok
                 step 2: j = 2, arr[2][2] = 'i' != 'o'  -> DISAGREES

                 f  l  o  w  e  r
                 f  l  o  w
                 f  l  i  g  h  t
                       ^
                       column 2: "flower"/"flow" say 'o', "flight" says 'i'

      return     the inner loop returns res IMMEDIATELY, without appending
                 c - so the 'o' that two of the three strings share is
                 correctly left out
      note       i never reaches 3, and would have been stopped at 4 by
                 minLen anyway

    --------------------------------------------------------------------------
    RETURN res = "fl"

    ==========================================================================
    Summary table
    ==========================================================================

    | i | c | arr[1][i] | arr[2][i] | all agree? | res  |
    |---|---|-----------|-----------|------------|------|
    | 0 | f |     f     |     f     |    yes     | "f"  |
    | 1 | l |     l     |     l     |    yes     | "fl" |
    | 2 | o |     o     |     i     |  NO -> ret | "fl" |

    Step count behind the O(n * minLen) claim:
      3 columns were visited and 2 comparisons were made in each, so 6
      character comparisons - well under the bound of 3 columns x 2 others
      x minLen 4. The mismatch short-circuits both loops at once.

    Why minLen is load-bearing, not just an optimization:
      on this input arr[0] = "flower" is LONGER than arr[1] = "flow". Without
      the bound the loop would run i up to 5 and evaluate arr[1][4], reading
      one past the end of a 4-character string - undefined behavior, not a
      graceful "no match". The bound also means that when every string does
      share everything, the loop ends naturally at i == minLen and res comes
      out equal to the shortest string (as with main()'s third test,
      {"interview", "interstellar", "internet"} -> "inter", where the stop
      is a mismatch at column 5: 'v' vs 's').
*/
