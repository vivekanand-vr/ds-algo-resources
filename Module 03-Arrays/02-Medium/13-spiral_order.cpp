#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<int> spiralOrder(vector<vector<int>>& arr) {
    int n = arr.size(), m = arr[0].size();
    int left = 0, right = m - 1, top = 0, bottom = n - 1;
    int ind = 0;
    vector<int> res(m * n);

    while (ind < m * n) {
        // Traverse top row
        for (int i = left; i <= right; ++i) {
            res[ind++] = arr[top][i];
        }
        top++;  // shrink top boundary

        // Traverse right column
        for (int i = top; i <= bottom; ++i) {
            res[ind++] = arr[i][right];
        }
        right--; // shrink right boundary

        // Traverse bottom row (if still valid)
        for (int i = right; i >= left && ind < m * n; --i) {
            res[ind++] = arr[bottom][i];
        }
        bottom--; // shrink bottom boundary

        // Traverse left column (if still valid)
        for (int i = bottom; i >= top && ind < m * n; --i) {
            res[ind++] = arr[i][left];
        }
        left++; // shrink left boundary
    }

    return res;
}

/*
Time Complexity:  O(m*n) → visit every element once
Space Complexity: O(m*n) → output array
*/
