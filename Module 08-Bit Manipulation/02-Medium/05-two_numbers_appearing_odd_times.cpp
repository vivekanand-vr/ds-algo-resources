#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an array nums of length n, every integer appears twice except
//    for two integers, which each appear only once. Return those two
//    integers in ascending order.
//
// Example:
// nums = {1, 2, 1, 3, 5, 2}
// 3 and 5 each appear once -> answer = {3, 5}   (not {5, 3})

/*
    Approach: XOR + Rightmost Set Bit Partitioning

    - XORing every element together cancels out every properly paired
      duplicate (x ^ x = 0), leaving xorAll = a ^ b, where a and b are
      the two singleton values.
    - Since a != b, xorAll is non-zero, so it has at least one set bit.
      Pick any such bit - the rightmost one is easiest to isolate - and
      call it diffBit. a and b must differ at that position (otherwise
      it could not have survived the XOR into xorAll).
    - diffBit splits every number in nums into two groups: those with
      diffBit set, and those without. Because a and b differ there,
      they land in different groups. Every duplicate pair, on the other
      hand, always lands in the SAME group (both copies of a value have
      identical bits), so duplicates still cancel out within each group.
    - XORing each group separately therefore isolates a in one group
      and b in the other.
    - diffBit is extracted with xorAll & (-xorAll): in two's complement,
      negation flips every bit and adds 1, which turns every bit up to
      and including the lowest set bit, and only that low run cancels
      correctly under AND, leaving just that one lowest set bit.

    Algorithm Steps
    ----------------
    1. XOR all elements of nums into xorAll (= a xor b)
    2. Isolate the lowest set bit: diffBit = xorAll & (-xorAll)
    3. Walk nums again, splitting into two running XORs based on
       whether diffBit is set in each element
    4. The two running XORs are {a, b} in some order - sort them
       ascending before returning

    Time Complexity: O(n) - two linear passes through the array
    Space Complexity: O(1) - only a few accumulator variables
*/
vector<int> twoOddOccurring(vector<int>& nums) {
  int xorAll = 0;
  for (int x : nums) xorAll ^= x;

  int diffBit = xorAll & (-xorAll);   // isolate the lowest set bit

  int a = 0, b = 0;
  for (int x : nums) {
    if (x & diffBit) a ^= x;
    else b ^= x;
  }

  if (a > b) swap(a, b);
  return {a, b};
}

