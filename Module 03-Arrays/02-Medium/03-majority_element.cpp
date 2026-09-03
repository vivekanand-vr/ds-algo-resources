#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array of size n, find the majority element (appears more than n/2 times)

/*
    Approach: Boyer-Moore Voting Algorithm
    - Keeps track of a candidate and a counter representing its "balance"
    - Majority element (appears > n/2 times) is guaranteed to survive the cancellation

    Algorithm Steps
    ----------------
    1. Initialize counter c = 0 and candidate curr = nums[0]
    2. Traverse the array; if c == 0, set curr to the current element
    3. Increment c if current element matches curr, else decrement c
    4. Return curr as the majority element

    Time Complexity: O(n) - single pass over the array
    Space Complexity: O(1) - uses only a couple of variables
*/
int majorityElement(vector<int>& nums) {
    int n = (int) nums.size();
    int c = 0;             // counter for tracking "balance" of candidate
    int curr = nums[0];    // candidate element

    // Traverse array to find potential majority element
    for (int i = 0; i < n; ++i) {
        if (c == 0) 
            curr = nums[i];    // set new candidate when counter is zero

        // If current element matches candidate, increment counter
        // Else, decrement (cancel out one occurrence)
        c += (curr == nums[i]) ? 1 : -1;
    }

    // At the end, 'curr' will be the majority element
    return curr;
}

int main() {
    vector<int> nums = {2, 2, 1, 1, 1, 2, 2};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Majority element: " << majorityElement(nums) << endl;

    return 0;
}
