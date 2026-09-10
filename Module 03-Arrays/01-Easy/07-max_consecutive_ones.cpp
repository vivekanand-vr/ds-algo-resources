#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given a binary array, find the maximum number of consecutive 1's in the array

/*
    Approach: Single Pass Counter
    - Keep a running counter of the current streak of consecutive 1s
    - Every time a 1 is seen, increment the counter and update the max
    - Every time a 0 is seen, reset the counter to 0

    Algorithm Steps
    ----------------
    1. Initialize curr = 0, mx = 0
    2. Traverse the array
    3. If the element is 1, increment curr and update mx = max(mx, curr)
    4. If the element is 0, reset curr to 0
    5. Return mx

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
int findMaxConsecutiveOnes(vector<int>& arr) {
    int curr = 0, mx = 0;
    for (int num : arr) {
        if (num == 1) {
            curr++;
            // Increment count and update maximum
            mx = max(mx, curr);
        } else {
            // Reset the counter
            curr = 0;
        }
    }
    return mx;
}

int main() {
    vector<int> arr = {1, 1, 0, 1, 1, 1, 0, 1, 1};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Max consecutive ones: " << findMaxConsecutiveOnes(arr) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {1, 1, 0, 1, 1, 1, 0, 1, 1}   (n = 9, answer = 3)
    ==========================================================================

      index:   0   1   2   3   4   5   6   7   8
      value:   1   1   0   1   1   1   0   1   1

    Tracked state:
      curr - length of the streak of 1s ending exactly at the current index
      mx   - the longest streak seen anywhere so far (the answer)

    Initial state: curr = 0, mx = 0

    --------------------------------------------------------------------------
    i = 0, num = 1
      compare      num == 1 -> extend the streak
      count        curr = 0 + 1 = 1
      measure      mx = max(0, 1) = 1

    --------------------------------------------------------------------------
    i = 1, num = 1
      compare      num == 1 -> extend the streak
      count        curr = 1 + 1 = 2
      measure      mx = max(1, 2) = 2                       <-- best so far

    --------------------------------------------------------------------------
    i = 2, num = 0
      compare      num != 1 -> the streak is broken
      reset        curr = 0        (mx is NOT touched - it remembers 2)

    --------------------------------------------------------------------------
    i = 3, num = 1     curr = 1,  mx = max(2, 1) = 2
    i = 4, num = 1     curr = 2,  mx = max(2, 2) = 2
    i = 5, num = 1     curr = 3,  mx = max(2, 3) = 3        <-- new best

                       1   1   0   1   1   1   0   1   1
                                   \___________/
                                   the winning run, curr = 3

    --------------------------------------------------------------------------
    i = 6, num = 0
      compare      num != 1 -> streak broken
      reset        curr = 0        (mx stays 3)

    --------------------------------------------------------------------------
    i = 7, num = 1     curr = 1,  mx = max(3, 1) = 3
    i = 8, num = 1     curr = 2,  mx = max(3, 2) = 3   -> loop ends

    --------------------------------------------------------------------------
    RETURN mx = 3        (the run arr[3..5])

    ==========================================================================
    Summary table
    ==========================================================================

    | i | num | action | curr | mx |
    |---|-----|--------|------|----|
    | 0 |  1  | count  |  1   | 1  |
    | 1 |  1  | count  |  2   | 2  |
    | 2 |  0  | reset  |  0   | 2  |
    | 3 |  1  | count  |  1   | 2  |
    | 4 |  1  | count  |  2   | 2  |
    | 5 |  1  | count  |  3   | 3  |
    | 6 |  0  | reset  |  0   | 3  |
    | 7 |  1  | count  |  1   | 3  |
    | 8 |  1  | count  |  2   | 3  |

    Why one pass is enough:
      9 elements, 9 iterations, O(1) work each - no rescanning. The trick is
      that curr only ever needs the streak ending HERE: a longer run further
      left cannot be extended past a 0, and mx has already banked it. Note
      the final streak (curr = 2) is shorter than the best - which is exactly
      why mx must be a separate variable and must never be reset.
*/
