#include <bits/stdc++.h>
using namespace std;

// Q: Given a valid parentheses string s formed by concatenating one or more
//    "primitive" valid parentheses strings (a primitive string is non-empty
//    and cannot be split into two non-empty valid parentheses strings),
//    remove the outermost parentheses of every primitive substring and
//    return the resulting string.
//
// Example:
// s = "(()())(())"          -> "()()()"
// s = "(()())(())(()(()))"  -> "()()()()(())"
// s = "()()"                -> ""

/*
    Approach: Balance Counter

    - A primitive substring starts with the '(' that brings the running
      balance from 0 to 1, and ends with the ')' that brings it back from
      1 to 0. Those two parentheses are exactly the "outermost" pair we
      need to drop for that primitive block.
    - So track a running balance while scanning left to right:
        - Every '(' increases balance by 1, every ')' decreases it by 1.
        - A character belongs to the OUTPUT only if it is not one of
          those two boundary characters, i.e. only if balance is > 0
          strictly inside the block (not the opening char that made it 1,
          not the closing char that brought it back to 0).
    - Concretely: for '(', append it only if balance was already > 0
      before incrementing; for ')', decrement first, then append only if
      balance is still > 0 after decrementing.

    Algorithm Steps
    ----------------
    1. Initialize balance = 0 and an empty result string.
    2. For each character c in s:
         a. If c == '(':
              - if balance > 0, append c to result (it's an inner paren)
              - balance++
         b. Else (c == ')'):
              - balance--
              - if balance > 0, append c to result (it's an inner paren)
    3. Return result.

    Time Complexity: O(n) - single pass over the string
    Space Complexity: O(n) for the result string (O(1) extra apart from output)
*/
string removeOuterParentheses(string s) {
  string res;
  res.reserve(s.size());

  int balance = 0;
  for (char c : s) {
    if (c == '(') {
      if (balance > 0) res.push_back(c);   // inner '(' of the current block
      balance++;
    } else {
      balance--;
      if (balance > 0) res.push_back(c);   // inner ')' of the current block
    }
  }

  return res;
}

int main() {
  vector<string> tests = {"(()())(())", "(()())(())(()(()))", "()()"};

  for (const string& s : tests) {
    cout << "Input:  " << s << endl;
    cout << "Output: " << removeOuterParentheses(s) << endl << endl;
  }

  return 0;
}
