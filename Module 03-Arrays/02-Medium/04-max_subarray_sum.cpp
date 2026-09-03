#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the contiguous subarray with the largest sum and return that sum

/*
    Approach: Kadane's Algorithm
    - Iteratively decide whether to extend the previous subarray
      or start a new one from the current element
    - Tracks the maximum subarray sum in a single pass

    Algorithm Steps
    ----------------
    1. Initialize sum and res with nums[0]
    2. For each subsequent element, set sum = max(nums[i], sum + nums[i])
    3. Update res with the maximum of res and sum
    4. Return res

    Time Complexity: O(n) - single pass through the array
    Space Complexity: O(1) - only uses a few integer variables
*/
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

int main() {
    vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Maximum subarray sum: " << maxSubArray(nums) << endl;

    return 0;
}
