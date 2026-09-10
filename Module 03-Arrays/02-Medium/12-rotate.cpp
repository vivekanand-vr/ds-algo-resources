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

/*
    ======================================================================
    DRY RUN: arr = { {1,2,3}, {4,5,6}, {7,8,9} }
    (n = 3, answer = { {7,4,1}, {8,5,2}, {9,6,3} })
    ======================================================================

      col:      0   1   2
      row 0:    1   2   3
      row 1:    4   5   6
      row 2:    7   8   9

    Tracked state:
      arr   - the matrix, mutated in place by two separate passes
      i, j  - the swap cursors of the transpose pass; the inner loop
              runs j = 0..i, so it touches only the LOWER triangle
              including the diagonal

    Initial state: n = 3, matrix as above

    Why this works: a 90-degree clockwise rotation sends the cell at
    (i,j) to (j, n-1-i). Doing that in one shot needs 4-cycles. Instead
    it is factored into two easy in-place passes:
      transpose      (i,j) -> (j,i)          - reflect in the diagonal
      reverse rows   (j,i) -> (j, n-1-i)     - reflect left-to-right
    Two reflections compose into a rotation, which is why the column
    that used to be at the bottom ends up on the left.

    ----------------------------------------------------------------------
    STEP 1 - transpose:  for i = 0..2, for j = 0..i, swap (i,j) <-> (j,i)

    i = 0
      j = 0:  swap arr[0][0] <-> arr[0][0]      <-- SAME CELL, no-op
              (j <= i means the diagonal is included, so every i costs
               one wasted self-swap; harmless, just not free)

      grid:   1   2   3
              4   5   6
              7   8   9        (unchanged)

    ----------------------------------------------------------------------
    i = 1
      j = 0:  swap arr[1][0] <-> arr[0][1]      4 <-> 2

      grid:   1   4   3
              2   5   6
              7   8   9
                  ^
              the 4 and the 2 have traded places across the diagonal

      j = 1:  swap arr[1][1] <-> arr[1][1]      no-op (diagonal, 5 stays)

    ----------------------------------------------------------------------
    i = 2
      j = 0:  swap arr[2][0] <-> arr[0][2]      7 <-> 3

      grid:   1   4   7
              2   5   6
              3   8   9

      j = 1:  swap arr[2][1] <-> arr[1][2]      8 <-> 6

      grid:   1   4   7
              2   5   8
              3   6   9

      j = 2:  swap arr[2][2] <-> arr[2][2]      no-op (diagonal, 9 stays)

    ----------------------------------------------------------------------
    grid after the whole transpose pass:

          col:      0   1   2
          row 0:    1   4   7
          row 1:    2   5   8
          row 2:    3   6   9

      The old row 1 (4 5 6) is now column 1; the diagonal 1, 5, 9 never
      moved. Note the lower triangle only: had the inner loop been
      j = 0..n-1, every off-diagonal pair would be swapped TWICE and the
      matrix would come back unchanged. `j <= i` is what makes it a
      transpose rather than a very slow no-op.

    ----------------------------------------------------------------------
    STEP 2 - reverse each row

    i = 0:  reverse(1 4 7)
              step 1: swap arr[0][0] <-> arr[0][2]   1 <-> 7  -> 7 4 1
              step 2: cursors meet at arr[0][1] = 4, it stays put

      grid:   7   4   1
              2   5   8
              3   6   9

    i = 1:  reverse(2 5 8)
              step 1: swap arr[1][0] <-> arr[1][2]   2 <-> 8  -> 8 5 2
              step 2: cursors meet at arr[1][1] = 5, it stays put

      grid:   7   4   1
              8   5   2
              3   6   9

    i = 2:  reverse(3 6 9)
              step 1: swap arr[2][0] <-> arr[2][2]   3 <-> 9  -> 9 6 3
              step 2: cursors meet at arr[2][1] = 6, it stays put

      grid:   7   4   1
              8   5   2
              9   6   3

    ----------------------------------------------------------------------
    FINAL MATRIX (rotate returns void; arr is mutated in place)

          col:      0   1   2
          row 0:    7   4   1
          row 1:    8   5   2
          row 2:    9   6   3

      Sanity check on one cell: the 3 started at (0,2). After the
      rotation it should be at (2, n-1-0) = (2,2). It is. And the old
      bottom row 7 8 9 is now the left column, read downward - exactly
      what turning the page clockwise does.

    ======================================================================
    Summary table
    ======================================================================

    | pass      | cursor    | swap performed         | grid after (rows)     |
    |-----------|-----------|------------------------|-----------------------|
    | init      | -         | -                      | 123 / 456 / 789       |
    | transpose | i=0,j=0   | (0,0)<->(0,0)  no-op   | 123 / 456 / 789       |
    | transpose | i=1,j=0   | 4 <-> 2                | 143 / 256 / 789       |
    | transpose | i=1,j=1   | (1,1)<->(1,1)  no-op   | 143 / 256 / 789       |
    | transpose | i=2,j=0   | 7 <-> 3                | 147 / 256 / 389       |
    | transpose | i=2,j=1   | 8 <-> 6                | 147 / 258 / 369       |
    | transpose | i=2,j=2   | (2,2)<->(2,2)  no-op   | 147 / 258 / 369       |
    | reverse   | row 0     | 1 <-> 7                | 741 / 258 / 369       |
    | reverse   | row 1     | 2 <-> 8                | 741 / 852 / 369       |
    | reverse   | row 2     | 3 <-> 9                | 741 / 852 / 963       |

    Step count backing O(n^2): the transpose loop body runs
    1 + 2 + 3 = 6 times (n(n+1)/2) of which 3 are diagonal no-ops, so 3
    real swaps; the reversal pass does 1 swap per row, 3 more. Six swaps,
    twelve element writes, for a 9-cell matrix - linear in the number of
    cells, i.e. O(n^2) in the side length. Space is O(1): every swap is
    between two cells of the caller's own matrix, no second grid.

    The one subtlety: this only works because the matrix is SQUARE.
    Transposing in place needs (i,j) and (j,i) to both exist, and
    reversing rows only lines the result up when the row length equals
    the column count. A rectangular m x n rotation has to allocate a
    new n x m grid.
*/
