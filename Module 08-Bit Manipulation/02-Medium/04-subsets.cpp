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
