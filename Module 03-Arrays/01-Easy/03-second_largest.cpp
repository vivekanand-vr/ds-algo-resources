#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array find the second largest element without sorting

/*
    Approach: Single Pass Tracking
    - Maintain two variables: largest (mx) and second largest (sec)
    - While scanning, whenever a new largest is found, the old largest becomes
      the second largest
    - Otherwise, if the current element is not equal to the largest but greater
      than the current second largest, update the second largest

    Algorithm Steps
    ----------------
    1. Initialize mx and sec to INT_MIN
    2. Traverse the array once
    3. If arr[i] > mx, shift mx into sec, then update mx
    4. Else if arr[i] > sec and arr[i] != mx, update sec
    5. Return sec (or -1 if no valid second largest exists)

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
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

int main() {
    vector<int> arr = {8, 8, 7, 6, 5};
    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Second largest element: " << getSecondLargest(arr) << endl;

    return 0;
}
