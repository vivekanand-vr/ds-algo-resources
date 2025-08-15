#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array find the second largest element without sorting

int getSecondLargest(vector<int> &arr) {
    int mx = INT_MIN;       
    int sec = INT_MIN;

    // Step 1: Iterate through the array to find the largest and second largest
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] > mx) {
            // If current element is greater than max, update second max and max
            sec = mx;
            mx = arr[i];
        } 
        else if (arr[i] > sec && arr[i] != mx) {
            // If it's not equal to max and greater than current second max, update second max
            sec = arr[i];
        }
    }

    // Step 2: If no valid second largest found (e.g., all elements are equal), return -1
    if (sec == INT_MIN) return -1;

    return sec;
}

// Time Complexity: O(n)
// Space Complexity: O(1)
