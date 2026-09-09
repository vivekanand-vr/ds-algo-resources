#include <bits/stdc++.h>
using namespace std;

// Q: Given a string num representing a large non-negative integer, return
//    the largest-value odd substring that is a PREFIX of num. Return ""
//    if no odd substring exists.
//
// Example:
// num = "52"    -> "5"
// num = "4206"  -> ""
// num = "35427" -> "35427"

/*
    Approach: Trim trailing even digits

    - A prefix num[0..i] represents an odd number iff its last digit
      num[i] is odd (the earlier digits don't affect parity).
    - To get the LARGEST possible odd prefix we want it to be as long as
      possible, so we look for the odd digit closest to the end of the
      string and cut right after it, discarding the even digits that
      follow it. Any prefix longer than that would end in an even digit
      and not qualify; any prefix shorter than that is a smaller number.
    - If no digit in num is odd, no odd prefix exists at all.

    Algorithm Steps
    ----------------
    1. Scan the string from the last index i = n-1 down to 0.
    2. If num[i] is odd, return the prefix num[0..i] (substring of
       length i+1) - this is the answer.
    3. If the loop finishes with no odd digit found, return "".

    Time Complexity: O(n) - single scan in the worst case (all even digits)
    Space Complexity: O(n) for the returned substring (O(1) extra)
*/
string largestOddNumber(string num) {
  int n = (int)num.length();

  for (int i = n - 1; i >= 0; --i) {
    if ((num[i] - '0') % 2 == 1) {   // last digit of this prefix is odd
      return num.substr(0, i + 1);
    }
  }

  return "";   // no odd digit anywhere in num
}

int main() {
  vector<string> tests = {"52", "4206", "35427"};

  for (const string& num : tests) {
    cout << "Input:  " << num << endl;
    cout << "Output: \"" << largestOddNumber(num) << "\"" << endl << endl;
  }

  return 0;
}
