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

/*
    ==========================================================================
    DRY RUN: n = 5   (5 rows, answer = [[1],[1,1],[1,2,1],[1,3,3,1],
                                        [1,4,6,4,1]])
    ==========================================================================

    Target shape (row index i on the left, column index j across the top):

               j:   0   1   2   3   4
      i = 0         1
      i = 1         1   1
      i = 2         1   2   1
      i = 3         1   3   3   1
      i = 4         1   4   6   4   1

    Tracked state:
      i    - index of the row currently being built (0 .. n-1)
      row  - the row under construction, created as (i+1) copies of 1
      j    - interior column being filled, runs 1 .. i-1 only
      res  - all completed rows; res[i-1] is the row directly above

    Key point about the loop bounds: `row(i + 1, 1)` pre-fills the WHOLE row
    with 1s, and the inner loop `for (j = 1; j < i; ++j)` deliberately never
    touches j = 0 or j = i. So the two `1` ends are never computed - they are
    left over from the initialization. Only the interior is overwritten.

    Initial state: res = [] (empty)

    --------------------------------------------------------------------------
    i = 0
      create       row(0 + 1, 1)            -> row = [1]          (1 element)
      interior     j = 1; 1 < 0 is false    -> inner loop body never runs
      ends         row[0] = 1 stays as initialized
      store        res.push_back([1])       -> res = [[1]]

    --------------------------------------------------------------------------
    i = 1
      create       row(1 + 1, 1)            -> row = [1, 1]       (2 elements)
      interior     j = 1; 1 < 1 is false    -> inner loop body never runs
      ends         row[0] = 1, row[1] = 1, both as initialized
      store        res = [[1], [1,1]]

    --------------------------------------------------------------------------
    i = 2      <-- FIRST row that has an interior at all
      create       row(3, 1)                -> row = [1, 1, 1]
      interior     j runs over {1} only (j = 1; 1 < 2 true, then 2 < 2 false)

                   step 1: j = 1
                     row[1] = res[1][0] + res[1][1]
                            = 1 + 1 = 2
                     row = [1, 2, 1]

                       above (res[1]):   1     1
                                          \   /
                       row[1]:              2

      ends         row[0] = 1 (untouched), row[2] = 1 (untouched)
      store        res = [[1], [1,1], [1,2,1]]

    --------------------------------------------------------------------------
    i = 3
      create       row(4, 1)                -> row = [1, 1, 1, 1]
      interior     j runs over {1, 2}

                   step 1: j = 1
                     row[1] = res[2][0] + res[2][1] = 1 + 2 = 3
                     row = [1, 3, 1, 1]
                   step 2: j = 2
                     row[2] = res[2][1] + res[2][2] = 2 + 1 = 3
                     row = [1, 3, 3, 1]

                       above (res[2]):   1     2     1
                                          \   / \   /
                       row:                 3     3

      ends         row[0] = 1, row[3] = 1 (both untouched)
      store        res = [[1], [1,1], [1,2,1], [1,3,3,1]]

    --------------------------------------------------------------------------
    i = 4
      create       row(5, 1)                -> row = [1, 1, 1, 1, 1]
      interior     j runs over {1, 2, 3}

                   step 1: j = 1
                     row[1] = res[3][0] + res[3][1] = 1 + 3 = 4
                     row = [1, 4, 1, 1, 1]
                   step 2: j = 2
                     row[2] = res[3][1] + res[3][2] = 3 + 3 = 6
                     row = [1, 4, 6, 1, 1]
                   step 3: j = 3
                     row[3] = res[3][2] + res[3][3] = 3 + 1 = 4
                     row = [1, 4, 6, 4, 1]

                       above (res[3]):   1     3     3     1
                                          \   / \   / \   /
                       row:                 4     6     4

      ends         row[0] = 1, row[4] = 1 (both untouched)
      store        res = [[1], [1,1], [1,2,1], [1,3,3,1], [1,4,6,4,1]]
      advance      i -> 5, 5 < 5 is false -> outer loop ends

    --------------------------------------------------------------------------
    RETURN res = [[1], [1,1], [1,2,1], [1,3,3,1], [1,4,6,4,1]]

    ==========================================================================
    Summary table
    ==========================================================================

    | i | size | j values | interior sums taken from res[i-1] | row built   |
    |---|------|----------|-----------------------------------|-------------|
    | 0 |  1   | none     | -                                 | [1]         |
    | 1 |  2   | none     | -                                 | [1,1]       |
    | 2 |  3   | 1        | 1+1=2                             | [1,2,1]     |
    | 3 |  4   | 1,2      | 1+2=3, 2+1=3                      | [1,3,3,1]   |
    | 4 |  5   | 1,2,3    | 1+3=4, 3+3=6, 3+1=4               | [1,4,6,4,1] |

    Step count behind the O(n^2) claim:
      the inner loop ran 0 + 0 + 1 + 2 + 3 = 6 times for n = 5, while the
      rows themselves hold 1 + 2 + 3 + 4 + 5 = 15 numbers. Both are of the
      order n(n-1)/2 and n(n+1)/2 respectively, i.e. O(n^2) - the nine
      untouched `1` slots were paid for by the vector fill, not by additions.

    The subtlety to notice:
      res[i - 1] is read WHILE row i is being written. That is safe only
      because `row` is a separate local vector that is pushed into res after
      it is finished. If the code tried to update res[i] in place from the
      left, row[j-1] would already have been overwritten by the time it was
      needed, and every value past the first interior slot would be wrong.
*/
