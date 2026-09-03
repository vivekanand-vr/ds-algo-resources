#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find all unique quadruplets in the array that sum up to the given target

/*
    Approach: Sort + Nested Loops + Two Pointers
    - Sort the array so duplicates are adjacent and two-pointer scanning works
    - Fix the first two elements of the quadruplet with nested loops, then use
      two pointers on the remaining subarray to find the last two elements
    - Skip duplicate values at every position (i, j, l, r) to avoid duplicate
      quadruplets in the result
    - Use a wider integer type (long) for the running sum to avoid overflow

    Algorithm Steps
    ----------------
    1. Sort nums in ascending order
    2. For each index i, skip if it duplicates the previous fixed first element
    3. For each index j > i, skip if it duplicates the previous fixed second element
    4. Set l = j+1, r = n-1 and move pointers based on the quadruplet sum
    5. If sum == target, record the quadruplet and skip duplicates for l and r
    6. If sum < target, move l right; if sum > target, move r left
    7. Return all collected quadruplets

    Time Complexity: O(n^3) - sorting O(n log n) plus two nested loops (O(n^2)) each doing an O(n) two-pointer scan
    Space Complexity: O(1) - excluding the space used for the output
*/
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

int main() {
    vector<int> nums = {1, 0, -1, 0, -2, 2};
    int target = 0;

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;
    cout << "Target: " << target << endl;

    vector<vector<int>> result = fourSum(nums, target);

    cout << "Quadruplets that sum to target:" << endl;
    for (auto& quad : result) {
        for (int x : quad) cout << x << " ";
        cout << endl;
    }

    return 0;
}