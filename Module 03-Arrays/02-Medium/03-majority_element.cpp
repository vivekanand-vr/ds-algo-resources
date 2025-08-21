#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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

/*
Algorithm: Boyer-Moore Voting Algorithm
---------------------------------------
- Keeps track of a candidate and a counter.
- Majority element (appears > n/2 times) is guaranteed to survive.

Time Complexity:
O(n) → single pass over the array.

Space Complexity:
O(1) → uses only a couple of variables.
*/
