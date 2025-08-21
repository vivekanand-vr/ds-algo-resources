#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<int> rearrangeArray(vector<int>& arr) {
    int pos = 0, neg = 1;         // positions to place positives & negatives
    int n = arr.size();
    vector<int> res(n);           // result array of same size

    // Place positive numbers at even indices (0,2,4,..)
    // Place negative numbers at odd indices (1,3,5,..)
    for (int i : arr) {
        if (i > 0) {
            res[pos] = i;
            pos += 2;
        } else {
            res[neg] = i;
            neg += 2;
        }
    }

    return res;
}

/*
Time Complexity:  O(n) → single pass through array
Space Complexity: O(n) → extra result array
*/
