#include <bits/stdc++.h>
using namespace std;

// Q: Given a string s representing a Roman numeral, convert it to an
//    integer. Roman numerals use I=1, V=5, X=10, L=50, C=100, D=500,
//    M=1000, where a smaller-value symbol placed before a larger one
//    is subtracted (e.g. IV = 4) rather than added.
//
// Example:
// s = "III"    -> 3
// s = "LVIII"  -> 58   (L=50, V=5, III=3)
// s = "MCMXCIV" -> 1994 (M=1000, CM=900, XC=90, IV=4)

/*
    Approach: Fixed value table + look-ahead comparison

    - The original version used an unordered_map<char,int>, which hashes
      on every lookup and - because operator[] auto-inserts on a miss -
      silently created a spurious '\0' -> 0 entry every time it peeked
      one character past the end via s[i+1] (i == n-1). A plain
      fixed-size array indexed directly by the character's ASCII value
      gives O(1) access with no hashing and no insertion side effects.
    - The core rule: scan left to right: if the current symbol's value
      is smaller than the NEXT symbol's value, it is being used as a
      subtractive prefix (like the 'I' in "IV"), so subtract it; it will
      be counted properly for its face value if not, so add it in that
      case. The last character never has a "next" symbol, so it is
      always added - guarded explicitly with an `i + 1 < n` bounds check
      instead of relying on reading past the string.

    Algorithm Steps
    ----------------
    1. Build value[128] with the seven known symbol values (all other
       entries default to 0).
    2. Initialize res = 0.
    3. For i from 0 to n - 1:
         a. If i + 1 < n and value[s[i]] < value[s[i+1]], subtract
            value[s[i]] from res (subtractive case, e.g. 'I' before 'V').
         b. Otherwise, add value[s[i]] to res.
    4. Return res.

    Time Complexity: O(n) - single pass, O(1) work per character
    Space Complexity: O(1) - a fixed 128-entry table regardless of input
*/
int romanToInt(string s) {
  vector<int> value(128, 0);
  value['I'] = 1;
  value['V'] = 5;
  value['X'] = 10;
  value['L'] = 50;
  value['C'] = 100;
  value['D'] = 500;
  value['M'] = 1000;

  int n = (int)s.length();
  int res = 0;

  for (int i = 0; i < n; ++i) {
    if (i + 1 < n && value[s[i]] < value[s[i + 1]]) {
      res -= value[s[i]];    // subtractive prefix, e.g. the 'I' in "IV"
    } else {
      res += value[s[i]];    // normal face-value addition
    }
  }

  return res;
}

int main() {
  vector<string> tests = {"III", "LVIII", "MCMXCIV"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: " << romanToInt(s) << endl << endl;
  }

  return 0;
}

/*
    ==========================================================================
    DRY RUN: s = "MCMXCIV"   (n = 7, answer = 1994)
    ==========================================================================

      index:   0     1     2     3     4     5     6
      char:    M     C     M     X     C     I     V
      value: 1000   100  1000    10   100     1     5

    This input is the ideal one to trace: it contains THREE subtractive
    pairs (CM = 900, XC = 90, IV = 4) and one plain additive symbol.

    Tracked state:
      value[] - a fixed 128-entry table, indexed straight by the character's
                ASCII code: value['I']=1, ['V']=5, ['X']=10, ['L']=50,
                ['C']=100, ['D']=500, ['M']=1000, everything else 0
      res     - the running total

    The rule at each index i:
      if i + 1 < n AND value[s[i]] < value[s[i+1]]   -> res -= value[s[i]]
      otherwise                                       -> res += value[s[i]]

    Initial state: res = 0

    --------------------------------------------------------------------------
    i = 0, s[i] = 'M'
      look up      value['M'] = 1000,  value[s[1]='C'] = 100
      compare      1000 < 100 ?  NO
      branch       add          res = 0 + 1000
      res          1000
    --------------------------------------------------------------------------
    i = 1, s[i] = 'C'      <-- subtractive: the C of "CM"
      look up      value['C'] = 100,   value[s[2]='M'] = 1000
      compare      100 < 1000 ?  YES
      branch       subtract     res = 1000 - 100
      res          900

                   M   C   M   X   C   I   V
                       ^---^
                       "CM" = 1000 - 100 = 900, spread across i=1 and i=2:
                       the C is taken off here, the M is added next.
    --------------------------------------------------------------------------
    i = 2, s[i] = 'M'
      look up      value['M'] = 1000,  value[s[3]='X'] = 10
      compare      1000 < 10 ?  NO
      branch       add          res = 900 + 1000
      res          1900         ("MCM" = 1900 so far)
    --------------------------------------------------------------------------
    i = 3, s[i] = 'X'      <-- subtractive: the X of "XC"
      look up      value['X'] = 10,    value[s[4]='C'] = 100
      compare      10 < 100 ?  YES
      branch       subtract     res = 1900 - 10
      res          1890
    --------------------------------------------------------------------------
    i = 4, s[i] = 'C'
      look up      value['C'] = 100,   value[s[5]='I'] = 1
      compare      100 < 1 ?  NO
      branch       add          res = 1890 + 100
      res          1990         ("MCMXC" = 1990)
    --------------------------------------------------------------------------
    i = 5, s[i] = 'I'      <-- subtractive: the I of "IV"
      look up      value['I'] = 1,     value[s[6]='V'] = 5
      compare      1 < 5 ?  YES
      branch       subtract     res = 1990 - 1
      res          1989
    --------------------------------------------------------------------------
    i = 6, s[i] = 'V'      <-- LAST character, the guard does the work
      guard        i + 1 = 7, and 7 < n = 7 is FALSE
                   so the look-ahead is short-circuited and never evaluated
      branch       add          res = 1989 + 5
      res          1994

                   Without the `i + 1 < n` guard this would read s[7].
                   std::string does return '\0' there rather than crashing,
                   and value['\0'] = 0 would make the comparison false and
                   still take the add branch - but relying on that is a
                   trap, and with an unordered_map it would also silently
                   insert a spurious '\0' key on every call.
    --------------------------------------------------------------------------
    RETURN res = 1994

    ==========================================================================
    Summary table
    ==========================================================================

    | i | s[i] | value | next | next value | subtract? | res  |
    |---|------|-------|------|------------|-----------|------|
    | 0 |  M   | 1000  |  C   |    100     |    no     | 1000 |
    | 1 |  C   |  100  |  M   |   1000     |   YES     |  900 |
    | 2 |  M   | 1000  |  X   |     10     |    no     | 1900 |
    | 3 |  X   |   10  |  C   |    100     |   YES     | 1890 |
    | 4 |  C   |  100  |  I   |      1     |    no     | 1990 |
    | 5 |  I   |    1  |  V   |      5     |   YES     | 1989 |
    | 6 |  V   |    5  |  -   | (guarded)  |    no     | 1994 |

    Cross-check by grouping:  M + CM + XC + IV
                            = 1000 + 900 + 90 + 4 = 1994   matches.

    Time / space:
      one pass, n = 7 iterations, two O(1) array reads each - O(n) time and
      O(1) space (the 128-entry table is a fixed size, independent of the
      input). A plain array beats unordered_map here: no hashing, and no
      accidental insertion on a miss.

    Why one look-ahead is enough:
      Roman numerals only ever subtract a SINGLE smaller symbol placed
      directly before a larger one, and never chain (there is no "IIX").
      So the decision at index i depends on index i+1 alone - no
      backtracking, no multi-character lookahead, and the scan stays
      strictly linear.
*/
