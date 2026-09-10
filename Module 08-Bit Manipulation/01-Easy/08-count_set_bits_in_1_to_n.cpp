#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Count total set bits for all numbers from 1 to n (both inclusive)
//
// Example 1:
// n = 4
// Numbers: 1 (001), 2 (010), 3 (011), 4 (100)
// Set bits count: 1 + 1 + 2 + 1 = 5
//
// Example 2:
// n = 11
// Set bits count: 20

/*
    Approach: Bit Manipulation & Pattern Analysis (Recursive)

    - A naive approach of counting set bits for each number from 1 to n takes
   O(n log n), which is slow for large n (e.g. n = 10^9).
    - Instead, we observe the bit patterns in numbers from 0 to n.

    - Let 2^x be the largest power of 2 less than or equal to n.
      We can break the range [1, n] into three main parts:

      1. Set bits in range [0, 2^x - 1]:
         - For all numbers from 0 to (2^x - 1), every bit position from 0 to (x
   - 1) has an equal distribution of 0s and 1s.
         - Each bit position contains 1 exactly 2^(x - 1) times.
         - Total set bits in this range = x * 2^(x - 1).

      2. MSB (Most Significant Bit at 2^x) contribution in range [2^x, n]:
         - The x-th bit (MSB) is set to 1 for every number from 2^x to n.
         - Total MSBs set = n - 2^x + 1.

      3. Remaining lower bits contribution in range [2^x, n]:
         - After removing the MSB from numbers in [2^x, n], the lower bits
           repeat the exact same sequence of bit patterns as numbers in [0, n -
   2^x].
         - Therefore, total set bits in these lower bits = countSetBitsIn1ToN(n
   - 2^x).

    Formula:
    totalSetBits(n) = (x * 2^(x - 1)) + (n - 2^x + 1) + totalSetBits(n - 2^x)
    where 2^x <= n.

    Algorithm Steps
    ----------------
    1. Base case: If n <= 0, return 0.
    2. Find x such that 2^x <= n (largest power of 2 <= n).
    3. Calculate:
         a. bitsTill2x = (x > 0) ? x * 2^(x - 1) : 0
         b. msbFrom2xToN = n - 2^x + 1
         c. rest = countSetBitsIn1ToN(n - 2^x)
    4. Return bitsTill2x + msbFrom2xToN + rest

    Time Complexity: O(log n) - reducing n by its most significant bit power in
   each step Space Complexity: O(log n) - due to recursion stack depth
*/

// Helper function to find the largest power of 2 (x) such that (1 << x) <= n
int findLargestPowerOf2(int n) {
  int x = 0;
  while ((n >> (x + 1)) > 0) {
    x++;
  }
  return x;
}

long long countSetBitsIn1ToN(int n) {
  if (n <= 0) return 0;

  int x = findLargestPowerOf2(n);

  // Guard (x > 0) prevents (1 << -1) undefined behavior when x = 0 (n = 1).
  // 1LL shift and long long avoid integer overflow for large n (e.g. n = 10^9).
  long long bitsTillHighestPower = (x > 0) ? ((long long)x * (1LL << (x - 1))) : 0LL;
  long long msbFromHighestPowerToN = (long long)n - (1LL << x) + 1;
  long long remainingBits = countSetBitsIn1ToN(n - (1 << x));

  return bitsTillHighestPower + msbFromHighestPowerToN + remainingBits;
}

int main() {
  int n = 1;
  cout << "n = " << n << endl;
  cout << "Total set bits from 1 to " << n << ": " << countSetBitsIn1ToN(n)
       << endl;

  n = 4;
  cout << "\nn = " << n << endl;
  cout << "Total set bits from 1 to " << n << ": " << countSetBitsIn1ToN(n)
       << endl;

  n = 11;
  cout << "\nn = " << n << endl;
  cout << "Total set bits from 1 to " << n << ": " << countSetBitsIn1ToN(n)
       << endl;

  n = 16;
  cout << "\nn = " << n << endl;
  cout << "Total set bits from 1 to " << n << ": " << countSetBitsIn1ToN(n)
       << endl;

  n = 1000000000;
  cout << "\nn = " << n << endl;
  cout << "Total set bits from 1 to " << n << ": " << countSetBitsIn1ToN(n)
       << endl;

  return 0;
}

