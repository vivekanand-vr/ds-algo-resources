#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array find the second largest element without sorting

/*
    Approach: Single Pass Tracking
    - Maintain two variables: largest (mx) and second largest (sec)
    - While scanning, whenever a new largest is found, the old largest becomes
      the second largest
    - Otherwise, if the current element is not equal to the largest but greater
      than the current second largest, update the second largest

    Algorithm Steps
    ----------------
    1. Initialize mx and sec to INT_MIN
    2. Traverse the array once
    3. If arr[i] > mx, shift mx into sec, then update mx
    4. Else if arr[i] > sec and arr[i] != mx, update sec
    5. Return sec (or -1 if no valid second largest exists)

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
int getSecondLargest(vector<int> &arr) {
    int mx = INT_MIN;       
    int sec = INT_MIN;

    // Step 1: Iterate through the array to find the largest and second largest
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] > mx) {
            // If current element is greater than max, update second max and max
            sec = mx;
            mx = arr[i];
        } 
        else if (arr[i] > sec && arr[i] != mx) {
            // If it's not equal to max and greater than current second max, update second max
            sec = arr[i];
        }
    }

    // Step 2: If no valid second largest found (e.g., all elements are equal), return -1
    if (sec == INT_MIN) return -1;

    return sec;
}

int main() {
    vector<int> arr = {8, 8, 7, 6, 5};
    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Second largest element: " << getSecondLargest(arr) << endl;

    return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {8, 8, 7, 6, 5}      (n = 5, answer = 7)
    ======================================================================

      index:   0   1   2   3   4
      value:   8   8   7   6   5

    This input is chosen because the largest value is DUPLICATED. The
    second largest is 7, not 8 - the duplicate must not be allowed to
    slide into sec.

    Tracked state:
      mx  - largest value seen so far
      sec - largest value seen so far that is strictly less than mx
      INT_MIN is used as "nothing seen yet"

    Initial state: mx = INT_MIN, sec = INT_MIN

    ----------------------------------------------------------------------
    i = 0, arr[i] = 8
      branch 1?    8 > mx (INT_MIN) ?  YES
      shift        sec = mx    -> sec = INT_MIN   (old mx demoted)
      promote      mx  = 8     -> mx  = 8

    ----------------------------------------------------------------------
    i = 1, arr[i] = 8      <-- the tricky one: a repeat of the maximum
      branch 1?    8 > mx (8) ?  no, it is EQUAL, not greater
      branch 2?    8 > sec (INT_MIN) ?  yes, but
                   8 != mx (8) ?        NO  -> whole condition false
      keep         nothing changes: mx = 8, sec = INT_MIN

                   the arr[i] != mx guard is what saves us here. Without
                   it, branch 2 would have set sec = 8 and the function
                   would wrongly answer 8.

    ----------------------------------------------------------------------
    i = 2, arr[i] = 7
      branch 1?    7 > mx (8) ?  no
      branch 2?    7 > sec (INT_MIN) ?  yes,  and  7 != mx (8) ?  yes
      write        sec = 7                              <-- the answer
      state        mx = 8, sec = 7

    ----------------------------------------------------------------------
    i = 3, arr[i] = 6
      branch 1?    6 > mx (8) ?   no
      branch 2?    6 > sec (7) ?  no
      skip         mx = 8, sec = 7

    ----------------------------------------------------------------------
    i = 4, arr[i] = 5
      branch 1?    5 > mx (8) ?   no
      branch 2?    5 > sec (7) ?  no
      skip         mx = 8, sec = 7

    ----------------------------------------------------------------------
    Post-loop:   sec = 7, which is not INT_MIN -> no -1 fallback
    RETURN 7

    ======================================================================
    Summary table
    ======================================================================

    | i | arr[i] | 1: > mx? | 2: > sec && != mx? | mx | sec     |
    |---|--------|----------|--------------------|----|---------|
    | - |   -    |    -     |         -          | MIN|  MIN    |
    | 0 |   8    |  yes     |    (not reached)   |  8 |  MIN    |
    | 1 |   8    |  no      |  no (== mx)        |  8 |  MIN    |
    | 2 |   7    |  no      |  yes               |  8 |   7     |
    | 3 |   6    |  no      |  no                |  8 |   7     |
    | 4 |   5    |  no      |  no                |  8 |   7     |

    (MIN = INT_MIN)

    Invariant that makes it correct: after processing index i, mx is the
    maximum of arr[0..i] and sec is the maximum of the values in arr[0..i]
    that are strictly smaller than mx. One pass, 5 iterations, at most two
    comparisons each - O(n) time, two ints of state, O(1) space.

    The -1 fallback fires only when sec never moved off INT_MIN, e.g.
    arr = {8, 8, 8} (every element equals mx) or a single-element array.
*/
