#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find all leader elements in the array, i.e., elements greater than or equal to
// all elements to their right

/*
    Approach: Right-to-Left Scan with Running Maximum
    - Traverse the array from right to left while tracking the maximum seen so far
    - An element is a leader if it is greater than or equal to this running maximum
    - Collect leaders while scanning, then reverse to restore original order

    Algorithm Steps
    ----------------
    1. Initialize h with the last element of the array
    2. Traverse from the last index down to the first
    3. Update h = max(h, arr[i]); if arr[i] >= h, add arr[i] to the result
    4. Reverse the result to restore left-to-right order and return it

    Time Complexity: O(n) - single pass through array + reverse
    Space Complexity: O(n) - to store leaders
*/
vector<int> leaders(vector<int>& arr) {
    int n = arr.size();
    int h = arr[n - 1];      // keep track of maximum seen so far (from right side)
    vector<int> res;

    // Traverse from right to left
    for (int i = n - 1; i >= 0; --i) {
        // A leader is an element which is greater than or equal to 
        // all elements to its right.
        h = max(h, arr[i]);       // update running maximum
        if (arr[i] >= h) {
            res.push_back(arr[i]);  // if current element is leader, store it
        }
    }

    // We collected leaders from right to left,
    // so reverse to restore original left-to-right order
    reverse(begin(res), end(res));
    return res;
}


int main() {
    vector<int> arr = {10, 22, 12, 3, 0, 6};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    vector<int> res = leaders(arr);

    cout << "Leaders: ";
    for (int x : res) cout << x << " ";
    cout << endl;

    return 0;
}
