#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Rotate an n x n matrix by 90 degrees clockwise, in-place

/*
    Approach: Transpose + Reverse Rows
    - 90 degree clockwise rotation = Transpose + Reverse each row
    - Transpose: flip along the main diagonal, arr[i][j] <-> arr[j][i]
    - Reverse: flip rows horizontally to complete the rotation

    Algorithm Steps
    ----------------
    1. Transpose the matrix by swapping arr[i][j] with arr[j][i] for j <= i
    2. Reverse each row of the transposed matrix
    3. The matrix is now rotated 90 degrees clockwise, in-place

    Time Complexity: O(n^2) - traversing all cells
    Space Complexity: O(1) - in-place (no extra matrix)
*/
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

int main() {
    vector<vector<int>> arr = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    cout << "Matrix before:" << endl;
    for (auto& row : arr) {
        for (int x : row) cout << x << " ";
        cout << endl;
    }

    rotate(arr);

    cout << "Matrix after rotating 90 degrees clockwise:" << endl;
    for (auto& row : arr) {
        for (int x : row) cout << x << " ";
        cout << endl;
    }

    return 0;
}
