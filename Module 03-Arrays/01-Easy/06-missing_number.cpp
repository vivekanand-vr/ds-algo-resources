#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array nums containing n distinct numbers in the range [0, n],
// return the only number in the range that is missing from the array.

/*
    Approach: Sum Formula
    - The numbers 0 to n should sum to n*(n+1)/2 if none were missing
    - Subtract every present number from this expected sum
    - What remains is exactly the missing number

    Algorithm Steps
    ----------------
    1. Compute expected sum = n * (n + 1) / 2
    2. Subtract each element of nums from expected sum
    3. Return the remaining value (the missing number)

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
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

int main() {
    vector<int> nums = {3, 0, 1};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Missing number: " << missingNumber(nums) << endl;

    return 0;
}
