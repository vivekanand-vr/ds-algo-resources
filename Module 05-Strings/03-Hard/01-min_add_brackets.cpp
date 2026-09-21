#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s of '(' and ')', return the minimum number of parentheses
//    to insert anywhere so that the string becomes valid

// Examples:
//   "())"  -> 1   (one extra ')' at the end needs a '(' before it)
//   "((("  -> 3   (three openers never get closed)
//   ")("   -> 2   (one unmatchable ')' and one unclosed '(')

/*
    Approach: Single-pass counter sweep (greedy balance)
    - open  counts '(' seen so far that are still UNMATCHED
    - close counts ')' that arrived when no unmatched '(' was available,
      so they can never be matched by anything to their right
    - On '(' we simply park it in open, hoping a later ')' claims it
    - On ')' we cancel one parked '(' if there is one (a match costs nothing);
      otherwise this ')' is permanently unmatchable and goes into close
    - Matching a ')' with a pending '(' is always safe: any valid pairing can
      be rewritten into this one, so the greedy never loses a better option
    - The answer is open + close: every leftover unmatched '(' needs a ')'
      inserted after it, and every unmatchable ')' needs a '(' inserted
      before it - one insertion each

    Algorithm Steps
    ----------------
    1. Initialize open = 0 and close = 0
    2. Scan the string left to right, one character at a time
    3. If the character is '(', increment open
    4. If the character is ')':
         a. if open > 0, decrement open (this ')' matches a pending '(')
         b. else increment close (this ')' can never be matched)
    5. After the scan, return open + close

    Time Complexity: O(n) - one pass, O(1) work per character
    Space Complexity: O(1) - only two integer counters, no stack needed
*/
int minAddToMakeValid(string s) {
    int n = s.length();          // length of the string (unused by the logic)
    int open = 0, close = 0;     // open = pending '(' ; close = unmatchable ')'

    for(char c: s){
        if(c == '(') open++;     // park this '(' and wait for a closer
        else{
            // a ')' first tries to consume a pending '('
            if(open > 0) open--;
            // no pending '(' to its left, so this ')' is hopeless
            else close++;
        }
    }

    // each leftover on either side costs exactly one insertion
    return open + close;
}

int main() {
    vector<string> tests = {"())", "(((", ")(", "()", "()))((", "", "()()"};

    for (const string& t : tests) {
        cout << "\"" << t << "\"" << " -> " << minAddToMakeValid(t) << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN 1: s = ")("     (n = 2, answer = 2)
    ==========================================================================

      index:   0   1
       char:   )   (

    Tracked state:
      open  - '(' seen so far that are still waiting for a ')'
      close - ')' already proven unmatchable (nothing later can help them)

    Initial state: open = 0, close = 0

    --------------------------------------------------------------------------
    i = 0, c = ')'
      branch     else-branch (it is a closer)
      check      open > 0 ?   open = 0   -> NO
      action     close++                 -> open = 0, close = 1
      meaning    there is no '(' to the left of this ')', and a '(' to its
                 RIGHT could never match it, so it is doomed right now

    --------------------------------------------------------------------------
    i = 1, c = '('
      branch     if-branch (it is an opener)
      action     open++                  -> open = 1, close = 1
      meaning    parked, hoping for a later ')' - but the string ends here

    --------------------------------------------------------------------------
    End of scan: open = 1, close = 1
    RETURN open + close = 1 + 1 = 2

      fix:   (  )  (  )        insert '(' before index 0 and ')' after index 1
             ^           ^
           added       added

    ==========================================================================
    DRY RUN 2: s = "()))(("   (n = 6, answer = 4)
    ==========================================================================

      index:   0   1   2   3   4   5
       char:   (   )   )   )   (   (

    --------------------------------------------------------------------------
    i = 0, c = '('   open++               -> open = 1, close = 0
    i = 1, c = ')'   open > 0, open--     -> open = 0, close = 0  (0-1 matched)
    i = 2, c = ')'   open = 0, close++    -> open = 0, close = 1  (doomed)
    i = 3, c = ')'   open = 0, close++    -> open = 0, close = 2  (doomed)
    i = 4, c = '('   open++               -> open = 1, close = 2
    i = 5, c = '('   open++               -> open = 2, close = 2
    --------------------------------------------------------------------------
    RETURN open + close = 2 + 2 = 4

      the only matched pair is indices 0-1; the two ')' at 2,3 each need a '('
      inserted before them, and the two '(' at 4,5 each need a ')' after them

    ==========================================================================
    Summary table   (s = "()))((")
    ==========================================================================

    | i | char | branch taken        | open | close |
    |---|------|---------------------|------|-------|
    | - |  -   | initial             |  0   |   0   |
    | 0 |  (   | open++              |  1   |   0   |
    | 1 |  )   | open > 0 -> open--  |  0   |   0   |
    | 2 |  )   | open = 0 -> close++ |  0   |   1   |
    | 3 |  )   | open = 0 -> close++ |  0   |   2   |
    | 4 |  (   | open++              |  1   |   2   |
    | 5 |  (   | open++              |  2   |   2   |

    Why one left-to-right pass is enough:
      when a ')' arrives and open = 0, every '(' to its left has already been
      consumed by an earlier ')'. A '(' appearing LATER cannot help it either,
      because a match requires the '(' to sit before the ')'. So the only
      repair is inserting a '(' to its left - a decision that can be taken
      immediately and never has to be revisited. That is what makes the greedy
      safe: no future character can change the verdict on a doomed ')'.

    Why open and close never cancel each other:
      a ')' is only counted into close while open = 0, so at that moment no
      unmatched '(' exists. Any '(' still sitting in open at the end must have
      appeared AFTER every counted close. An unmatched '(' at position p and
      an unmatchable ')' at position q therefore always satisfy q < p, and
      "...)...(..." is the wrong order to form a pair. Hence the two counts
      are independent and the answer is their plain sum, not a difference.

    Dead state:
      int n = s.length(); is assigned but never read - the range-based for
      loop walks the string directly. Harmless, but it is leftover scaffolding
      from an index-based version of this loop.

    Relation to the stack solution:
      the classic approach pushes '(' on a stack and pops on ')'. Since every
      stack entry would be the same symbol, only its SIZE matters - which is
      exactly what open is. That is why this runs in O(1) space.
*/
