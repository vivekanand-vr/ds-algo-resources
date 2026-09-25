#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: A peak in a 2D grid is a cell strictly greater than all of its adjacent
// neighbours (up, down, left, right). Given an n x m matrix mat where no two
// adjacent cells are equal, return the position {row, col} of ANY peak.
// Assume the grid is surrounded by -infinity on every side. Must run in
// O(n log m) or O(m log n).
//
// Example:
// mat = {{1, 4},
//        {3, 2}}                  -> {1, 0}   ({0, 1} is also valid)
// mat = {{10, 20, 15},
//        {21, 30, 14},
//        { 7, 16, 32}}            -> {1, 1}   ({2, 2} is also valid)

/*
    Approach: Binary search on columns, taking the column maximum
    - This is Find Peak Element (1D) done column by column. The binary
      search runs over the column indices low..high.
    - For the middle column mid, find the row maxRow holding its largest
      value. That cell already beats its up and down neighbours, since it is
      the largest in its column. Only left and right are left to check.
        beats left and right    -> it is a peak, return it
        left neighbour bigger   -> a peak exists in columns low..mid-1:
                                   high = mid - 1
        otherwise (right bigger)-> a peak exists in columns mid+1..high:
                                   low = mid + 1
    - Why the bigger side must hold a peak: suppose mat[maxRow][mid - 1] is
      bigger. Then the largest value in columns low..mid-1 is bigger than
      every value in column mid (column mid's max is smaller than a left
      neighbour). That largest value beats everything around it, so it is a
      peak on the left side. The same argument works for the right.
    - Out of range neighbours count as INT_MIN, like the -infinity border.

    Algorithm Steps
    ----------------
    1. low = 0, high = m - 1 over the columns.
    2. While low <= high, mid = low + (high - low) / 2:
         a. maxRow = row of the largest value in column mid.
         b. left  = mat[maxRow][mid - 1], or INT_MIN if mid == 0.
            right = mat[maxRow][mid + 1], or INT_MIN if mid == m - 1.
         c. If mat[maxRow][mid] > left and > right, return {maxRow, mid}.
         d. Else if left > mat[maxRow][mid], set high = mid - 1.
         e. Else set low = mid + 1.
    3. Return {-1, -1}. With the problem's guarantees this never happens.

    Time Complexity: O(n log m) - log m columns probed, each costing an O(n) scan
    Space Complexity: O(1)
*/
int maxRowInColumn(vector<vector<int>>& mat, int col) {
    int maxRow = 0;
    for(int i = 1; i < (int)mat.size(); ++i) {
        if(mat[i][col] > mat[maxRow][col]) {
            maxRow = i;
        }
    }
    return maxRow;
}

vector<int> findPeakGrid(vector<vector<int>>& mat) {
    int m = mat[0].size();
    int low = 0, high = m - 1;

    while(low <= high) {
        int mid = low + (high - low) / 2;

        // The column max already beats its up and down neighbours
        int maxRow = maxRowInColumn(mat, mid);
        int curr   = mat[maxRow][mid];

        // Neighbours outside the grid act as -infinity
        int left  = (mid == 0)     ? INT_MIN : mat[maxRow][mid - 1];
        int right = (mid == m - 1) ? INT_MIN : mat[maxRow][mid + 1];

        if(curr > left && curr > right) {
            return { maxRow, mid };
        } else if(left > curr) {
            high = mid - 1;     // climb left, a peak is guaranteed there
        } else {
            low = mid + 1;      // climb right, a peak is guaranteed there
        }
    }

    return { -1, -1 };  // unreachable: every grid has at least one peak
}

int main() {
    vector<vector<vector<int>>> tests = {
        {{1, 4}, {3, 2}},
        {{10, 20, 15}, {21, 30, 14}, {7, 16, 32}},
        {{4, 10, 7, 3, 1}, {6, 7, 5, 8, 2}, {1, 9, 3, 2, 5}}
    };

    for(auto& mat : tests) {
        cout << "Matrix:" << endl;
        for(auto& row : mat) {
            cout << "  ";
            for(int v : row) cout << v << " ";
            cout << endl;
        }
        vector<int> peak = findPeakGrid(mat);
        cout << "Peak at {" << peak[0] << ", " << peak[1] << "} = "
             << mat[peak[0]][peak[1]] << endl << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: mat = {{4,10,7,3,1}, {6,7,5,8,2}, {1,9,3,2,5}}
    (n = 3, m = 5, answer = {0, 1})
    ==========================================================================

      col:       0   1   2   3   4
      row 0:     4  10   7   3   1
      row 1:     6   7   5   8   2
      row 2:     1   9   3   2   5

    Tracked state:
      low, high - the inclusive column range that still holds a peak
      mid       - the column being probed
      maxRow    - row of the largest value in column mid

    Initial state: low = 0, high = 4

    --------------------------------------------------------------------------
    Iteration 1
      mid          0 + (4 - 0) / 2 = 2
      column 2     {7, 5, 3}   -> maxRow = 0, curr = 7
      neighbours   left = mat[0][1] = 10, right = mat[0][3] = 3
      peak?        7 > 10 ? no
      direction    left 10 > 7  -> climb left
      update       high = mid - 1 = 1

    --------------------------------------------------------------------------
    Iteration 2
      mid          0 + (1 - 0) / 2 = 0
      column 0     {4, 6, 1}   -> maxRow = 1, curr = 6
      neighbours   left = INT_MIN (mid == 0), right = mat[1][1] = 7
      peak?        6 > 7 ? no
      direction    left INT_MIN > 6 ? no -> climb right
      update       low = mid + 1 = 1

    --------------------------------------------------------------------------
    Iteration 3
      mid          1 + (1 - 1) / 2 = 1
      column 1     {10, 7, 9}  -> maxRow = 0, curr = 10
      neighbours   left = mat[0][0] = 4, right = mat[0][2] = 7
      peak?        10 > 4 and 10 > 7  -> YES

    RETURN {0, 1}   (10: up = -inf, down = 7, left = 4, right = 7)

    ==========================================================================
    Summary table
    ==========================================================================

    | iter | low | high | mid | column     | maxRow | curr | left | right | action    |
    |------|-----|------|-----|------------|--------|------|------|-------|-----------|
    |  1   |  0  |  4   |  2  | {7, 5, 3}  |   0    |  7   |  10  |   3   | high = 1  |
    |  2   |  0  |  1   |  0  | {4, 6, 1}  |   1    |  6   | -inf |   7   | low = 1   |
    |  3   |  1  |  1   |  1  | {10, 7, 9} |   0    |  10  |   4  |   7   | return    |

    Bug fix in maxRowInColumn:
      the original code started from maxVal = 0, maxRow = -1 and only updated
      when a value was strictly greater than maxVal. For a column whose
      values are all <= 0, maxRow stayed -1 and mat[-1][mid] read out of
      bounds. Starting from row 0 and comparing against the real column
      values works for any integers. (LeetCode's constraints keep values
      >= 1, so it passed there, but it was not safe in general.)

    Why the column max, not any cell in column mid:
      the column max is what makes up and down safe, so only one direction
      (left or right) has to be decided. Any other cell might have a bigger
      value right above or below it, and the left/right argument would no
      longer hold.
*/
