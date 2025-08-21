#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the element that appears only once in an array where all other elements appear twice

int singleNumber(vector<int>& nums) {
    int n = nums.size();
    int answer = nums[0];

    // XOR properties:
    // x ^ x = 0, x ^ 0 = x, and XOR is commutative & associative
    // Pairs cancel out to 0, leaving only the unique number
    for (int i = 1; i < n; i++) {
        answer ^= nums[i];
    }

    return answer;
}

// Time Complexity: O(n) — single pass through nums
// Space Complexity: O(1) — constant extra space
