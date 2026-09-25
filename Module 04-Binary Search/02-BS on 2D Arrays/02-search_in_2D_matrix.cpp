#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an n x m matrix where each row is sorted in ascending order and the
// first element of each row is greater than the last element of the previous
// row, return true if x is present in the matrix, otherwise false.
//
// Example:
// matrix = {{ 1,  3,  5,  7},
//           {10, 11, 16, 20},
//           {23, 30, 34, 60}}
// x = 3   -> true
// x = 13  -> false

/*
    Approach: Binary search for the row, then binary search inside that row
    - The rows are ordered too: all of row i comes before all of row i + 1.
      So each row covers the range [row[0], row[m - 1]], and these ranges
      do not overlap.
    - Binary search over the rows, comparing x with the range of the middle
      row:
        x inside  [matrix[mid][0], matrix[mid][m-1]] -> only this row can hold x
        x greater than matrix[mid][m-1]              -> x is in a lower row
        x less than matrix[mid][0]                   -> x is in a higher row
    - Once the row is found, run a normal binary search on it. If x is not
      there, it is not anywhere, because no other row covers that range.
    - Alternative: treat the matrix as one sorted array of n * m elements
      and map index k to matrix[k / m][k % m]. That is O(log(n * m)), which
      is the same as O(log n + log m).

    Algorithm Steps
    ----------------
    1. low = 0, high = n - 1 over the rows.
    2. While low <= high, mid = low + (high - low) / 2:
         a. If x lies within row mid's range, return searchRow(matrix[mid], x).
         b. Else if x > last element of row mid, set low = mid + 1.
         c. Else set high = mid - 1.
    3. No row's range contains x, so return false.

    Time Complexity: O(log n + log m) - one binary search over rows, one within a row
    Space Complexity: O(1)
*/
bool searchRow(vector<int>& row, int x) {
    int low = 0, high = row.size() - 1;

    // Classic binary search on a sorted row
    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(row[mid] == x) {
            return true;
        } else if(row[mid] > x) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return false;
}

bool searchMatrix(vector<vector<int>>& matrix, int x) {
    int n = matrix.size();
    int m = matrix[0].size();
    int low = 0, high = n - 1;

    // Binary search over rows using each row's [first, last] range
    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(x >= matrix[mid][0] && x <= matrix[mid][m - 1]) {
            return searchRow(matrix[mid], x);     // only this row can contain x
        } else if(x > matrix[mid][m - 1]) {
            low = mid + 1;                          // x is in a lower row
        } else {
            high = mid - 1;                         // x is in a higher row
        }
    }

    return false;
}

int main() {
    vector<vector<int>> matrix = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};

    cout << "Matrix:" << endl;
    for(auto& row : matrix) {
        cout << "  ";
        for(int v : row) cout << v << " ";
        cout << endl;
    }

    for(int x : {3, 13, 60, 0}) {
        cout << "Search " << x << ": " << (searchMatrix(matrix, x) ? "true" : "false") << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: x = 3 and x = 13   (n = 3, m = 4)
    ==========================================================================

      row 0:     1   3   5   7      range [ 1,  7]
      row 1:    10  11  16  20      range [10, 20]
      row 2:    23  30  34  60      range [23, 60]

    Tracked state:
      low, high, mid - bounds of the search over rows, then over columns

    --------------------------------------------------------------------------
    x = 3
      Row search
        low=0 high=2   mid=1   range [10, 20]   3 < 10    -> high = 0
        low=0 high=0   mid=0   range [ 1,  7]   3 inside  -> searchRow(row 0)

      searchRow({1, 3, 5, 7}, 3)
        low=0 high=3   mid=1   row[1] = 3 == 3            -> return true

    RETURN true

    --------------------------------------------------------------------------
    x = 13
      Row search
        low=0 high=2   mid=1   range [10, 20]   13 inside -> searchRow(row 1)

      searchRow({10, 11, 16, 20}, 13)
        low=0 high=3   mid=1   row[1] = 11 < 13           -> low  = 2
        low=2 high=3   mid=2   row[2] = 16 > 13           -> high = 1
        low > high, loop ends                              -> return false

    RETURN false - rows 0 and 2 are never checked, because 13 can only be
    in row 1's range.

    ==========================================================================
    Summary table (x = 13)
    ==========================================================================

    | phase | low | high | mid | compared with    | update       |
    |-------|-----|------|-----|------------------|--------------|
    | rows  |  0  |  2   |  1  | [10, 20]         | pick row 1   |
    | row 1 |  0  |  3   |  1  | 11 < 13          | low = 2      |
    | row 1 |  2  |  3   |  2  | 16 > 13          | high = 1     |
    | row 1 |  2  |  1   |  -  | low > high       | return false |

    Why returning searchRow's result directly is correct:
      the row ranges are disjoint and in order, so if x falls in row mid's
      range, no other row could contain x. A miss inside that row is a miss
      for the whole matrix.

    Values outside every range:
      x = 0 keeps moving high down until high = -1. x = 100 keeps moving low
      up until low = n. Both end the row loop and return false without ever
      calling searchRow.
*/
