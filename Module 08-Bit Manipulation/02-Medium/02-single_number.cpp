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