int main() {
  vector<int> nums = {1, 2, 1, 3, 5, 2};

  cout << "Array: ";
  for (int x : nums) cout << x << " ";
  cout << endl;

  vector<int> res = twoOddOccurring(nums);
  cout << "The two once-occurring numbers: [" << res[0] << ", " << res[1] << "]" << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: nums = {1, 2, 1, 3, 5, 2}   (answer = {3, 5})
    ======================================================================

      index        :   0     1     2     3     4     5
      value        :   1     2     1     3     5     2
      binary       : 0001  0010  0001  0011  0101  0010
      bit position : bit3 bit2 bit1 bit0   (8=bit3, 4=bit2, 2=bit1, 1=bit0)

    Tracked state:
      xorAll  - running XOR of everything; ends up as a ^ b
      diffBit - the single lowest set bit of xorAll, used as the splitter
      a       - running XOR of the numbers that HAVE diffBit set
      b       - running XOR of the numbers that do NOT have diffBit set

    Initial state: xorAll = 0000, a = 0000, b = 0000

    ======================================================================
    PASS 1 - fold everything into xorAll
    ======================================================================

    ----------------------------------------------------------------------
    x = 1     0000 ^ 0001 = 0001      xorAll = 1
    ----------------------------------------------------------------------
    x = 2     0001 ^ 0010 = 0011      xorAll = 3
    ----------------------------------------------------------------------
    x = 1     0011 ^ 0001 = 0010      xorAll = 2   the pair of 1s cancels
    ----------------------------------------------------------------------
    x = 3     0010 ^ 0011 = 0001      xorAll = 1
    ----------------------------------------------------------------------
    x = 5     0001 ^ 0101 = 0100      xorAll = 4
    ----------------------------------------------------------------------
    x = 2     0100 ^ 0010 = 0110      xorAll = 6   the pair of 2s cancels
    ----------------------------------------------------------------------

    xorAll = 0110 = 6, and indeed 3 ^ 5 = 0011 ^ 0101 = 0110 = 6.
    Only the two singletons survived.

    ======================================================================
    ISOLATE the differentiating bit:  diffBit = xorAll & (-xorAll)
    ======================================================================

      xorAll        0 0 0 0 0 1 1 0        (6, shown in 8 bits)
      invert bits   1 1 1 1 1 0 0 1
      add 1         1 1 1 1 1 0 1 0        (= -6 in two's complement)
      AND           0 0 0 0 0 1 1 0
                  & 1 1 1 1 1 0 1 0
                  = 0 0 0 0 0 0 1 0        diffBit = 0010 = 2 (bit1)

      Why bit1 and not bit2: xorAll = 0110 has bit1 AND bit2 set, so
      either would work as a splitter; & (-x) always picks the LOWEST
      one, here bit1. A set bit in xorAll means 3 and 5 disagree there:
        3 = 0011  bit1 = 1
        5 = 0101  bit1 = 0     <-- they land in different buckets

    ======================================================================
    PASS 2 - partition on bit1 and XOR each bucket separately
    ======================================================================

    ----------------------------------------------------------------------
    x = 1   0001 & 0010 = 0000 -> bit1 CLEAR, goes to b
      b     0000 ^ 0001 = 0001        a = 0 (0000),  b = 1 (0001)
    ----------------------------------------------------------------------
    x = 2   0010 & 0010 = 0010 -> bit1 SET, goes to a
      a     0000 ^ 0010 = 0010        a = 2 (0010),  b = 1 (0001)
    ----------------------------------------------------------------------
    x = 1   0001 & 0010 = 0000 -> bit1 CLEAR, goes to b
      b     0001 ^ 0001 = 0000        a = 2 (0010),  b = 0 (0000)
                                      the 1s cancelled INSIDE bucket b
    ----------------------------------------------------------------------
    x = 3   0011 & 0010 = 0010 -> bit1 SET, goes to a
      a     0010 ^ 0011 = 0001        a = 1 (0001),  b = 0 (0000)
    ----------------------------------------------------------------------
    x = 5   0101 & 0010 = 0000 -> bit1 CLEAR, goes to b
      b     0000 ^ 0101 = 0101        a = 1 (0001),  b = 5 (0101)
    ----------------------------------------------------------------------
    x = 2   0010 & 0010 = 0010 -> bit1 SET, goes to a
      a     0001 ^ 0010 = 0011        a = 3 (0011),  b = 5 (0101)
                                      the 2s cancelled INSIDE bucket a
      loop end - array exhausted
    ----------------------------------------------------------------------

    ORDER: a = 3, b = 5. The check is a > b, i.e. 3 > 5 -> false, so no
    swap happens on this input.

    ----------------------------------------------------------------------
    RETURN {3, 5}        (a = 0011, b = 0101)

    ======================================================================
    Summary table
    ======================================================================

    Pass 1:

    | step | x | x binary | xorAll before | xorAll after | decimal |
    |------|---|----------|---------------|--------------|---------|
    |  1   | 1 |  0001    |     0000      |    0001      |    1    |
    |  2   | 2 |  0010    |     0001      |    0011      |    3    |
    |  3   | 1 |  0001    |     0011      |    0010      |    2    |
    |  4   | 3 |  0011    |     0010      |    0001      |    1    |
    |  5   | 5 |  0101    |     0001      |    0100      |    4    |
    |  6   | 2 |  0010    |     0100      |    0110      |    6    |

    Pass 2 (diffBit = 0010):

    | step | x | x binary | x & diffBit | bucket | a after | b after |
    |------|---|----------|-------------|--------|---------|---------|
    |  1   | 1 |  0001    |    0000     |   b    |  0000   |  0001   |
    |  2   | 2 |  0010    |    0010     |   a    |  0010   |  0001   |
    |  3   | 1 |  0001    |    0000     |   b    |  0010   |  0000   |
    |  4   | 3 |  0011    |    0010     |   a    |  0001   |  0000   |
    |  5   | 5 |  0101    |    0000     |   b    |  0001   |  0101   |
    |  6   | 2 |  0010    |    0010     |   a    |  0011   |  0101   |

    Bucket contents at a glance:
      bucket a (bit1 set)   : {2, 3, 2}  ->  2 ^ 3 ^ 2 = 3
      bucket b (bit1 clear) : {1, 1, 5}  ->  1 ^ 1 ^ 5 = 5
    Note that BOTH copies of 2 went to bucket a and BOTH copies of 1 went
    to bucket b. That is the whole trick: identical values have identical
    bits, so a duplicate pair can never be split across the two buckets,
    and each bucket reduces to the single-number problem.

    The identity that makes it work:
      1^2^1^3^5^2 = (1^1) ^ (2^2) ^ 3 ^ 5 = 3 ^ 5 = 6, so xorAll is
      exactly a ^ b; and any set bit of a ^ b is a position where a and b
      disagree, which is what lets one bit separate them.

    Step count backing O(n):
      6 XORs in pass 1, then 1 negate + 1 AND, then 6 AND-tests and 6
      XORs in pass 2 - 20 operations for 6 elements, two linear passes,
      no map and no sorting (the final ordering is a single comparison).
*/
