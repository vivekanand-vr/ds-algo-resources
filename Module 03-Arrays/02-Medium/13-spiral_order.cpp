#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an m x n matrix, return all its elements in spiral order

/*
    Approach: Boundary Shrinking
    - Maintain four boundaries: left, right, top, bottom
    - Traverse the top row, right column, bottom row, and left column in order,
      shrinking each boundary after it is traversed
    - Repeat until all elements have been visited

    Algorithm Steps
    ----------------
    1. Initialize left, right, top, bottom boundaries and an index counter
    2. Traverse the top row left-to-right, then increment top
    3. Traverse the right column top-to-bottom, then decrement right
    4. Traverse the bottom row right-to-left (if still valid), then decrement bottom
    5. Traverse the left column bottom-to-top (if still valid), then increment left
    6. Repeat steps 2-5 until all m*n elements are collected

    Time Complexity: O(m*n) - visit every element once
    Space Complexity: O(m*n) - output array
*/
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

int main() {
    vector<vector<int>> arr = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    cout << "Matrix:" << endl;
    for (auto& row : arr) {
        for (int x : row) cout << x << " ";
        cout << endl;
    }

    vector<int> res = spiralOrder(arr);

    cout << "Spiral order: ";
    for (int x : res) cout << x << " ";
    cout << endl;

    return 0;
}
