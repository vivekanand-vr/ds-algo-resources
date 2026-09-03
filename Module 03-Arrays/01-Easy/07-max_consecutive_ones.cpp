#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given a binary array, find the maximum number of consecutive 1's in the array

/*
    Approach: Single Pass Counter
    - Keep a running counter of the current streak of consecutive 1s
    - Every time a 1 is seen, increment the counter and update the max
    - Every time a 0 is seen, reset the counter to 0

    Algorithm Steps
    ----------------
    1. Initialize curr = 0, mx = 0
    2. Traverse the array
    3. If the element is 1, increment curr and update mx = max(mx, curr)
    4. If the element is 0, reset curr to 0
    5. Return mx

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
int findMaxConsecutiveOnes(vector<int>& arr) {
    int curr = 0, mx = 0;
    for (int num : arr) {
        if (num == 1) {
            curr++;
            // Increment count and update maximum
            mx = max(mx, curr);
        } else {
            // Reset the counter
            curr = 0;
        }
    }
    return mx;
}

int main() {
    vector<int> arr = {1, 1, 0, 1, 1, 1, 0, 1, 1};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Max consecutive ones: " << findMaxConsecutiveOnes(arr) << endl;

    return 0;
}
