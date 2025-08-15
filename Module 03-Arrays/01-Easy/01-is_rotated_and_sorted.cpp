#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Check if the given array is sorted

bool check(vector<int>& arr) {
    int point = -1;
    
    // Step 1: Find the first index where the order is violated (i.e., arr[i] > arr[i+1])
    // This marks the "rotation point"
    for (int i = 0; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            point = i;
            break;
        }
    }

    // Step 2: If no such point is found, array is already sorted (not rotated or rotated 0 times)
    if (point == -1) return true;

    // Step 3: Check if the remaining part after the point is sorted
    for (int i = point + 1; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return false; // not sorted
        }
    }

    // Step 4: Check if the last element is not greater than the first part before the point
    // i.e., max in second part ≤ min in first part for rotated sorted array
    for (int i = 0; i <= point; ++i) {
        if (arr[arr.size() - 1] > arr[i]) {
            return false;
        }
    }

    return true;
}

// Time Complexity: O(n)
// Space Complexity: O(1)
