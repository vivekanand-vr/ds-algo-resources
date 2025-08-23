#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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


/*
Time Complexity:  O(n) → single pass through array + reverse
Space Complexity: O(n) → to store leaders
*/
