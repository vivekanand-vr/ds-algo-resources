#include <bits/stdc++.h>
using namespace std;

// Q: Implement myAtoi(string s), which converts a string to a 32-bit
//    signed integer, following these rules in order:
//      1. Skip any leading whitespace.
//      2. Read an optional single '+' or '-' sign.
//      3. Read in digits until the first non-digit character (or the end
//         of the string), ignoring everything after it.
//      4. If no digits were read, return 0.
//      5. Clamp the result to the 32-bit signed range:
//         values above INT_MAX become INT_MAX (2147483647), values below
//         INT_MIN become INT_MIN (-2147483648).
//
// Example:
// s = "42"             -> 42
// s = "   -042"        -> -42
// s = "1337c0d3"       -> 1337     (stops at 'c')
// s = "words and 987"  -> 0        (first non-space char is not a digit/sign)
// s = "-91283472332"   -> -2147483648  (clamped to INT_MIN)

/*
    Approach: Guarded linear parse with an overflow check inside the loop

    - The parse itself is a single left-to-right walk through four
      phases: whitespace, an optional sign, a run of digits, then stop.
      Nothing needs to be revisited, so one index `i` and no extra
      storage is enough.
    - Overflow is the only subtle part. Two bugs are easy to hit here:
        a) Accumulating into a plain `int` overflows (undefined
           behaviour) before you get a chance to notice.
        b) Clamping the MAGNITUDE first and applying the sign afterward
           is off by one at the negative end: "-2147483648" has
           magnitude 2147483648 > INT_MAX, so clamping the magnitude to
           INT_MAX and negating yields -2147483647, one short of the
           correct INT_MIN.
      Both are avoided by accumulating into a `long long` and testing
      the SIGNED value against INT_MAX / INT_MIN inside the digit loop,
      returning the clamp immediately. Returning early also keeps the
      accumulator itself bounded, so an input of a thousand digits can
      never overflow the `long long` either.
    - Every read of s[i] is guarded by `i < n`, including the sign check
      (a string of nothing but spaces must not read past the end).

    Algorithm Steps
    ----------------
    1. n = s.length(), i = 0.
    2. While i < n and s[i] == ' ': i++            (skip whitespace)
    3. sign = +1. If i < n and s[i] is '+' or '-', set sign accordingly
       and i++.
    4. num = 0 (as long long). While i < n and s[i] is a digit:
         a. num = num * 10 + (s[i] - '0')
         b. If sign is positive and num > INT_MAX, return INT_MAX.
         c. If sign is negative and -num < INT_MIN, return INT_MIN.
         d. i++.
    5. Return (int)(sign * num). If no digits were read, num is 0, which
       is exactly the required "no conversion" answer.

    Time Complexity: O(n) - one pass over the string
    Space Complexity: O(1) - only a few scalars
*/
int myAtoi(string s) {
  int n = s.length();
  int i = 0;

  while (i < n && s[i] == ' ') i++;      // 1. skip leading whitespace

  int sign = 1;                          // 2. optional single sign
  if (i < n && (s[i] == '+' || s[i] == '-')) {
    if (s[i] == '-') sign = -1;
    i++;
  }

  long long num = 0;                     // 3. accumulate digits
  while (i < n && s[i] >= '0' && s[i] <= '9') {
    num = num * 10 + (s[i] - '0');

    // Clamp with the sign applied, so INT_MIN is reachable exactly
    if (sign == 1 && num > INT_MAX) return INT_MAX;
    if (sign == -1 && -num < INT_MIN) return INT_MIN;

    i++;
  }

  return (int)(sign * num);              // num stays 0 when no digits were read
}

int main() {
  vector<string> tests = {"42", "   -042", "1337c0d3", "words and 987", "-91283472332"};

  for (const string& s : tests) {
    cout << "Input:  \"" << s << "\"" << endl;
    cout << "Output: " << myAtoi(s) << endl << endl;
  }

  return 0;
}
