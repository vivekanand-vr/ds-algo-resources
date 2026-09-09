#include <bits/stdc++.h>
using namespace std;

// Q: Given a Valid Parentheses String (VPS) - a string built only from
//    digits, '+', '-', '*', '/', '(' and ')' where the parentheses are
//    balanced - return its nesting depth, i.e. the maximum number of
//    parentheses that are open at the same time.
//
// Example:
// s = "1+(2*3)/(2-1)"      -> 1
// s = "(1)+((2))+(((3)))"  -> 3
// s = "()(())((()()))"     -> 3

/*
    Approach: Running balance counter

    - Every '(' opens one more level of nesting and every ')' closes the
      most recently opened one, so a simple running "balance" counter
      (net open parens so far) tracks the current depth at all times.
    - The answer is just the highest value that balance ever reaches,
      so track a running maximum alongside it. Non-parenthesis
      characters (digits, operators) don't affect balance and are
      simply skipped.

    Algorithm Steps
    ----------------
    1. Initialize balance = 0 and res = 0.
    2. For each character c in s:
         a. If c == '(': increment balance, then update
            res = max(res, balance).
         b. Else if c == ')': decrement balance.
         c. Otherwise: ignore the character.
    3. Return res.

    Time Complexity: O(n) - single pass over the string
    Space Complexity: O(1) - only two counters used
*/
int maxDepth(string s) {
  int res = 0;
  int balance = 0;

  for (char c : s) {
    if (c == '(') {
      balance++;
      res = max(res, balance);   // depth only ever grows on '('
    } else if (c == ')') {
      balance--;
    }
  }

  return res;
}

int main() {
  vector<string> tests = {"1+(2*3)/(2-1)", "(1)+((2))+(((3)))", "()(())((()()))"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: " << maxDepth(s) << endl << endl;
  }

  return 0;
}
