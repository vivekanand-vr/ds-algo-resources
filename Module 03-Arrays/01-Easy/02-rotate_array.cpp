#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void rotateRight(vector<int>& nums, int k) {
  int n = nums.size();
  k %= n;  // Handle cases where k > n

  // Step 1: Reverse the first n-k elements
  reverse(nums.begin(), nums.end() - k);

  // Step 2: Reverse the last k elements
  reverse(nums.end() - k, nums.end());

  // Step 3: Reverse the entire array
  reverse(nums.begin(), nums.end());
}

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