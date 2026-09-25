#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given a binary matrix mat of size n x m where every row is sorted
// (all 0s come before all 1s), return the index of the first row that has
// the maximum number of 1s. If no row contains a 1, return -1.
//
// Example:
// mat = {{0, 1, 1, 1},
//        {0, 0, 1, 1},
//        {1, 1, 1, 1},
//        {0, 0, 0, 0}}            -> 2   (row 2 has four 1s)
// mat = {{0, 0}, {1, 1}}          -> 1
// mat = {{0, 0}, {0, 0}}          -> -1  (no 1s anywhere)

/*
    Approach: Binary search inside each row for the first 1
    - Each row is sorted, so its 1s form one block at the right end. If the
      first 1 is at index idx, the row holds m - idx ones.
    - Finding that first 1 is a lower-bound search: whenever row[mid] == 1,
      mid could be the first 1, so keep it (high = mid). Otherwise the first
      1 must be to the right (low = mid + 1).
    - The rows are NOT sorted relative to each other, so there is no way to
      binary search over the rows. Every row gets visited once.
    - Use a strict > when updating the best row, so on a tie the EARLIER
      row wins.
    - Alternative: start at the top-right corner and step left on a 1, down
      on a 0 (staircase walk). That runs in O(n + m), which beats
      O(n log m) when m is large.

    Algorithm Steps
    ----------------
    1. Set ansRow = -1, maxOnes = 0.
    2. For each row i, count its 1s with a binary search for the first 1:
         a. low = 0, high = m - 1. While low < high, mid = low + (high - low) / 2
         b. If row[mid] == 1, set high = mid. Otherwise set low = mid + 1.
         c. If row[low] == 1, the row has m - low ones. Otherwise it has 0.
    3. If this count is greater than maxOnes, update maxOnes and ansRow = i.
    4. Return ansRow.

    Time Complexity: O(n log m) - one O(log m) binary search for each of the n rows
    Space Complexity: O(1)
*/
int countOnes(vector<int>& row) {
    int m = row.size();
    int low = 0, high = m - 1;

    // Lower bound for the first 1
    while(low < high) {
        int mid = low + (high - low) / 2;
        if(row[mid] == 1) {
            high = mid;         // mid might be the first 1, so keep it
        } else {
            low = mid + 1;      // first 1 is strictly to the right
        }
    }

    // low is the first 1, unless the row has no 1s at all
    return row[low] == 1 ? m - low : 0;
}

int rowWithMax1s(vector<vector<int>>& mat) {
    int n = mat.size();
    int ansRow = -1, maxOnes = 0;

    // Rows are not sorted against each other, so visit every row
    for(int i = 0; i < n; ++i) {
        int ones = countOnes(mat[i]);
        if(ones > maxOnes) {    // strict > keeps the first row on ties
            maxOnes = ones;
            ansRow = i;
        }
    }

    return ansRow;
}

int main() {
    vector<vector<vector<int>>> tests = {
        {{0, 1, 1, 1}, {0, 0, 1, 1}, {1, 1, 1, 1}, {0, 0, 0, 0}},
        {{0, 0}, {1, 1}},
        {{0, 0}, {0, 0}}
    };

    for(auto& mat : tests) {
        cout << "Matrix:" << endl;
        for(auto& row : mat) {
            cout << "  ";
            for(int x : row) cout << x << " ";
            cout << endl;
        }
        cout << "Row with max 1s: " << rowWithMax1s(mat) << endl << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: mat = {{0,1,1,1}, {0,0,1,1}, {1,1,1,1}, {0,0,0,0}}
    (n = 4, m = 4, answer = 2)
    ==========================================================================

      col:      0  1  2  3
      row 0:    0  1  1  1
      row 1:    0  0  1  1
      row 2:    1  1  1  1
      row 3:    0  0  0  0

    Tracked state:
      ansRow, maxOnes - the best row found so far and its count of 1s
      low, high, mid  - bounds of the lower-bound search inside one row

    Initial state: ansRow = -1, maxOnes = 0

    --------------------------------------------------------------------------
    Row 0 = {0, 1, 1, 1}
      low=0 high=3   mid=1   row[1]=1   -> high = 1
      low=0 high=1   mid=0   row[0]=0   -> low  = 1
      stop at low = 1, row[1] = 1       -> ones = 4 - 1 = 3
      3 > 0                             -> maxOnes = 3, ansRow = 0

    --------------------------------------------------------------------------
    Row 1 = {0, 0, 1, 1}
      low=0 high=3   mid=1   row[1]=0   -> low  = 2
      low=2 high=3   mid=2   row[2]=1   -> high = 2
      stop at low = 2, row[2] = 1       -> ones = 4 - 2 = 2
      2 > 3 ? no                        -> no change

    --------------------------------------------------------------------------
    Row 2 = {1, 1, 1, 1}
      low=0 high=3   mid=1   row[1]=1   -> high = 1
      low=0 high=1   mid=0   row[0]=1   -> high = 0
      stop at low = 0, row[0] = 1       -> ones = 4 - 0 = 4
      4 > 3                             -> maxOnes = 4, ansRow = 2

    --------------------------------------------------------------------------
    Row 3 = {0, 0, 0, 0}
      low=0 high=3   mid=1   row[1]=0   -> low  = 2
      low=2 high=3   mid=2   row[2]=0   -> low  = 3
      stop at low = 3, row[3] = 0       -> no 1s, ones = 0
      0 > 4 ? no                        -> no change

    --------------------------------------------------------------------------
    RETURN 2

    ==========================================================================
    Summary table
    ==========================================================================

    | row | contents  | first 1 | ones | maxOnes after | ansRow after |
    |-----|-----------|---------|------|---------------|--------------|
    |  0  | 0 1 1 1   |    1    |  3   |       3       |      0       |
    |  1  | 0 0 1 1   |    2    |  2   |       3       |      0       |
    |  2  | 1 1 1 1   |    0    |  4   |       4       |      2       |
    |  3  | 0 0 0 0   |  none   |  0   |       4       |      2       |

    Why check row[low] after the loop:
      the search always stops on SOME index, even when the row has no 1s. In
      that case it walks all the way to m - 1 and row[m - 1] is still 0.
      Without the check, an all-zero row would be counted as having one 1.

    Why maxOnes starts at 0 and not -1:
      a row with zero 1s must never become the answer. Starting at 0 with a
      strict > means ansRow stays -1 unless some row has at least one 1.
*/
