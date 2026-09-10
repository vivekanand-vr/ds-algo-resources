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

/*
    ==========================================================================
    DRY RUN: s = "(()())(())"   (n = 10, answer = "()()()")
    ==========================================================================

      index:   0   1   2   3   4   5   6   7   8   9
      char:    (   (   )   (   )   )   (   (   )   )
      block:   |<---- primitive 1 ----->|<- primitive 2 ->|

    Tracked state:
      balance - number of '(' currently unmatched (the nesting depth)
      res     - the output string, built one character at a time

    The rule the code applies:
      on '('  -> append it ONLY if balance was already > 0, then balance++
      on ')'  -> balance-- FIRST, then append it only if balance is still > 0

    Why that drops exactly the outermost pair of each primitive block:
      the '(' that opens a block is the one seen while balance == 0, and
      the ')' that closes it is the one that drives balance back to 0.
      Both tests fail on those two characters and on no others.

    Initial state: balance = 0, res = ""

    --------------------------------------------------------------------------
    i = 0, c = '('          <-- OPENS primitive block 1
      read     '('
      test     balance = 0, not > 0   -> do NOT append (this is outermost)
      open     balance 0 -> 1
      res      ""            (unchanged)

    --------------------------------------------------------------------------
    i = 1, c = '('
      read     '('
      test     balance = 1 > 0        -> append (inner paren)
      append   res -> "("
      open     balance 1 -> 2

    --------------------------------------------------------------------------
    i = 2, c = ')'
      read     ')'
      close    balance 2 -> 1
      test     balance = 1 > 0        -> append (inner paren)
      append   res -> "()"

    --------------------------------------------------------------------------
    i = 3, c = '('
      read     '('
      test     balance = 1 > 0        -> append
      append   res -> "()("
      open     balance 1 -> 2

    --------------------------------------------------------------------------
    i = 4, c = ')'
      read     ')'
      close    balance 2 -> 1
      test     balance = 1 > 0        -> append
      append   res -> "()()"

    --------------------------------------------------------------------------
    i = 5, c = ')'          <-- CLOSES primitive block 1
      read     ')'
      close    balance 1 -> 0
      test     balance = 0, not > 0   -> do NOT append (this is outermost)
      skip     res -> "()()"          (unchanged)

               (   (   )   (   )   )   (   (   )   )
               ^                   ^
               |                   |
               dropped at i = 0    dropped here at i = 5
               (balance was 0)     (balance fell back to 0)

               everything strictly between them survived: "()()"

    --------------------------------------------------------------------------
    i = 6, c = '('          <-- OPENS primitive block 2
      read     '('
      test     balance = 0, not > 0   -> do NOT append
      open     balance 0 -> 1
      res      "()()"        (unchanged)

    --------------------------------------------------------------------------
    i = 7, c = '('
      read     '('
      test     balance = 1 > 0        -> append
      append   res -> "()()("
      open     balance 1 -> 2

    --------------------------------------------------------------------------
    i = 8, c = ')'
      read     ')'
      close    balance 2 -> 1
      test     balance = 1 > 0        -> append
      append   res -> "()()()"

    --------------------------------------------------------------------------
    i = 9, c = ')'          <-- CLOSES primitive block 2
      read     ')'
      close    balance 1 -> 0
      test     balance = 0, not > 0   -> do NOT append
      skip     res -> "()()()"        (unchanged), loop ends

    --------------------------------------------------------------------------
    RETURN res = "()()()"

    ==========================================================================
    Summary table
    ==========================================================================

    | i | c | balance before | balance after | kept? | res      |
    |---|---|----------------|---------------|-------|----------|
    | 0 | ( |       0        |       1       |  no   | ""       |
    | 1 | ( |       1        |       2       |  yes  | "("      |
    | 2 | ) |       2        |       1       |  yes  | "()"     |
    | 3 | ( |       1        |       2       |  yes  | "()("    |
    | 4 | ) |       2        |       1       |  yes  | "()()"   |
    | 5 | ) |       1        |       0       |  no   | "()()"   |
    | 6 | ( |       0        |       1       |  no   | "()()"   |
    | 7 | ( |       1        |       2       |  yes  | "()()("  |
    | 8 | ) |       2        |       1       |  yes  | "()()()" |
    | 9 | ) |       1        |       0       |  no   | "()()()" |

    Step count behind the O(n) claim:
      10 characters, 10 loop iterations, and a constant amount of work in
      each (one comparison, one increment/decrement, at most one push_back).
      No rescanning and no stack - the single integer `balance` carries all
      the nesting information a stack would have held.

    The subtlety to watch:
      the two branches are NOT symmetric. For '(' the test comes BEFORE the
      increment; for ')' it comes AFTER the decrement. Both branches are
      really asking the same question - "is the depth outside this character
      zero?" - and getting the order wrong on either one would drop the
      inner parens instead of the outer ones.
*/
