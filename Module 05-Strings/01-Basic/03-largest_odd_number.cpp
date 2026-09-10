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

/*
    ==========================================================================
    DRY RUN: num = "4206"   (n = 4, answer = "")
    ==========================================================================

      index:   0   1   2   3
      digit:   4   2   0   6

    Tracked state:
      i - the scan position, walking from n - 1 down to 0; it is also the
          last index of the prefix num[0..i] being considered

    Initial state: i = 3

    --------------------------------------------------------------------------
    i = 3
      read     num[3] = '6'
      test     (6) % 2 == 0  -> even, this prefix "4206" is an even number
      skip     i -> 2

    --------------------------------------------------------------------------
    i = 2
      read     num[2] = '0'
      test     (0) % 2 == 0  -> even, prefix "420" rejected
      skip     i -> 1

    --------------------------------------------------------------------------
    i = 1
      read     num[1] = '2'
      test     (2) % 2 == 0  -> even, prefix "42" rejected
      skip     i -> 0

    --------------------------------------------------------------------------
    i = 0
      read     num[0] = '4'
      test     (4) % 2 == 0  -> even, prefix "4" rejected
      skip     i -> -1, the `i >= 0` condition fails, loop ends

    --------------------------------------------------------------------------
    RETURN ""        (no odd digit exists anywhere, so no odd prefix can)

    ==========================================================================
    Summary table
    ==========================================================================

    | i | num[i] | odd? | action                    |
    |---|--------|------|---------------------------|
    | 3 |   6    |  no  | keep scanning left        |
    | 2 |   0    |  no  | keep scanning left        |
    | 1 |   2    |  no  | keep scanning left        |
    | 0 |   4    |  no  | loop exhausted -> return ""|

    Contrast, on main()'s other inputs:
      num = "35427" -> i = 4, num[4] = '7' is odd on the FIRST test, so it
                       returns substr(0, 5) = "35427" after one step.
      num = "52"    -> i = 1, '2' is even; i = 0, '5' is odd -> substr(0, 1)
                       = "5" after two steps.

    Step count behind the O(n) claim:
      "4206" is the worst case - every digit is even, so all 4 indices are
      examined once and nothing else happens. The first odd digit found ends
      the function immediately, which is why "35427" costs a single step.

    Why the FIRST odd digit from the right is the answer and not just an
    answer: a longer prefix would have to end on one of the even digits we
    already passed, which cannot be odd; and any shorter prefix is a number
    with fewer digits, hence strictly smaller.
*/
