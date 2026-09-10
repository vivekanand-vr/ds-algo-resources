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

/*
    ======================================================================
    DRY RUN: arr = { {1,2,3}, {4,5,6}, {7,8,9} }
    (n = 3, m = 3, m*n = 9, answer = 1 2 3 6 9 8 7 4 5)
    ======================================================================

      col:      0   1   2
      row 0:    1   2   3
      row 1:    4   5   6
      row 2:    7   8   9

    Tracked state:
      top, bottom  - first and last row still unvisited
      left, right  - first and last column still unvisited
      ind          - next free slot in res; also the count emitted so far
      res          - output of size m*n, filled front to back

    Initial state:
      left = 0, right = m-1 = 2, top = 0, bottom = n-1 = 2
      ind = 0, res = [_ _ _ _ _ _ _ _ _]

    The live region is always the rectangle rows top..bottom crossed with
    cols left..right. Each of the four inner loops peels ONE edge off that
    rectangle and then moves its own boundary inward, so the rectangle
    strictly shrinks and no cell is ever emitted twice.

    ----------------------------------------------------------------------
    OUTER ITERATION 1     (ind = 0 < 9, rectangle = rows 0..2, cols 0..2)

      live rectangle:
              0   1   2
        0 [   1   2   3 ]
        1 [   4   5   6 ]
        2 [   7   8   9 ]

      step 1: TOP row, left -> right, at row top = 0
              i = 0: res[0] = arr[0][0] = 1
              i = 1: res[1] = arr[0][1] = 2
              i = 2: res[2] = arr[0][2] = 3
              ind = 3;  top: 0 -> 1
              res = [1 2 3 _ _ _ _ _ _]
              bounds now: top=1 bottom=2 left=0 right=2

      step 2: RIGHT column, top -> bottom, at col right = 2
              i = 1: res[3] = arr[1][2] = 6
              i = 2: res[4] = arr[2][2] = 9
              ind = 5;  right: 2 -> 1
              res = [1 2 3 6 9 _ _ _ _]
              bounds now: top=1 bottom=2 left=0 right=1

              (i starts at the ALREADY-INCREMENTED top = 1, which is why
               the corner 3 is not emitted twice)

      step 3: BOTTOM row, right -> left, at row bottom = 2
              i = 1: res[5] = arr[2][1] = 8
              i = 0: res[6] = arr[2][0] = 7
              ind = 7;  bottom: 2 -> 1
              res = [1 2 3 6 9 8 7 _ _]
              bounds now: top=1 bottom=1 left=0 right=1

      step 4: LEFT column, bottom -> top, at col left = 0
              i = 1: res[7] = arr[1][0] = 4
              (i = 0 is not reached: 0 >= top = 1 is false)
              ind = 8;  left: 0 -> 1
              res = [1 2 3 6 9 8 7 4 _]
              bounds now: top=1 bottom=1 left=1 right=1

      the ring is peeled, one cell remains:

              0   1   2
        0     .   .   .
        1     .  [5]  .
        2     .   .   .

    ----------------------------------------------------------------------
    OUTER ITERATION 2     (ind = 8 < 9, rectangle = row 1..1, col 1..1)

      step 1: TOP row, left = 1 -> right = 1, at row top = 1
              i = 1: res[8] = arr[1][1] = 5
              ind = 9;  top: 1 -> 2
              res = [1 2 3 6 9 8 7 4 5]      <-- full

      step 2: RIGHT column, i = top = 2 to bottom = 1
              2 <= 1 is false -> ZERO iterations
              right: 1 -> 0

      step 3: BOTTOM row, i = right = 0, needs i >= left = 1
              0 >= 1 is false -> ZERO iterations
              bottom: 1 -> 0

      step 4: LEFT column, i = bottom = 0, needs i >= top = 2
              0 >= 2 is false -> ZERO iterations
              left: 1 -> 2

      loop test: ind = 9 < 9 is false -> exit

    ----------------------------------------------------------------------
    RETURN res = 1 2 3 6 9 8 7 4 5

      the path taken:

              1 -> 2 -> 3
                        |
              4 -> 5    6
              ^         |
              7 <- 8 <- 9

    ======================================================================
    Summary table
    ======================================================================

    | it | edge   | cells emitted | ind | top | bottom | left | right |
    |----|--------|---------------|-----|-----|--------|------|-------|
    | -  | init   | -             |  0  |  0  |   2    |  0   |   2   |
    | 1  | top    | 1, 2, 3       |  3  |  1  |   2    |  0   |   2   |
    | 1  | right  | 6, 9          |  5  |  1  |   2    |  0   |   1   |
    | 1  | bottom | 8, 7          |  7  |  1  |   1    |  0   |   1   |
    | 1  | left   | 4             |  8  |  1  |   1    |  1   |   1   |
    | 2  | top    | 5             |  9  |  2  |   1    |  1   |   1   |
    | 2  | right  | (none)        |  9  |  2  |   1    |  1   |   0   |
    | 2  | bottom | (none)        |  9  |  2  |   0    |  1   |   0   |
    | 2  | left   | (none)        |  9  |  2  |   0    |  2   |   0   |

    Step count backing O(m*n): 9 stores into res, one per cell, plus 8
    boundary updates and 4 loop-condition checks that emitted nothing.
    Every cell is read exactly once - the shrinking rectangle guarantees
    it, so no visited[][] grid is needed. Space is O(m*n) for res itself
    and O(1) beyond that.

    The one subtlety - why the bottom and left loops carry an extra
    `ind < m*n` guard but the top and right loops do not:
      when only a SINGLE row is left (top == bottom), the top-row loop
      already emitted it left-to-right; without the guard the bottom-row
      loop would emit that same row again right-to-left and run off the
      end of res. Concretely, on a 1x3 matrix {{1,2,3}}: the top loop
      fills res[0..2] and sets top = 1, then the bottom loop has
      i = 1 >= left = 0 satisfied and would write res[3] and res[4] -
      out of bounds. The `ind < m*n` test is what stops it. The top and
      right loops need no such guard because by the time res is full,
      their ranges (left..right and top..bottom) are already empty, as
      iteration 2 above shows.
*/
