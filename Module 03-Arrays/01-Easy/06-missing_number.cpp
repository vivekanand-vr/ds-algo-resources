#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int missingNumber(vector<int>& nums) {
    int n = nums.size();

    // Step 1: Calculate expected sum of first n natural numbers (0 to n)
    int sum = n * (n + 1) / 2;

    // Step 2: Subtract each number in the array from the expected sum
    for (int i : nums) {
        sum -= i;
    }

    // Step 3: The result is the missing number
    return sum;
}

// Time Complexity: O(n)
// Space Complexity: O(1)
