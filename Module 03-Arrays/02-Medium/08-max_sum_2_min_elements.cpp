#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int maxSum(vector<int> &arr) {
    int n = arr.size();
    int ans = INT_MIN;

    // Traverse the array and consider adjacent pairs
    for (int i = 0; i < n - 1; i++) {
        // In a subarray that includes arr[i] and arr[i+1],
        // the two smallest elements will be these two.
        // So, take their sum.
        int curr = arr[i] + arr[i + 1];

        // Update the maximum sum
        ans = max(ans, curr);
    }

    return ans;
}

/*
Logic:
------
- For every adjacent pair (arr[i], arr[i+1]),
  their sum represents the sum of the smallest two 
  in some subarray that contains them.
- The maximum over all such adjacent sums is the answer.

Time Complexity:  O(n) → single pass through array
Space Complexity: O(1) → only extra variables
*/
