#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s containing only '(', ')' and '*', where '*' may be
//    treated as '(', as ')', or as an empty string, return true if s can
//    be interpreted as a valid parenthesis string.
//
// Example:
// s = "()"     -> true   (already balanced, no '*' needed)
// s = "(*)"    -> true   (treat '*' as empty: "()")
// s = "(*))"   -> true   (treat '*' as '(': "(())")
// s = "(("     -> false  (no '*' available to close either '(')

/*
    Approach: Greedy range tracking - the possible open-count as an
              interval [low, high]

    - Because '*' is ambiguous, a single running "open count" is not
      enough - the true count depends on choices not yet committed to. So
      track the RANGE of open-paren counts that are still achievable:
        low  = fewest '(' that could be currently unmatched, if every '*'
               so far that COULD help close something was used as ')'
        high = most '(' that could be currently unmatched, if every '*'
               so far was used as '('
    - '(' raises both bounds by 1 (an open paren is added no matter what).
      ')' lowers both bounds by 1 (a close paren is used no matter what).
      '*' lowers low by 1 (treat it as ')') and raises high by 1 (treat it
      as '('), because both interpretations remain possible until proven
      otherwise.
    - `high < 0` at any point means even the most generous interpretation
      of every '*' so far as '(' still leaves too many ')' - there is no
      way to recover, so fail immediately. This check must happen INSIDE
      the loop (not just at the end), because once high goes negative no
      future character can ever bring it back up - `high` only moves by
      -1, 0, or +1 per character and never jumps, so it stays negative
      forever once it crosses zero.
    - `low = max(0, low)` clamps low from below every iteration. A '*'
      can always be read as an empty string rather than as ')', so low
      can never meaningfully go negative - there is no such thing as a
      "negative number of guaranteed-open parens" to carry forward, and
      clamping keeps low a valid lower bound on an always-non-negative
      true count.
    - At the end, the string is valid if and only if 0 is INSIDE the
      final range, i.e. low == 0 - low is checked instead of high because
      low is already the SMALLEST achievable count, so if even it is not
      0 then no interpretation reaches perfect balance. (high == 0 would
      not need checking separately: since low <= high always, low == 0
      already implies high >= 0.)

    Algorithm Steps
    ----------------
    1. Initialize low = 0, high = 0.
    2. For each character c in s:
         a. '(' : low++, high++.
         b. ')' : low--, high--.
         c. '*' : low--, high++.
         d. low = max(0, low)          (a '*' can always mean empty)
         e. if high < 0, return false  (too many ')' even in the best case)
    3. Return low == 0.

    Time Complexity: O(n) - one pass, O(1) work per character.
    Space Complexity: O(1) - two running bounds.
*/
bool checkValidString(string s) {
  int low = 0;
  int high = 0;

  for (char c : s) {
    if (c == '(') {
      low++;
      high++;
    } else if (c == ')') {
      low--;
      high--;
    } else {   // '*'
      low--;   // use * as ')'
      high++;  // use * as '('
    }

    // We can choose * as empty
    low = max(0, low);

    // Too many ')' even in the best case
    if (high < 0) return false;
  }

  return low == 0;
}

