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

/*
    ======================================================================
    DRY RUN: arr = { {0,1,2,0}, {3,4,5,2}, {1,3,1,5} }
    (n = 3 rows, m = 4 cols, answer = row 0, col 0 and col 3 all zeroed)

    Note: main() uses the 3x3 { {1,1,1}, {1,0,1}, {1,1,1} } (which gives
    { {1,0,1}, {0,0,0}, {1,0,1} }). That input never touches rowZero and
    never triggers steps 3 or 4, so this trace uses a 3x4 whose zeros sit
    IN the first row - the case the marker scheme actually has to work
    around.
    ======================================================================

      col:      0   1   2   3
      row 0:    0   1   2   0        <-- two zeros, both in the first row
      row 1:    3   4   5   2
      row 2:    1   3   1   5

    Tracked state:
      arr[i][0]  - marker: "row i must be zeroed"   (for i >= 1)
      arr[0][j]  - marker: "col j must be zeroed"   (for all j)
      rowZero    - flag: 0 means the first row must be zeroed.
                   Needed because arr[0][0] is shared by BOTH marker
                   strips - it cannot mean "row 0" and "col 0" at once,
                   so the row-0 meaning is moved out into this flag and
                   arr[0][0] keeps only the col-0 meaning.

    Initial state: n = 3, m = 4, rowZero = 1, matrix as above

    ----------------------------------------------------------------------
    STEP 1 - marking pass (i = 0..2, j = 0..3)

      i = 0, j = 0:  arr[0][0] == 0  -> ZERO FOUND
                       row mark: i == 0, so rowZero = 0   (not arr[0][0])
                       col mark: arr[0][0] = 0            (already 0)
      i = 0, j = 1:  1, not zero
      i = 0, j = 2:  2, not zero
      i = 0, j = 3:  arr[0][3] == 0  -> ZERO FOUND
                       row mark: i == 0, so rowZero = 0   (already 0)
                       col mark: arr[0][3] = 0            (already 0)
      i = 1:         3, 4, 5, 2 - no zeros, nothing marked
      i = 2:         1, 3, 1, 5 - no zeros, nothing marked

      grid after the marking pass (numerically UNCHANGED here - both
      zeros were already sitting in the marker strip):

          col:      0   1   2   3
          row 0:    0   1   2   0
          row 1:    3   4   5   2
          row 2:    1   3   1   5

      read as markers:
          arr[0][0] = 0 -> col 0 must be zeroed
          arr[0][3] = 0 -> col 3 must be zeroed
          arr[1][0] = 3, arr[2][0] = 1 -> rows 1 and 2 not marked
          rowZero  = 0  -> row 0 must be zeroed

    ----------------------------------------------------------------------
    STEP 2 - write-back for the interior (i = 1..2, j = 1..3)

      This loop deliberately SKIPS row 0 and col 0 so the markers stay
      readable the whole way through.

      i = 1, j = 1:  arr[1][0] = 3, arr[0][1] = 1  -> neither 0, keep 4
      i = 1, j = 2:  arr[1][0] = 3, arr[0][2] = 2  -> neither 0, keep 5
      i = 1, j = 3:  arr[0][3] == 0                -> arr[1][3] = 0
                                                      (was 2)
      i = 2, j = 1:  arr[2][0] = 1, arr[0][1] = 1  -> neither 0, keep 3
      i = 2, j = 2:  arr[2][0] = 1, arr[0][2] = 2  -> neither 0, keep 1
      i = 2, j = 3:  arr[0][3] == 0                -> arr[2][3] = 0
                                                      (was 5)

      grid after step 2:

          col:      0   1   2   3
          row 0:    0   1   2   0
          row 1:    3   4   5   0        <-- (1,3): 2 -> 0
          row 2:    1   3   1   0        <-- (2,3): 5 -> 0
                    ^                ^
                 markers          col 3 done

    ----------------------------------------------------------------------
    STEP 3 - first column, gated on arr[0][0]

      test     arr[0][0] == 0  ->  true, so col 0 is zeroed
      writes   i = 0: arr[0][0] = 0   (already 0)
               i = 1: arr[1][0] = 0   (was 3)
               i = 2: arr[2][0] = 0   (was 1)

      grid after step 3:

          col:      0   1   2   3
          row 0:    0   1   2   0
          row 1:    0   4   5   0        <-- (1,0): 3 -> 0
          row 2:    0   3   1   0        <-- (2,0): 1 -> 0

    ----------------------------------------------------------------------
    STEP 4 - first row, gated on rowZero

      test     rowZero == 0  ->  true, so row 0 is zeroed
      writes   j = 0..3: arr[0][0..3] = 0   (1 and 2 overwritten)

      grid after step 4  -  FINAL:

          col:      0   1   2   3
          row 0:    0   0   0   0
          row 1:    0   4   5   0
          row 2:    0   3   1   0

    ----------------------------------------------------------------------
    FINAL MATRIX (setZeroes returns void; arr is mutated in place)

          0   0   0   0
          0   4   5   0
          0   3   1   0

      Check against the original zeros at (0,0) and (0,3): row 0 gone,
      col 0 gone, col 3 gone, and the untouched 4, 5, 3, 1 survive.

    ======================================================================
    Summary table
    ======================================================================

    | phase  | cells written        | grid after (rows)                |
    |--------|----------------------|----------------------------------|
    | init   | -                    | 0 1 2 0 / 3 4 5 2 / 1 3 1 5      |
    | step 1 | none (rowZero = 0)   | 0 1 2 0 / 3 4 5 2 / 1 3 1 5      |
    | step 2 | (1,3), (2,3)         | 0 1 2 0 / 3 4 5 0 / 1 3 1 0      |
    | step 3 | (1,0), (2,0)         | 0 1 2 0 / 0 4 5 0 / 0 3 1 0      |
    | step 4 | (0,0)..(0,3)         | 0 0 0 0 / 0 4 5 0 / 0 3 1 0      |

    Step count backing O(n*m): step 1 reads all 12 cells, step 2 visits
    the 2*3 = 6 interior cells, step 3 writes 3 cells and step 4 writes
    4 - about 25 cell touches for a 12-cell matrix, i.e. a small constant
    times n*m. Space is O(1): one int flag, no O(n+m) row/col sets.

    Why the ORDER of steps 3 and 4 matters (the one subtlety):
      Step 4 writes zeros across the whole first row, arr[0][0] included.
      If step 4 ran first, step 3 would then read arr[0][0] == 0 and
      zero the entire first column even when column 0 had no zero at all.
      Take { {1,1,1,0}, {3,4,5,2}, {1,3,1,5} }: only rowZero and
      arr[0][3] get marked, arr[0][0] stays 1, and step 3 correctly does
      nothing - but only because it is asked BEFORE step 4 clobbers the
      cell it depends on. Read all markers, then destroy them.

    Why the marking pass cannot poison itself:
      step 1 writes only into row 0 and column 0. Row 0 is scanned first
      (i = 0) and is never revisited, and cell (i,0) is read at j = 0
      before any marker for row i could be written. So no marker zero is
      ever mistaken for an original zero.
*/
