#include <bits/stdc++.h>

#include <iostream>
using namespace std;

int searchInsert(vector<int>& nums, int t) {
  int n = nums.size();
  int l = 0, h = n - 1;
  int ans = 0;  // stores the index where target should be inserted

  while (l <= h) {
    int mid = l + (h - l) / 2;  // avoid overflow compared to (l+h)/2

    if (nums[mid] == t) {
      // If we find the target, return its index
      return mid;
    } else if (nums[mid] < t) {
      // If target is bigger, move to the right half
      l = mid + 1;
      ans = l;  // potential insert position is just after mid
    } else {
      // If target is smaller, move to the left half
      h = mid - 1;
      // ans doesn't update here, because insertion would be at 'mid'
    }
  }

  return ans;  // if not found, return insert position
}

/*
  Time Complexity:  O(log n)  → binary search on sorted array
  Space Complexity: O(1)      → constant extra space
*/

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  return 0;
}