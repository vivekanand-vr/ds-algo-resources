#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> res;
    int n = nums.size();

    // Step 1: Sort the array to handle duplicates and use two-pointer method
    sort(nums.begin(), nums.end());

    // Step 2: Fix one number and find the other two using two pointers
    for (int i = 0; i < n; i++) {
        // Skip duplicates for the first element
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        int l = i + 1;       // left pointer (next element)
        int r = n - 1;       // right pointer (last element)

        while (l < r) {
            long sum = (long)nums[i] + nums[l] + nums[r];  // current triplet sum

            if (sum == 0) {
                // Found a valid triplet
                res.push_back({nums[i], nums[l], nums[r]});
                l++;
                r--;

                // Skip duplicates for left pointer
                while (l < r && nums[l] == nums[l - 1]) l++;
                // Skip duplicates for right pointer
                while (l < r && nums[r] == nums[r + 1]) r--;
            }
            else if (sum < 0) {
                // If sum is too small, move left pointer to increase sum
                l++;
            }
            else {
                // If sum is too large, move right pointer to decrease sum
                r--;
            }
        }
    }
    return res;
}


/*
Efficient 3Sum approach (sorting + two-pointer)
-----------------------------------------------
Time Complexity:  
- Sorting → O(n log n)  
- Outer loop runs n times, and inside we use a two-pointer scan (O(n))  
- So total = O(n^2)

Space Complexity:  
- O(1) extra (ignoring result storage)  
*/
