#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array left rotate it by k places

/*
    Approach: Reversal Algorithm
    - Left rotating by k is equivalent to:
      reverse the first k elements, reverse the rest, then reverse the whole array
    - This achieves the rotation in-place without extra space

    Algorithm Steps
    ----------------
    1. Reduce k modulo n to handle k > n
    2. Reverse the first k elements
    3. Reverse the remaining n-k elements
    4. Reverse the entire array

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void rotateLeft(vector<int>& nums, int k) {
  int n = nums.size();
  k %= n;  // Handle cases where k > n

  // Step 1: Reverse the first k elements
  reverse(nums.begin(), nums.begin() + k);

  // Step 2: Reverse the remaining n-k elements
  reverse(nums.begin() + k, nums.end());

  // Step 3: Reverse the entire array
  reverse(nums.begin(), nums.end());
}

int main() {
  vector<int> nums = {1, 2, 3, 4, 5, 6, 7};
  int k = 2;

  cout << "Original array: ";
  for (int x : nums) cout << x << " ";
  cout << endl;

  rotateLeft(nums, k);

  cout << "Array after left rotating by " << k << " places: ";
  for (int x : nums) cout << x << " ";
  cout << endl;

  return 0;
}
