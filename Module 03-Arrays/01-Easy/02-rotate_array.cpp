#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array left rotate it by k places

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

// Time Complexity: O(n)
// Space Complexity:O(1) 
