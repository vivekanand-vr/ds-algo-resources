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

/*
    ==========================================================================
    DRY RUN 1: s = "   -042"   (n = 7, answer = -42)
    ==========================================================================

      index:   0    1    2    3    4    5    6
      char:   ' '  ' '  ' '  '-'  '0'  '4'  '2'

    Tracked state:
      i    - the scan position, moving strictly left to right
      sign - +1 or -1, fixed once and never revisited
      num  - the magnitude so far, accumulated in a LONG LONG so the
             overflow test can be made before any int would wrap

    Initial state: i = 0, sign = +1, num = 0

    --------------------------------------------------------------------------
    PHASE 1 - skip leading whitespace   while (i < n && s[i] == ' ') i++
      step 1: s[0] = ' '  -> i -> 1
      step 2: s[1] = ' '  -> i -> 2
      step 3: s[2] = ' '  -> i -> 3
      step 4: s[3] = '-'  -> not a space, stop.   i = 3

    --------------------------------------------------------------------------
    PHASE 2 - optional single sign
      guard    i = 3 < n = 7, so s[3] is safe to read
      read     s[3] = '-'  -> sign = -1
      advance  i -> 4
      (only ONE sign is consumed; a second '-' would fall through to the
       digit loop, fail the isdigit test immediately, and yield 0)

    --------------------------------------------------------------------------
    PHASE 3 - accumulate digits
      i = 4, s[i] = '0'
        digit      0
        accumulate num = 0 * 10 + 0 = 0
        clamp      sign is -1, is -0 < INT_MIN ?  no
        advance    i -> 5
        (a leading zero contributes nothing - no special case needed)

      i = 5, s[i] = '4'
        digit      4
        accumulate num = 0 * 10 + 4 = 4
        clamp      is -4 < -2147483648 ?  no
        advance    i -> 6

      i = 6, s[i] = '2'
        digit      2
        accumulate num = 4 * 10 + 2 = 42
        clamp      is -42 < -2147483648 ?  no
        advance    i -> 7   ->   i == n, loop ends

    --------------------------------------------------------------------------
    RETURN (int)(sign * num) = (int)(-1 * 42) = -42

    | phase | i in | i out | state after                    |
    |-------|------|-------|--------------------------------|
    |   1   |  0   |   3   | three spaces consumed          |
    |   2   |  3   |   4   | sign = -1                      |
    |   3   |  4   |   7   | num = 42 (via 0 -> 4 -> 42)    |
    | return|      |       | -42                            |

    ==========================================================================
    DRY RUN 2: s = "-91283472332"   (answer = INT_MIN = -2147483648)
    ==========================================================================

    This is the case the overflow clamp exists for. No whitespace; sign is
    read at i = 0, so digits start at i = 1.

    The clamp fires the moment -num drops below INT_MIN, i.e. the moment
    num exceeds 2147483648:

    | i  | digit | num after            | -num < INT_MIN ? |
    |----|-------|----------------------|------------------|
    |  1 |   9   |            9         | no               |
    |  2 |   1   |           91         | no               |
    |  3 |   2   |          912         | no               |
    |  4 |   8   |         9128         | no               |
    |  5 |   3   |        91283         | no               |
    |  6 |   4   |       912834         | no               |
    |  7 |   7   |      9128347         | no               |
    |  8 |   2   |     91283472         | no               |
    |  9 |   3   |    912834723         | no               |
    | 10 |   3   |   9128347233         | YES -> return    |

      at i = 10:  num = 9128347233, so -num = -9128347233
                  -9128347233 < -2147483648  ->  RETURN INT_MIN

    The remaining digit at i = 11 is never even read - the function returns
    from inside the loop. That early exit is also what keeps `num` itself
    bounded: it can never grow past ~2.1e10 before the return fires, so the
    long long cannot overflow no matter how long the digit string is.

    Why the clamp must carry the SIGN:
      clamping the magnitude first and negating afterwards is off by one at
      the negative end. For "-2147483648" the magnitude is 2147483648,
      which exceeds INT_MAX (2147483647); clamping the magnitude to INT_MAX
      and then negating gives -2147483647 - one short of the correct
      answer. Testing the signed value against INT_MIN directly, as this
      code does, returns exactly -2147483648.

    Time / space:
      one left-to-right pass, O(n) time, O(1) space. Every read of s[i] is
      guarded by i < n, including the sign check - a string of nothing but
      spaces leaves i == n and must not be dereferenced.
*/
