#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the element that appears only once in an array where all other elements appear twice

/*
    Approach: XOR
    - XOR-ing a number with itself gives 0, and XOR-ing with 0 gives the number back
    - XOR is commutative and associative, so order doesn't matter
    - XOR-ing every element together cancels out all pairs, leaving only the
      element that appears once

    Algorithm Steps
    ----------------
    1. Initialize answer with the first element
    2. XOR answer with every remaining element
    3. Return answer (the unique element)

    Time Complexity: O(n) - single pass through nums
    Space Complexity: O(1) - constant extra space
*/
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

int main() {
    vector<int> nums = {4, 1, 2, 1, 2};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Single number: " << singleNumber(nums) << endl;

    return 0;
}
