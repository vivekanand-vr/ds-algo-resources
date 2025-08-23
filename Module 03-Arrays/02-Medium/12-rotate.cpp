#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void rotate(vector<vector<int>>& arr) {
    int n = arr.size();

    // Step 1: Transpose the matrix
    // (Convert rows → columns by swapping arr[i][j] with arr[j][i])
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            swap(arr[i][j], arr[j][i]);
        }
    }

    // Step 2: Reverse each row
    // (This gives the final rotated matrix)
    for (int i = 0; i < n; ++i) {
        reverse(arr[i].begin(), arr[i].end());
    }
}

/*
Logic:
------
- 90° clockwise rotation = Transpose + Reverse each row.
- Transpose: flip along diagonal → arr[i][j] ↔ arr[j][i].
- Reverse: flip rows horizontally to complete rotation.

Time Complexity:  O(n^2) → traversing all cells
Space Complexity: O(1)  → in-place (no extra matrix)
*/
