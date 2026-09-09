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
         a. bitsTill2x = x * 2^(x - 1)
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

int countSetBitsIn1ToN(int n) {
  if (n <= 0) return 0;

  int x = findLargestPowerOf2(n);

  int bitsTillHighestPower = x * (1 << (x - 1));          // x * (2^(x-1))
  int msbFromHighestPowerToN = n - (1 << x) + 1;          // n = 2^x + 1
  int remainingBits = countSetBitsIn1ToN(n - (1 << x));   // n - 2^x

  return bitsTillHighestPower + msbFromHighestPowerToN + remainingBits;
}

int main() {
  int n = 4;
  cout << "n = " << n << endl;
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