// Bit table walkthrough (n = 11):
//
//   number   binary   set bits
//     1       0001        1
//     2       0010        1
//     3       0011        2
//     4       0100        1
//     5       0101        2
//     6       0110        2
//     7       0111        3
//     8       1000        1
//     9       1001        2
//    10       1010        2
//    11       1011        3
//                        ----
//                total =  20
//
// Recursive formula breakdown for n = 11 (largest power of 2, 2^x <= 11, is 2^3 = 8):
//   1. bitsTill2x  = x * 2^(x-1) = 3 * 2^2         = 12   (bits set across [0, 7])
//   2. msbFrom2xToN = n - 2^x + 1 = 11 - 8 + 1      =  4   (bit 3 set for 8,9,10,11)
//   3. rest = countSetBitsIn1ToN(n - 2^x) = countSetBitsIn1ToN(3)
//        -> for n = 3: x = 1, bitsTill2x = 1*2^0 = 1, msbFrom2xToN = 3-2+1 = 2,
//           rest = countSetBitsIn1ToN(1) = 1  =>  total = 1 + 2 + 1 = 4
//   total(11) = 12 + 4 + 4 = 20

/*
    ======================================================================
    DRY RUN: n = 11   (answer = 20)
    ======================================================================

      n = 11    binary:  1 0 1 1
                bit:     3 2 1 0
                         ^
                         highest set bit is bit 3, so 2^x = 8 and x = 3

    The bit table above (before this block) shows WHAT the answer is by
    adding up 11 rows. This block shows HOW the code gets there without
    touching those rows at all: it peels off the highest set bit of n,
    accounts for it in closed form, and recurses on the leftover
    low bits. Peeling the bits of 1011 gives the call chain

          f(11)  ->  f(3)  ->  f(1)  ->  f(0)
          1011       0011      0001      0000
           ^peel      ^peel     ^peel
           bit 3      bit 1     bit 0

    so the depth is the number of SET bits of n, at most log n calls.

    Tracked state (per call):
      x        - largest exponent with 2^x <= n, found by shifting
      bitsTill - x * 2^(x-1), all set bits in the whole range [0, 2^x -1]
      msbPart  - n - 2^x + 1, the count of numbers in [2^x, n], each of
                 which carries the peeled MSB exactly once
      rest     - recursive result for n - 2^x, the low bits of that same
                 upper stretch

    ----------------------------------------------------------------------
    CALL f(11)
      base case?   11 <= 0 ?  no

      findLargestPowerOf2(11)   -- inner shift loop
        x = 0:   n >> 1  =  0 1 0 1   ( 5) > 0  -> x = 1
        x = 1:   n >> 2  =  0 0 1 0   ( 2) > 0  -> x = 2
        x = 2:   n >> 3  =  0 0 0 1   ( 1) > 0  -> x = 3
        x = 3:   n >> 4  =  0 0 0 0   ( 0) not > 0  -> stop
        returns x = 3            (2^3 = 1 0 0 0 = 8 <= 11 < 16)

      step 1  bitsTill = x * (1 << (x-1)) = 3 * (1 << 2) = 3 * 4 = 12
              this is every set bit in 0000..0111, i.e. the numbers
              0 through 7: three bit columns, each holding a 1 in
              exactly half of the 8 numbers -> 3 * 4 = 12

      step 2  msbPart = n - (1 << x) + 1 = 11 - 8 + 1 = 4
              the four numbers 1000, 1001, 1010, 1011 (8..11) each carry
              bit 3, and nothing else in [1, 11] does

      step 3  rest = f(n - (1 << x)) = f(11 - 8) = f(3)
              strip bit 3 off those same four numbers and what is left
              is 000, 001, 010, 011 - exactly the range [0, 3]

      (recursion suspends here, waiting on f(3))

    ----------------------------------------------------------------------
    CALL f(3)
      base case?   3 <= 0 ?  no

      findLargestPowerOf2(3)
        x = 0:   n >> 1  =  0 0 0 1   ( 1) > 0  -> x = 1
        x = 1:   n >> 2  =  0 0 0 0   ( 0) not > 0  -> stop
        returns x = 1            (2^1 = 0 0 1 0 = 2 <= 3 < 4)

      step 1  bitsTill = 1 * (1 << 0) = 1 * 1 = 1
              set bits across 00..01 (the numbers 0 and 1): just one

      step 2  msbPart = 3 - 2 + 1 = 2
              the two numbers 10, 11 (2 and 3) each carry bit 1

      step 3  rest = f(3 - 2) = f(1)

    ----------------------------------------------------------------------
    CALL f(1)
      base case?   1 <= 0 ?  no

      findLargestPowerOf2(1)
        x = 0:   n >> 1  =  0 0 0 0   ( 0) not > 0  -> stop immediately
        returns x = 0            (2^0 = 1 <= 1 < 2)

      step 1  bitsTill = (x > 0) ? x * (1LL << (x - 1)) : 0 = 0
              x is 0 here, so the condition (x > 0) evaluates to false,
              avoiding a negative shift count (1 << -1) which is Undefined
              Behavior in C++.
              Conceptually it is right - the range [0, 0] holds no set
              bits at all.

      step 2  msbPart = 1 - 1 + 1 = 1
              the single number 1 (0001) carries bit 0

      step 3  rest = f(1 - 1) = f(0)

    ----------------------------------------------------------------------
    CALL f(0)
      base case?   0 <= 0 ->  YES
      RETURN 0                  the recursion bottoms out

    ----------------------------------------------------------------------
    Unwinding - each suspended call now adds its three parts

      f(1)  =  bitsTill + msbPart + rest  =  0 +  1 + 0  =  1
      f(3)  =  bitsTill + msbPart + rest  =  1 +  2 + 1  =  4
      f(11) =  bitsTill + msbPart + rest  = 12 +  4 + 4  = 20

    RETURN 20       main prints "Total set bits from 1 to 11: 20"
                    and it matches the 1 + 1 + 2 + ... + 3 hand sum in
                    the bit table above

    ======================================================================
    Summary table (the call stack for n = 11)
    ======================================================================

    | call  | n binary | x | 2^x | 2^x binary | bitsTill | msbPart | ret |
    |-------|----------|---|-----|------------|----------|---------|-----|
    | f(11) |   1011   | 3 |  8  |    1000    |    12    |    4    | 20  |
    | f(3)  |   0011   | 1 |  2  |    0010    |     1    |    2    |  4  |
    | f(1)  |   0001   | 0 |  1  |    0001    |     0    |    1    |  1  |
    | f(0)  |   0000   | - |  -  |     -      |     -    |    -    |  0  |

    The other two calls in main, for contrast:

    | call  | n binary | x | 2^x | bitsTill      | msbPart    | ret |
    |-------|----------|---|-----|---------------|------------|-----|
    | f(4)  |  00100   | 2 |  4  | 2 * 2^1 =  4  | 4- 4+1 = 1 |   5 |
    | f(0)  |  00000   | - |  -  |      -        |     -      |   0 |
    | f(16) |  10000   | 4 | 16  | 4 * 2^3 = 32  | 16-16+1= 1 |  33 |
    | f(0)  |  00000   | - |  -  |      -        |     -      |   0 |

      f(4) = 4 + 1 + f(0) = 5      matches 1 + 1 + 2 + 1 from the Q block
      f(16) = 32 + 1 + f(0) = 33   (32 set bits across 0..15, plus the
                                    single bit 4 belonging to 16 itself)
      Both are exact powers of two, so n - 2^x is 0 and the recursion
      stops after ONE level - the cheapest possible shape for this code.

    The bit-level identity that makes the trick work:
      look at the numbers 0..7 written out in binary and count columns
      instead of rows:

          000   001   010   011   100   101   110   111
          bit 2: 0 0 0 0 1 1 1 1   -> four 1s
          bit 1: 0 0 1 1 0 0 1 1   -> four 1s
          bit 0: 0 1 0 1 0 1 0 1   -> four 1s
                                      total 12  = x * 2^(x-1), x = 3

      Every column of a complete range [0, 2^x - 1] is a perfectly
      balanced alternating pattern, so each of the x columns holds
      2^(x-1) ones - no counting required. Above that range, the numbers
      8..11 are just 0..3 with a 1000 glued on the front, which splits
      their bits cleanly into "the glued MSB" (msbPart, 4 of them) and
      "whatever 0..3 contribute" (the recursive call). Nothing is
      double-counted because the MSB column and the lower columns are
      disjoint.

    Step count backing O(log n):
      n = 11 (1011) needed 4 calls and 3 + 2 + 1 = 6 shift-loop tests -
      about a dozen operations. The naive "count bits of every number
      from 1 to 11" would run 11 iterations of a bit-counting loop, and
      at n = 10^9 that becomes ~30 billion bit inspections versus at
      most 30 calls here. Recursion depth equals the number of set bits
      of n (each level clears exactly the highest remaining one), so the
      O(log n) stack claim is tight: 1011 has three 1s -> three
      non-base calls.
*/
