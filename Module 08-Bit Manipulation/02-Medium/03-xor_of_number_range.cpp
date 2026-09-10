#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given two integers l and r, find the XOR of every integer in the
//    inclusive range [l, r]
//
// Example:
// l = 2, r = 5
// 2 xor 3 xor 4 xor 5 = 0

/*
    Approach: Prefix XOR with a Period-4 Pattern

    - XORing the whole range one element at a time is O(r - l), too
      slow when the range is large.
    - Just like prefix sums let range sums be computed as a subtraction,
      prefix XORs let range XORs be computed the same way, because XOR
      is its own inverse: x ^ x = 0.
          xor(l..r) = prefixXor(r) ^ prefixXor(l - 1)
      (XORing the two prefixes cancels everything from 0 to l - 1,
      leaving only l..r.)
    - prefixXor(n) = 0 ^ 1 ^ 2 ^ ... ^ n follows a simple repeating
      pattern with period 4, which can be verified by tracing the
      running XOR for n = 0, 1, 2, 3, 4, 5, ...:
          n % 4 == 0  ->  prefixXor(n) = n
          n % 4 == 1  ->  prefixXor(n) = 1
          n % 4 == 2  ->  prefixXor(n) = n + 1
          n % 4 == 3  ->  prefixXor(n) = 0
      so no loop is required at all.

    Algorithm Steps
    ----------------
    1. Write a helper prefixXor(n) that returns 0^1^...^n in O(1)
       using the period-4 pattern above
    2. Return prefixXor(r) ^ prefixXor(l - 1)

    Time Complexity: O(1) - constant-time pattern lookup, no loop
    Space Complexity: O(1) - no extra memory used
*/
int prefixXor(int n) {
  if (n < 0) return 0;   // prefixXor of an empty range (used when l = 0)

  switch (n % 4) {
    case 0: return n;
    case 1: return 1;
    case 2: return n + 1;
    default: return 0;   // n % 4 == 3
  }
}

int xorOfRange(int l, int r) {
  return prefixXor(r) ^ prefixXor(l - 1);
}

int main() {
  int l = 2, r = 5;
  cout << "l = " << l << ", r = " << r << endl;
  cout << "XOR of range: " << xorOfRange(l, r) << endl;

  l = 4, r = 8;
  cout << "\nl = " << l << ", r = " << r << endl;
  cout << "XOR of range: " << xorOfRange(l, r) << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: l = 2, r = 5   (answer = 0)
    ======================================================================

      value        :   2     3     4     5
      binary       : 0010  0011  0100  0101
      bit position : bit3 bit2 bit1 bit0   (8=bit3, 4=bit2, 2=bit1, 1=bit0)

    Tracked state:
      n        - the argument handed to prefixXor
      n % 4    - which of the four cases of the period-4 pattern applies
      returned - the O(1) value 0^1^...^n that the switch produced

    Initial state: nothing accumulates - there is no loop, just two
                   constant-time helper calls and one XOR.

    ----------------------------------------------------------------------
    call 1: prefixXor(r) = prefixXor(5)
      guard        n = 5, not < 0 -> fall through to the switch
      branch       5 % 4 = 1      -> case 1
      formula      case 1 returns the constant 1
      returned     1 = 0001

    ----------------------------------------------------------------------
    call 2: prefixXor(l - 1) = prefixXor(1)
      guard        n = 1, not < 0 -> fall through to the switch
      branch       1 % 4 = 1      -> case 1   (same branch as above)
      formula      case 1 returns the constant 1
      returned     1 = 0001

    ----------------------------------------------------------------------
    combine
      xor          0001 ^ 0001 = 0000
      meaning      prefixXor(1) cancels 0^1 out of prefixXor(5),
                   leaving exactly 2^3^4^5

    ----------------------------------------------------------------------
    RETURN 0000 = 0

    ======================================================================
    Cross-check: brute-force XOR of the range, one element at a time
    ======================================================================

    This is NOT what the code does - it is the hand check that the
    period-4 shortcut agrees with the definition.

      acc = 0000 (0)
      ^ 2   0000 ^ 0010 = 0010   acc = 2
      ^ 3   0010 ^ 0011 = 0001   acc = 1
      ^ 4   0001 ^ 0100 = 0101   acc = 5
      ^ 5   0101 ^ 0101 = 0000   acc = 0     <-- matches the RETURN above

    And the two prefixes it was built from, also by hand:
      prefixXor(1) = 0 ^ 1                         = 0001 = 1   (matches
                                                    case 1)
      prefixXor(5) = 0 ^ 1 ^ 2 ^ 3 ^ 4 ^ 5
                   = 0001 ^ 0010 = 0011 (3)
                   ^ 0011 = 0000 (0)
                   ^ 0100 = 0100 (4)
                   ^ 0101 = 0001 (1)              = 0001 = 1   (matches
                                                    case 1)
      So prefixXor(5) ^ prefixXor(1) = 1 ^ 1 = 0. Both routes agree.

    ======================================================================
    Summary table
    ======================================================================

    | call            | n | n % 4 | case taken     | returned | binary |
    |-----------------|---|-------|----------------|----------|--------|
    | prefixXor(r)    | 5 |   1   | case 1 -> 1    |    1     |  0001  |
    | prefixXor(l-1)  | 1 |   1   | case 1 -> 1    |    1     |  0001  |
    | final XOR       | - |   -   | -              |    0     |  0000  |

    The second main() case, l = 4, r = 8, exercises two other branches:

    | call            | n | n % 4 | case taken     | returned | binary |
    |-----------------|---|-------|----------------|----------|--------|
    | prefixXor(r)    | 8 |   0   | case 0 -> n    |    8     |  1000  |
    | prefixXor(l-1)  | 3 |   3   | default -> 0   |    0     |  0000  |
    | final XOR       | - |   -   | -              |    8     |  1000  |

      brute force: 0100 ^ 0101 = 0001 (1), ^ 0110 = 0111 (7),
                   ^ 0111 = 0000 (0), ^ 1000 = 1000 (8)   <-- agrees

    Why the period-4 pattern exists:
      pair up consecutive even/odd numbers 2k and 2k+1. They differ in
      bit0 only, so (2k) ^ (2k+1) = 1. Four consecutive numbers starting
      at a multiple of 4 therefore contribute 1 ^ 1 = 0, which is why
      prefixXor resets to 0 at every n % 4 == 3 and the whole running XOR
      only ever depends on n mod 4.

    Step count backing O(1):
      2 modulo operations, 2 switch dispatches, 1 XOR - five operations
      total, and that count does not change whether the range is 2..5 or
      2..2000000000. The brute-force cross-check above needed 4 XORs for
      a 4-element range and would need 2 billion for the large one.
*/
