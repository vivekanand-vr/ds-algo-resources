#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int maxSubArray(vector<int>& nums) {
    int sum = nums[0];  // running sum (max subarray ending at current index)
    int res = nums[0];  // global maximum subarray sum found so far

    // Start from second element since we initialized with nums[0]
    for (int i = 1; i < nums.size(); ++i) {
        // Either extend the previous subarray or start fresh from current element
        sum = max(nums[i], sum + nums[i]);

        // Update global maximum
        res = max(res, sum);
    }

    return res;
}

/*
Algorithm: Kadane’s Algorithm
------------------------------
- Iteratively decide whether to extend the previous subarray 
  or start a new one from the current element.
- Tracks the maximum subarray sum in a single pass.

Time Complexity:
O(n) → single pass through the array.

Space Complexity:
O(1) → only uses a few integer variables.
*/