int main() {
  vector<string> tests = {"()", "(*)", "(*))", "(("};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: " << (checkValidString(s) ? "true" : "false") << endl
         << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "(*))"   (n = 4, answer true)
    ==========================================================================

      index:  0  1  2  3
      char:   (  *  )  )

    Tracked state:
      low   - fewest '(' that could still be unmatched right now
      high  - most '(' that could still be unmatched right now
      [low, high] is the full range of possible "open count" values

    Initial state: low = 0, high = 0     -> range [0, 0]

    --------------------------------------------------------------------------
    i = 0, c = '('
      update       low++ -> 1,  high++ -> 1
      clamp        low = max(0, 1) = 1
      valid?       high = 1, not < 0 -> continue
      range        [1, 1]      exactly one '(' is open, no ambiguity yet

    --------------------------------------------------------------------------
    i = 1, c = '*'
      update       low-- -> 0 (treat as ')'),  high++ -> 2 (treat as '(')
      clamp        low = max(0, 0) = 0
      valid?       high = 2, not < 0 -> continue
      range        [0, 2]      three interpretations are still alive:
                     '*' as ')':  "()"  -> 0 open
                     '*' as '' :  "("   -> 1 open
                     '*' as '(':  "(("  -> 2 open

    --------------------------------------------------------------------------
    i = 2, c = ')'
      update       low-- -> -1,  high-- -> 1
      clamp        low = max(0, -1) = 0
      valid?       high = 1, not < 0 -> continue
      range        [0, 1]      the "'*' as ')'" branch would now be at -1
                   opens (invalid - clamped away), "'*' as ''" branch is at
                   0, "'*' as '('" branch is at 1

    --------------------------------------------------------------------------
    i = 3, c = ')'
      update       low-- -> -1,  high-- -> 0
      clamp        low = max(0, -1) = 0
      valid?       high = 0, not < 0 -> continue
      range        [0, 0]      only the "'*' as '('" branch survives, and
                   it lands at exactly 0 open parens: "(())" fully balanced

    --------------------------------------------------------------------------
    RETURN low == 0 -> true

      the surviving interpretation is '*' -> '(' , giving "(())", which is
      a valid, fully balanced string.

    ==========================================================================
    Summary table
    ==========================================================================

    | i | c | low before | high before | low after | high after | high<0? | range  |
    |---|---|------------|-------------|-----------|------------|---------|--------|
    | 0 | ( |     0      |      0      |     1     |     1      |   no    | [1,1]  |
    | 1 | * |     1      |      1      |     0     |     2      |   no    | [0,2]  |
    | 2 | ) |     0      |      2      |     0     |     1      |   no    | [0,1]  |
    | 3 | ) |     0      |      1      |     0     |     0      |   no    | [0,0]  |

    ==========================================================================
    Why "((" fails: high goes negative and can never recover
    ==========================================================================

      s = "((" is unbalanced on its own (no '*' at all to help), but trace
      a longer related string to show the early-exit in action:
      s = "))(" :

      i=0, ')': low=-1->clamp 0, high=-1        high < 0 -> RETURN false
                immediately, before even reading the '(' at i=2.

      This is why the `if (high < 0) return false` check sits INSIDE the
      loop: two ')' with no '(' or '*' before them can never be fixed by
      anything appearing LATER in the string - high only changes by at
      most +1 per subsequent character, so once negative it would take
      more characters than remain to claw back to 0, and by construction
      it never will, since the check fires the instant it dips below zero.

    ==========================================================================
    Notes
    ==========================================================================

    Why checking low == 0 alone is sufficient at the end:
      the invariant low <= high holds throughout (every branch only ever
      moves low by at most as much as high in the "closing" direction).
      So if low == 0 then high >= 0 automatically, meaning 0 is within the
      achievable range - some interpretation balances exactly. There is
      no need for a separate `high == 0` check.

    Why the clamp uses 0 and not some other bound:
      low represents a count of unmatched '(' that must be non-negative
      in any REAL interpretation - a string cannot have "negative open
      parens". When the pessimistic tracking (treating every ambiguous
      '*' as ')') would drive it below zero, that particular pessimistic
      branch is simply infeasible and drops out of consideration; 0 is
      the tightest still-meaningful lower bound to carry forward.

    Alternative approaches worth knowing:
      a stack-based solution tracks indices of unmatched '(' and indices
      of '*' separately, then greedily matches trailing ')' against the
      most recent open '(' or, failing that, against a '*'; it is O(n)
      time and O(n) space. This low/high range technique achieves the
      same O(n) time in O(1) space, which is why it is the one usually
      preferred.
*/
