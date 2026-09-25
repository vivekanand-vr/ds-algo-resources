#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an n x m matrix mat where every row is sorted in ascending order
// and n * m is odd, return the median of all its elements.
//
// Example:
// mat = {{1, 3, 5},
//        {2, 6, 9},
//        {3, 6, 9}}              -> 5   (sorted: 1 2 3 3 [5] 6 6 9 9)
// mat = {{1, 3, 8},
//        {2, 3, 4},
//        {1, 2, 5}}              -> 3   (sorted: 1 1 2 2 [3] 3 4 5 8)
// mat = {{1}, {2}, {3}}          -> 2

/*
    Approach: Binary search on the answer (the value, not an index)
    - Let total = n * m. The median is the element at position total / 2
      (0-based) in sorted order. So it is the SMALLEST value v with at least
      required = total / 2 + 1 elements <= v.
    - For any guess mid, count how many elements are <= mid. Every row is
      sorted, so each row's count is an upper-bound binary search (the index
      of the first element > mid). Add these up over all rows.
        count <  required  -> mid is too small, the median is bigger:
                              low = mid + 1
        count >= required  -> mid could be the median, or it is bigger than
                              the median: high = mid
    - The search range is [smallest first element, largest last element].
      Those are the matrix min and max, since every row is sorted.
    - The answer is always a real element of the matrix. The count only
      increases at values that appear in the matrix, so the smallest v that
      reaches required must be one of them.
    - Brute force: copy all elements, sort, take the middle. That is
      O(nm log(nm)) time and O(nm) space. This approach uses O(1) space.

    Algorithm Steps
    ----------------
    1. required = (n * m) / 2 + 1.
    2. low = min of mat[i][0], high = max of mat[i][m - 1] over all rows.
    3. While low < high, mid = low + (high - low) / 2:
         a. count = sum over rows of countLessEqual(row, mid).
         b. If count < required, set low = mid + 1. Otherwise set high = mid.
    4. Return low.

    Time Complexity: O(n log m log(max - min)) - log(max - min) guesses, each
                     running an O(log m) search on every one of the n rows
    Space Complexity: O(1)
*/
int countLessEqual(vector<int>& row, int x) {
    int low = 0, high = row.size();

    // Upper bound: first index whose value is > x
    while(low < high) {
        int mid = low + (high - low) / 2;
        if(row[mid] <= x) {
            low = mid + 1;      // row[mid] is counted, look further right
        } else {
            high = mid;         // row[mid] > x, it might be the first one
        }
    }

    return low;                 // everything before low is <= x
}

int median(vector<vector<int>>& mat) {
    int n = mat.size();
    int m = mat[0].size();
    int required = (n * m) / 2 + 1;     // median needs this many elements <= it

    // Range of possible values: smallest first element to largest last element
    int low = INT_MAX, high = INT_MIN;
    for(int i = 0; i < n; ++i) {
        low  = min(low, mat[i][0]);
        high = max(high, mat[i][m - 1]);
    }

    // Find the smallest value with at least `required` elements <= it
    while(low < high) {
        int mid = low + (high - low) / 2;

        int count = 0;
        for(int i = 0; i < n; ++i) {
            count += countLessEqual(mat[i], mid);
        }

        if(count < required) {
            low = mid + 1;      // too few elements <= mid, median is bigger
        } else {
            high = mid;         // mid is the median or above it
        }
    }

    return low;
}

int main() {
    vector<vector<vector<int>>> tests = {
        {{1, 3, 5}, {2, 6, 9}, {3, 6, 9}},
        {{1, 3, 8}, {2, 3, 4}, {1, 2, 5}},
        {{1}, {2}, {3}}
    };

    for(auto& mat : tests) {
        cout << "Matrix:" << endl;
        for(auto& row : mat) {
            cout << "  ";
            for(int v : row) cout << v << " ";
            cout << endl;
        }
        cout << "Median: " << median(mat) << endl << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: mat = {{1,3,5}, {2,6,9}, {3,6,9}}
    (n = 3, m = 3, total = 9, required = 9 / 2 + 1 = 5, answer = 5)
    ==========================================================================

      row 0:    1   3   5
      row 1:    2   6   9
      row 2:    3   6   9

      sorted:   1   2   3   3  [5]  6   6   9   9
                                ^ 5th smallest = median

    Tracked state:
      low, high - the value range that still holds the median
      mid       - the guessed value
      count     - how many elements are <= mid

    Range setup:
      low  = min(1, 2, 3) = 1
      high = max(5, 9, 9) = 9

    --------------------------------------------------------------------------
    Iteration 1
      bounds       low = 1, high = 9
      mid          1 + (9 - 1) / 2 = 5
      count <= 5   row 0 {1,3,5} -> 3,  row 1 {2,6,9} -> 1,  row 2 {3,6,9} -> 1
                   count = 5
      compare      5 < 5 ? no -> mid could be the median
      update       high = 5

    --------------------------------------------------------------------------
    Iteration 2
      bounds       low = 1, high = 5
      mid          1 + (5 - 1) / 2 = 3
      count <= 3   row 0 -> 2,  row 1 -> 1,  row 2 -> 1
                   count = 4
      compare      4 < 5 -> too few, median is bigger than 3
      update       low = 4

    --------------------------------------------------------------------------
    Iteration 3
      bounds       low = 4, high = 5
      mid          4 + (5 - 4) / 2 = 4
      count <= 4   row 0 -> 2,  row 1 -> 1,  row 2 -> 1
                   count = 4
      compare      4 < 5 -> too few
      update       low = 5

    --------------------------------------------------------------------------
    Loop ends: low = high = 5
    RETURN 5

    Note that mid = 4 is not in the matrix. That's fine: it gives the same
    count as 3, so the search simply moves past it.

    ==========================================================================
    Summary table
    ==========================================================================

    | iter | low | high | mid | row counts | count | vs 5 | update   |
    |------|-----|------|-----|------------|-------|------|----------|
    |  1   |  1  |  9   |  5  | 3 + 1 + 1  |   5   |  >=  | high = 5 |
    |  2   |  1  |  5   |  3  | 2 + 1 + 1  |   4   |  <   | low = 4  |
    |  3   |  4  |  5   |  4  | 2 + 1 + 1  |   4   |  <   | low = 5  |

    ==========================================================================
    Notes
    ==========================================================================

    Why count <= mid, and not < mid:
      with duplicates, "<" would undercount. In {{1,3,8},{2,3,4},{1,2,5}} the
      median is 3 and there are two 3s. Elements < 3 number only 4, but
      elements <= 3 number 6 >= 5, so counting <= lets 3 qualify.

    Why high = mid and not mid - 1:
      count >= required means mid might be exactly the median. Dropping it
      could skip the answer, which is also why the loop runs while
      low < high.

    Why high starts at row.size() in countLessEqual:
      if every element in the row is <= x, the count is the whole row, so
      the search must be able to land on index m (one past the end).

    Overflow:
      mid = low + (high - low) / 2 is safe for normal inputs. If the matrix
      held values near both INT_MIN and INT_MAX, high - low itself could
      overflow; using long long for the bounds would handle that.
*/
