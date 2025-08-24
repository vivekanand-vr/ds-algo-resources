#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> fourSum(vector<int>& nums, int target) {
    vector<vector<int>> res;
    int n = nums.size();

    // Step 1: Sort the array
    sort(nums.begin(), nums.end());

    // Step 2: Fix the first number
    for (int i = 0; i < n; i++) {
        // Skip duplicates for first number
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        // Step 3: Fix the second number
        for (int j = i + 1; j < n; j++) {
            // Skip duplicates for second number
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;

            int l = j + 1;       // left pointer
            int r = n - 1;       // right pointer

            // Step 4: Use two pointers for remaining two numbers
            while (l < r) {
                long sum = (long)nums[i] + nums[j] + nums[l] + nums[r]; // current sum

                if (sum == target) {
                    // Found a valid quadruplet
                    res.push_back({nums[i], nums[j], nums[l], nums[r]});
                    l++;
                    r--;

                    // Skip duplicates for left pointer
                    while (l < r && nums[l] == nums[l - 1]) l++;
                    // Skip duplicates for right pointer
                    while (l < r && nums[r] == nums[r + 1]) r--;
                }
                else if (sum < target) {
                    // Sum is too small → move left pointer
                    l++;
                }
                else {
                    // Sum is too large → move right pointer
                    r--;
                }
            }
        }
    }

    return res;
}

/*
Efficient 4Sum approach (sorting + two-pointer + nested loops)
--------------------------------------------------------------
Time Complexity:  
- Sorting → O(n log n)  
- Outer two loops (i, j) → O(n^2)  
- Inner two-pointer scan → O(n)  
- So total = O(n^3)

Space Complexity:  
- O(1) extra (ignoring result storage)  
*/