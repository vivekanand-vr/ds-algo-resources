#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find all unique triplets in the array that sum up to zero

/*
    Approach: Sort + Two Pointers
    - Sort the array so duplicates sit next to each other and two-pointer
      scanning becomes possible
    - Fix the first element of the triplet, then use two pointers (left/right)
      on the remaining subarray to find pairs summing to -nums[i]
    - Skip over duplicate values for all three positions to avoid duplicate
      triplets in the result

    Algorithm Steps
    ----------------
    1. Sort nums in ascending order
    2. For each index i, skip if it duplicates the previous fixed element
    3. Set l = i+1, r = n-1 and move pointers based on the triplet sum
    4. If sum == 0, record the triplet and skip duplicates for l and r
    5. If sum < 0, move l right; if sum > 0, move r left
    6. Return all collected triplets

    Time Complexity: O(n^2) - sorting O(n log n) plus O(n) two-pointer scan for each of n fixed elements
    Space Complexity: O(1) - excluding the space used for the output
*/
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

int main() {
    vector<int> nums = {-1, 0, 1, 2, -1, -4};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    vector<vector<int>> result = threeSum(nums);

    cout << "Triplets that sum to zero:" << endl;
    for (auto& triplet : result) {
        for (int x : triplet) cout << x << " ";
        cout << endl;
    }

    return 0;
}
