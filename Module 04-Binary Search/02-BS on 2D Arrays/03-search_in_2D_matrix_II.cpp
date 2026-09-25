#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an n x m matrix where each row is sorted in ascending order from
// left to right and each column is sorted in ascending order from top to
// bottom, return true if x is present in the matrix, otherwise false.
// (Unlike Search in 2D Matrix I, a row may start below the previous row's end.)
//
// Example:
// matrix = {{ 1,  4,  7, 11, 15},
//           { 2,  5,  8, 12, 19},
//           { 3,  6,  9, 16, 22},
//           {10, 13, 14, 17, 24},
//           {18, 21, 23, 26, 30}}
// x = 5   -> true
// x = 20  -> false

/*
    Approach: Staircase search from the top-right corner
    - The row ranges overlap here, so binary searching for "the" row does
      not work. Instead, use a cell where one comparison always removes a
      whole row or a whole column.
    - The top-right cell (row 0, col m - 1) is the largest in its row and
      the smallest in its column:
        curr == x  -> found
        curr >  x  -> every cell below curr in this column is even bigger,
                      so the whole column can be skipped: col--
        curr <  x  -> every cell left of curr in this row is even smaller,
                      so the whole row can be skipped: row++
    - Each step removes one row or one column, so after at most n + m steps
      we walk off the grid.
    - The bottom-left corner works the same way (smallest in its row,
      largest in its column). The top-left and bottom-right corners do NOT
      work, since both neighbours move the same way there.
    - Alternative: binary search each row separately, O(n log m). The
      staircase is better unless one dimension is much smaller than the other.

    Algorithm Steps
    ----------------
    1. Start at row = 0, col = m - 1.
    2. While row < n and col >= 0:
         a. If matrix[row][col] == x, return true.
         b. If it is greater than x, move left (col--).
         c. Otherwise move down (row++).
    3. Walked off the grid without a match, so return false.

    Time Complexity: O(n + m) - each step discards a full row or column
    Space Complexity: O(1)
*/
bool searchMatrix(vector<vector<int>>& matrix, int x) {
    int n = matrix.size();
    int m = matrix[0].size();

    // Start at the top-right corner: largest in its row, smallest in its column
    int row = 0, col = m - 1;
    while(row < n && col >= 0) {
        int curr = matrix[row][col];
        if(curr == x) {
            return true;
        } else if(curr > x) {
            col--;      // everything below in this column is even bigger
        } else {
            row++;      // everything left in this row is even smaller
        }
    }

    return false;
}

int main() {
    vector<vector<int>> matrix = {
        {1, 4, 7, 11, 15},
        {2, 5, 8, 12, 19},
        {3, 6, 9, 16, 22},
        {10, 13, 14, 17, 24},
        {18, 21, 23, 26, 30}
    };

    cout << "Matrix:" << endl;
    for(auto& row : matrix) {
        cout << "  ";
        for(int v : row) cout << v << " ";
        cout << endl;
    }

    for(int x : {5, 20}) {
        cout << "Search " << x << ": " << (searchMatrix(matrix, x) ? "true" : "false") << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: x = 5 and x = 20   (n = 5, m = 5)
    ==========================================================================

      col:       0   1   2   3   4
      row 0:     1   4   7  11  15   <- start here (0, 4)
      row 1:     2   5   8  12  19
      row 2:     3   6   9  16  22
      row 3:    10  13  14  17  24
      row 4:    18  21  23  26  30

    Tracked state:
      (row, col) - current cell; everything above it and right of it is
                   already ruled out

    --------------------------------------------------------------------------
    x = 5
      (0,4) curr = 15   15 > 5    -> col = 3
      (0,3) curr = 11   11 > 5    -> col = 2
      (0,2) curr =  7    7 > 5    -> col = 1
      (0,1) curr =  4    4 < 5    -> row = 1
      (1,1) curr =  5    5 == 5   -> return true

    RETURN true  (5 steps)

    --------------------------------------------------------------------------
    x = 20
      (0,4) curr = 15   15 < 20   -> row = 1
      (1,4) curr = 19   19 < 20   -> row = 2
      (2,4) curr = 22   22 > 20   -> col = 3
      (2,3) curr = 16   16 < 20   -> row = 3
      (3,3) curr = 17   17 < 20   -> row = 4
      (4,3) curr = 26   26 > 20   -> col = 2
      (4,2) curr = 23   23 > 20   -> col = 1
      (4,1) curr = 21   21 > 20   -> col = 0
      (4,0) curr = 18   18 < 20   -> row = 5, off the grid

    RETURN false  (9 steps, within the n + m = 10 bound)

    The path for x = 20 traced as a staircase:

                 1   4   7  11 [15]
                 2   5   8  12 [19]
                 3   6   9 [16][22]
                10  13  14 [17]  24
               [18][21][23][26]  30

    ==========================================================================
    Summary table (x = 20)
    ==========================================================================

    | step | (row,col) | curr | vs 20 | move  |
    |------|-----------|------|-------|-------|
    |  1   |  (0,4)    |  15  |   <   | down  |
    |  2   |  (1,4)    |  19  |   <   | down  |
    |  3   |  (2,4)    |  22  |   >   | left  |
    |  4   |  (2,3)    |  16  |   <   | down  |
    |  5   |  (3,3)    |  17  |   <   | down  |
    |  6   |  (4,3)    |  26  |   >   | left  |
    |  7   |  (4,2)    |  23  |   >   | left  |
    |  8   |  (4,1)    |  21  |   >   | left  |
    |  9   |  (4,0)    |  18  |   <   | down  |

    Why not start at the top-left (0, 0):
      it is the smallest in both its row and its column. If curr < x, x
      could be to the right OR below, so neither a row nor a column can be
      thrown away and the search has no clear direction.
*/
