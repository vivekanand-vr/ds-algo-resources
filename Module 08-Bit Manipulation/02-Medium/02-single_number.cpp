#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a non-empty array of integers where every element appears
//    exactly twice except for one, find that single element
//
// Example:
// nums = {4, 1, 2, 1, 2}
// 1 and 2 each appear twice, 4 appears once -> answer = 4

/*
    Approach: XOR Cancellation

    - XOR has two properties that make this work: x ^ x = 0 (a value
      cancels itself out) and x ^ 0 = x (identity element).
    - XOR is also commutative and associative, so the order the
      elements are combined in does not matter.
    - XORing every element together therefore lets every pair cancel
      out to 0, leaving only the element that had no partner.

    Algorithm Steps
    ----------------
    1. Initialize result = 0
    2. XOR every element of nums into result
    3. Return result

    Time Complexity: O(n) - a single pass through the array
    Space Complexity: O(1) - only one accumulator variable
*/
int singleNumber(vector<int>& nums) {
  int result = 0;
  for (int x : nums) result ^= x;
  return result;
}

int main() {
  vector<int> nums = {4, 1, 2, 1, 2};

  cout << "Array: ";
  for (int x : nums) cout << x << " ";
  cout << endl;

  cout << "Single number: " << singleNumber(nums) << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: nums = {4, 1, 2, 1, 2}   (answer = 4)
    ======================================================================

      index        :   0     1     2     3     4
      value        :   4     1     2     1     2
      binary       : 0100  0001  0010  0001  0010
      bit position : bit3 bit2 bit1 bit0  (4 = bit2, 2 = bit1, 1 = bit0)

    Tracked state:
      result - the running XOR of every element seen so far; a bit is 1
               here exactly when that bit has been seen an ODD number of
               times

    Initial state: result = 0000 (0)

    ----------------------------------------------------------------------
    x = 4
      xor         0000 ^ 0100 = 0100
      result      0 -> 4        bit2 now odd (first 4)

    ----------------------------------------------------------------------
    x = 1
      xor         0100 ^ 0001 = 0101
      result      4 -> 5        bit0 now odd (first 1)

    ----------------------------------------------------------------------
    x = 2
      xor         0101 ^ 0010 = 0111
      result      5 -> 7        bit1 now odd (first 2)
                  every bit is set here: 4, 1 and 2 use disjoint bits

    ----------------------------------------------------------------------
    x = 1     <-- the SECOND 1: it annihilates the first one
      xor         0111 ^ 0001 = 0110
      result      7 -> 6        bit0 back to 0, the pair of 1s cancelled

    ----------------------------------------------------------------------
    x = 2     <-- the SECOND 2: it annihilates the first one
      xor         0110 ^ 0010 = 0100
      result      6 -> 4        bit1 back to 0, the pair of 2s cancelled
      loop end    array exhausted

    ----------------------------------------------------------------------
    RETURN result = 0100 = 4        only the unpaired element survives

    ======================================================================
    Summary table
    ======================================================================

    | step | x | x binary | result before | result after | decimal |
    |------|---|----------|---------------|--------------|---------|
    |  1   | 4 |  0100    |    0000       |    0100      |    4    |
    |  2   | 1 |  0001    |    0100       |    0101      |    5    |
    |  3   | 2 |  0010    |    0101       |    0111      |    7    |
    |  4   | 1 |  0001    |    0111       |    0110      |    6    |
    |  5   | 2 |  0010    |    0110       |    0100      |    4    |

    The identity that makes it work:
      x ^ x = 0 and x ^ 0 = x, and XOR is commutative and associative, so
      the array may be reordered freely:
          4 ^ 1 ^ 2 ^ 1 ^ 2  =  4 ^ (1 ^ 1) ^ (2 ^ 2)
                             =  4 ^ 0 ^ 0
                             =  4
      Watch bit0 in the table: it turns on at step 2 and off at step 4.
      Bit1 turns on at step 3 and off at step 5. Bit2 (the 4) is never
      switched off because nothing else in the array carries it.

    Step count backing O(n):
      5 elements, 5 XOR operations, one accumulator - no map, no sorting.
*/
