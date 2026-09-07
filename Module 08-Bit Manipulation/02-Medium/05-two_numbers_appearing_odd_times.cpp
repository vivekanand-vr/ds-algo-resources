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
