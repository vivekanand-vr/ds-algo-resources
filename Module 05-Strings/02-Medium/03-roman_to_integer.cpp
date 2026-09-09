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
