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

/*
    ==========================================================================
    DRY RUN: s = "(1)+((2))+(((3)))"   (n = 17, answer = 3)
    ==========================================================================

      index:  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
      char:   (  1  )  +  (  (  2  )  )  +  (  (  (  3  )  )  )

    Tracked state:
      balance - how many '(' are currently open; this IS the nesting depth
                at the character being read
      res     - the largest value balance has ever reached

    Initial state: balance = 0, res = 0

    Only '(' can raise the depth, so res is updated in that branch alone.
    Digits and operators fall through both branches untouched.

    --------------------------------------------------------------------------
    | i  | char | branch      | balance | res | note                        |
    |----|------|-------------|---------|-----|-----------------------------|
    |  0 |  (   | open        |    1    |  1  | first level                 |
    |  1 |  1   | ignored     |    1    |  1  | not a paren                 |
    |  2 |  )   | close       |    0    |  1  | back to ground level        |
    |  3 |  +   | ignored     |    0    |  1  |                             |
    |  4 |  (   | open        |    1    |  1  | ties the record, no change  |
    |  5 |  (   | open        |    2    |  2  | new record                  |
    |  6 |  2   | ignored     |    2    |  2  |                             |
    |  7 |  )   | close       |    1    |  2  | res never decreases         |
    |  8 |  )   | close       |    0    |  2  |                             |
    |  9 |  +   | ignored     |    0    |  2  |                             |
    | 10 |  (   | open        |    1    |  2  |                             |
    | 11 |  (   | open        |    2    |  2  |                             |
    | 12 |  (   | open        |    3    |  3  | new record  <-- the answer  |
    | 13 |  3   | ignored     |    3    |  3  |                             |
    | 14 |  )   | close       |    2    |  3  |                             |
    | 15 |  )   | close       |    1    |  3  |                             |
    | 16 |  )   | close       |    0    |  3  | balanced, ends at zero      |

    Depth profile, drawn as balance over the scan:

      3                                       *
      2              *  *                  *  *  *
      1        *     *  *  *  *      *  *  *  *  *  *
      0  ------------------------------------------------
         (  )     (  (  )  )     (  (  (  )  )  )
         ^^^      ^^^^^^^^^^      ^^^^^^^^^^^^^^^^
         depth 1     depth 2          depth 3  <-- peak

    --------------------------------------------------------------------------
    RETURN res = 3

    Time / space:
      17 characters, 17 iterations, constant work each - O(n) time, O(1)
      space. A stack would also solve this, but its only use would be to
      report its own size, and an integer counter does that in one word.

    The subtlety:
      res is updated ONLY in the '(' branch, and only after the increment.
      Updating it in the ')' branch too would be harmless but pointless -
      depth is falling there and can never set a record. Conversely,
      checking before the increment would report 2 instead of 3, since it
      would measure the depth outside the character rather than at it.

    Note that the input is guaranteed to be a valid parentheses string, so
    balance never goes negative and finishes at exactly 0. On malformed
    input like "())(" the code would still return an answer rather than
    reporting the imbalance - detecting that would need a `balance < 0`
    check.
*/
