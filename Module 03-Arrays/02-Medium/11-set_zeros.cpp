#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an m x n matrix, if an element is 0, set its entire row and column to 0, in-place

/*
    Approach: Use First Row & Column as Markers
    - Use the first row & column as marker arrays to avoid extra space
    - First pass: if arr[i][j] == 0, mark arr[i][0] and arr[0][j]
      (special case: track first row separately with a rowZero flag)
    - Second pass: set arr[i][j] = 0 if its row or column was marked
    - Handle the first column and first row at the end using the markers/flag

    Algorithm Steps
    ----------------
    1. Traverse the matrix; if arr[i][j] == 0, mark arr[i][0] and arr[0][j]
       (use rowZero flag instead of arr[0][0] for the first row)
    2. For rows 1..n-1 and cols 1..m-1, set arr[i][j] = 0 if arr[i][0] == 0 or arr[0][j] == 0
    3. If arr[0][0] == 0, zero out the entire first column
    4. If rowZero == 0, zero out the entire first row

    Time Complexity: O(n * m) - two passes through the matrix
    Space Complexity: O(1) - in-place (only uses rowZero flag)
*/
void setZeroes(vector<vector<int>>& arr) {
    int n = arr.size(), m = arr[0].size();
    int rowZero = 1;   // flag to track if the first row needs to be zeroed

    // Step 1: Use first row & first column as "markers"
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (arr[i][j] == 0) {
                // Mark the row
                if (i == 0) {
                    rowZero = 0;   // remember separately if first row has a zero
                } else {
                    arr[i][0] = 0; // mark current row by setting first col = 0
                }

                // Mark the column
                arr[0][j] = 0;     // mark current col by setting first row = 0
            }
        }
    }

    // Step 2: Traverse the matrix (except first row/col)
    // If row or col is marked → set cell to 0
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            if (arr[i][0] == 0 || arr[0][j] == 0) {
                arr[i][j] = 0;
            }
        }
    }

    // Step 3: Handle first column separately (check arr[i][0])
    if (arr[0][0] == 0) {
        for (int i = 0; i < n; ++i) {
            arr[i][0] = 0;
        }
    }

    // Step 4: Handle first row separately (check rowZero flag)
    if (rowZero == 0) {
        for (int j = 0; j < m; ++j) {
            arr[0][j] = 0;
        }
    }
}

int main() {
    vector<vector<int>> arr = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    };

    cout << "Matrix before:" << endl;
    for (auto& row : arr) {
        for (int x : row) cout << x << " ";
        cout << endl;
    }

    setZeroes(arr);

    cout << "Matrix after:" << endl;
    for (auto& row : arr) {
        for (int x : row) cout << x << " ";
        cout << endl;
    }

    return 0;
}
