#include <bits/stdc++.h>

#include <iostream>
using namespace std;

// Q: Given a sorted array and a target value, return the index if the target
// is found, or the index where it would be inserted to keep the array sorted

/*
    Approach: Binary Search (Lower Bound)
    - This is equivalent to finding the "ceil" position of the target
    - Binary search for the target; whenever nums[mid] < t, the insert
      position must be to the right of mid, so update ans = mid + 1
    - Whenever nums[mid] >= t, the insert position is at or before mid,
      so search left without updating ans

    Algorithm Steps
    ----------------
    1. Initialize l = 0, h = n - 1, ans = 0
    2. While l <= h, compute mid = l + (h - l) / 2
    3. If nums[mid] == t, return mid (target found)
    4. If nums[mid] < t, set ans = l = mid + 1 (insert position moves right)
    5. Else set h = mid - 1 (insert position stays at or before mid)
    6. Return ans (the insert position if target isn't found)

    Time Complexity: O(log n) - binary search on sorted array
    Space Complexity: O(1) - constant extra space
*/
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<int> nums = {1, 3, 5, 6};
  int target = 5;

  cout << "Array: ";
  for (int n : nums) cout << n << " ";
  cout << endl;

  cout << "Search insert position of " << target << ": " << searchInsert(nums, target) << endl;

  return 0;
}