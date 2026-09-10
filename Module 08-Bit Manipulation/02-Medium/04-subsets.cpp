#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given an array of n distinct integers, return all possible subsets
//    (the power set)
//
// Example:
// nums = {1, 2, 3}
// Subsets: {}, {1}, {2}, {1,2}, {3}, {1,3}, {2,3}, {1,2,3}
//
// Bitmask walkthrough (n = 3, bit i <-> nums[i], bit0 = LSB = nums[0]):
//   mask   binary (b2 b1 b0)   included elements        subset
//    0        0  0  0         (none)                    {}
//    1        0  0  1         nums[0]=1                 {1}
//    2        0  1  0         nums[1]=2                 {2}
//    3        0  1  1         nums[0], nums[1]          {1,2}
//    4        1  0  0         nums[2]=3                 {3}
//    5        1  0  1         nums[0], nums[2]          {1,3}
//    6        1  1  0         nums[1], nums[2]          {2,3}
//    7        1  1  1         nums[0], nums[1], nums[2] {1,2,3}

/*
    Approach: Bitmask Enumeration

    - A set of n elements has exactly 2^n subsets, and every subset can
      be described by an n-bit pattern: bit i is 1 if nums[i] belongs
      to that subset, 0 if it does not.
    - So instead of recursing (include / exclude each element), we can
      simply count masks from 0 to 2^n - 1. Each mask IS a subset:
      mask = 0 is the empty subset, mask = (1<<n)-1 is the full set,
      and everything in between is exactly one of the remaining ones.
    - For a given mask, checking (mask >> i) & 1 tells us whether
      nums[i] is included.

    Algorithm Steps
    ----------------
    1. Let n = nums.size(); there are 2^n subsets in total
    2. For every mask from 0 to (1 << n) - 1:
         a. For every bit position i from 0 to n - 1:
              if bit i of mask is set, include nums[i] in this subset
         b. Add the built subset to the result
    3. Return all collected subsets

    Time Complexity: O(n * 2^n) - 2^n masks, O(n) work to build each subset
    Space Complexity: O(n * 2^n) - to store all subsets (output size itself)
*/
vector<vector<int>> subsets(vector<int>& nums) {
  int n = nums.size();
  vector<vector<int>> result;

  for (int mask = 0; mask < (1 << n); mask++) {
    vector<int> subset;
    for (int i = 0; i < n; i++) {
      if (mask & (1 << i)) subset.push_back(nums[i]);
    }
    result.push_back(subset);
  }

  return result;
}

int main() {
  vector<int> nums = {1, 2, 3};

  cout << "Array: ";
  for (int x : nums) cout << x << " ";
  cout << endl;

  vector<vector<int>> res = subsets(nums);

  cout << "All subsets:" << endl;
  for (const auto& subset : res) {
    cout << "{ ";
    for (int x : subset) cout << x << " ";
    cout << "}" << endl;
  }

  return 0;
}

/*
    ======================================================================
    DRY RUN: nums = {1, 2, 3}   (n = 3, answer = 8 subsets)
    ======================================================================

    The mask -> subset mapping is already tabulated at the top of this
    file. What follows traces the LOOP instead: how the inner bit-test
    actually builds one subset, and how result grows.

      index i      :   0     1     2
      nums[i]      :   1     2     3
      test value   : 1<<0  1<<1  1<<2
      binary       : 0001  0010  0100
      bit position : bit2 bit1 bit0   (bit0 = LSB = nums[0])

    Tracked state:
      mask   - the outer counter, 0 .. (1<<3)-1 = 0..7; its bit i says
               whether nums[i] joins this subset
      subset - the vector being built for the current mask
      result - all finished subsets so far

    Initial state: mask = 0, result = [] (empty)

    ----------------------------------------------------------------------
    mask = 0   (binary 000)
      step 1: i = 0   mask & (1<<0) = 000 & 001 = 000 -> 0, skip
      step 2: i = 1   mask & (1<<1) = 000 & 010 = 000 -> 0, skip
      step 3: i = 2   mask & (1<<2) = 000 & 100 = 000 -> 0, skip
      subset  {}                        no bit set -> the empty subset
      result  [ {} ]                    size 1

    ----------------------------------------------------------------------
    mask = 3   (binary 011)
      step 1: i = 0   011 & 001 = 001 -> nonzero, push nums[0] = 1
                      subset = {1}
      step 2: i = 1   011 & 010 = 010 -> nonzero, push nums[1] = 2
                      subset = {1, 2}
      step 3: i = 2   011 & 100 = 000 -> 0, skip
      subset  {1, 2}
      result  [ {}, {1}, {2}, {1,2} ]   size 4

    ----------------------------------------------------------------------
    mask = 5   (binary 101)
      step 1: i = 0   101 & 001 = 001 -> nonzero, push nums[0] = 1
                      subset = {1}
      step 2: i = 1   101 & 010 = 000 -> 0, skip   (2 stays out)
      step 3: i = 2   101 & 100 = 100 -> nonzero, push nums[2] = 3
                      subset = {1, 3}
      subset  {1, 3}
      result  [ {}, {1}, {2}, {1,2}, {3}, {1,3} ]   size 6

    ----------------------------------------------------------------------
    mask = 7   (binary 111)
      all three tests nonzero -> subset = {1, 2, 3}
      result  size 8, mask++ -> 8, and 8 < (1<<3) = 8 is false: loop ends

    ----------------------------------------------------------------------
    RETURN result, 8 subsets, in mask order:
      {} {1} {2} {1,2} {3} {1,3} {2,3} {1,2,3}

    ======================================================================
    Summary table
    ======================================================================

    | mask | binary | i=0 test | i=1 test | i=2 test | subset  | result |
    |------|--------|----------|----------|----------|---------|--------|
    |  0   |  000   |   skip   |   skip   |   skip   | {}      |   1    |
    |  1   |  001   |   push 1 |   skip   |   skip   | {1}     |   2    |
    |  2   |  010   |   skip   |   push 2 |   skip   | {2}     |   3    |
    |  3   |  011   |   push 1 |   push 2 |   skip   | {1,2}   |   4    |
    |  4   |  100   |   skip   |   skip   |   push 3 | {3}     |   5    |
    |  5   |  101   |   push 1 |   skip   |   push 3 | {1,3}   |   6    |
    |  6   |  110   |   skip   |   push 2 |   push 3 | {2,3}   |   7    |
    |  7   |  111   |   push 1 |   push 2 |   push 3 | {1,2,3} |   8    |

    Step count backing O(n * 2^n):
      8 masks x 3 bit tests = 24 bit tests, and 12 pushes in total (the
      subset sizes 0+1+1+2+1+2+2+3 = 12 = n * 2^(n-1)). Nothing is
      recomputed and nothing backtracks - the mask counter alone drives
      the whole enumeration.
*/
