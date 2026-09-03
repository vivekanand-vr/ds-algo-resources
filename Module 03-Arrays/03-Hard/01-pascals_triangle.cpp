#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Generate the first n rows of Pascal's triangle, where each number is the
// sum of the two numbers directly above it (edges of each row are 1)

/*
    Approach: Build Row-by-Row Using Previous Row
    - Each row i has i+1 elements, with the first and last elements always 1
    - Every middle element at column j equals the sum of the two elements
      diagonally above it in the previous row: res[i-1][j-1] + res[i-1][j]
    - Build rows in order so previous rows are always available

    Algorithm Steps
    ----------------
    1. For each row i from 0 to n-1, create a row of i+1 ones
    2. For each middle column j (1 to i-1), set row[j] = res[i-1][j-1] + res[i-1][j]
    3. Push the completed row into the result
    4. Return the result after all rows are built

    Time Complexity: O(n^2) - generating n rows, each with up to n elements
    Space Complexity: O(n^2) - storing all rows of the triangle
*/
vector<vector<int>> generate(int n) {
    vector<vector<int>> res;

    for (int i = 0; i < n; ++i) {
        vector<int> row(i + 1, 1);   // each row has i+1 elements, all initialized to 1

        // Fill middle elements using values from previous row
        for (int j = 1; j < i; ++j) {
            row[j] = res[i - 1][j - 1] + res[i - 1][j];
        }

        res.push_back(row);
    }

    return res;
}

int main() {
    int n = 5;

    cout << "Number of rows: " << n << endl;

    vector<vector<int>> triangle = generate(n);

    cout << "Pascal's Triangle:" << endl;
    for (auto& row : triangle) {
        for (int x : row) cout << x << " ";
        cout << endl;
    }

    return 0;
}
