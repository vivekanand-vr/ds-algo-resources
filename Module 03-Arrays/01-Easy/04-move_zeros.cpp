#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an integer array nums, move all 0's to the end of it while maintaining the relative order

void moveZeroes(vector<int>& arr) {
    int ind = 0;
    int n = arr.size();

    // Step 1: Move all non-zero elements to the front
    for (int i = 0; i < n; ++i) {
        if (arr[i] != 0) {
            arr[ind++] = arr[i]; // Place non-zero element at next available position
        }
    }

    // Step 2: Fill the rest of the array with 0s
    fill(arr.begin() + ind, arr.end(), 0);
}

// Time Complexity: O(n)
// Space Complexity: O(1)
