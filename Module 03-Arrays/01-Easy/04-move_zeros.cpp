#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an integer array nums, move all 0's to the end of it while maintaining the relative order

/*
    Approach: Two Pointer (Stable Partition)
    - Use a pointer "ind" to track the position where the next non-zero
      element should be placed
    - Scan through the array; whenever a non-zero element is found, place it
      at "ind" and advance ind
    - This preserves the relative order of non-zero elements
    - Finally, fill the rest of the array (from ind to end) with 0s

    Algorithm Steps
    ----------------
    1. Initialize ind = 0
    2. Traverse the array; copy each non-zero element to arr[ind] and increment ind
    3. Fill the remaining positions (ind to n-1) with 0

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void moveZeroes(vector<int>& arr) {
    int ind = 0;
    int n = arr.size();

    // Step 1: Move all non-zero elements to the front
    for (int i = 0; i < n; ++i) {
        if (arr[i] != 0) {
            arr[ind++] = arr[i]; // Place non-zero element at next available position
        }
    }

    // Step 2: Fill the rest of the array with 0s
    fill(arr.begin() + ind, arr.end(), 0);
}

int main() {
    vector<int> arr = {0, 1, 0, 3, 12};

    cout << "Before: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    moveZeroes(arr);

    cout << "After:  ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {0, 1, 0, 3, 12}   (n = 5, answer = 1 3 12 0 0)
    ======================================================================

      index:   0   1   2   3   4
      value:   0   1   0   3   12

    Tracked state:
      i    - read cursor, visits every index once
      ind  - write cursor: the slot where the NEXT non-zero goes. Also
             equals "how many non-zeros have been written so far".
      arr  - overwritten in place; positions < ind are already final

    Initial state: ind = 0, arr = 0 1 0 3 12

    ----------------------------------------------------------------------
    i = 0, arr[0] = 0
      test         arr[0] != 0 ?  no
      skip         ind stays 0, array untouched:  0 1 0 3 12

    ----------------------------------------------------------------------
    i = 1, arr[1] = 1
      test         arr[1] != 0 ?  yes
      write        arr[ind=0] = 1,  then ind -> 1
      array        1 1 0 3 12       <-- note index 1 still holds a stale
                                        copy of the 1; that is fine, the
                                        read cursor i has already passed
                                        it and never looks back

    ----------------------------------------------------------------------
    i = 2, arr[2] = 0
      test         arr[2] != 0 ?  no
      skip         ind stays 1, array 1 1 0 3 12

    ----------------------------------------------------------------------
    i = 3, arr[3] = 3
      test         arr[3] != 0 ?  yes
      write        arr[ind=1] = 3,  then ind -> 2
      array        1 3 0 3 12

    ----------------------------------------------------------------------
    i = 4, arr[4] = 12
      test         arr[4] != 0 ?  yes
      write        arr[ind=2] = 12, then ind -> 3
      array        1 3 12 3 12

                       1   3   12  3   12
                                   ^
                                  ind = 3
                       |-- final --|-- garbage --|

    ----------------------------------------------------------------------
    STEP 2 - fill(arr.begin() + 3, arr.end(), 0)
      Everything from ind to n-1 is leftover garbage from the shifting,
      and there were n - ind = 5 - 3 = 2 zeros in the input, so exactly
      that many slots get zeroed.

      overwrite    arr[3] = 0
      overwrite    arr[4] = 0

    ----------------------------------------------------------------------
    FINAL ARRAY: 1 3 12 0 0        (moveZeroes returns void, in place)

    ======================================================================
    Summary table
    ======================================================================

    | i | arr[i] | action        | ind after | array          |
    |---|--------|---------------|-----------|----------------|
    | 0 |   0    | skip          |     0     | 0 1 0 3 12     |
    | 1 |   1    | write to [0]  |     1     | 1 1 0 3 12     |
    | 2 |   0    | skip          |     1     | 1 1 0 3 12     |
    | 3 |   3    | write to [1]  |     2     | 1 3 0 3 12     |
    | 4 |  12    | write to [2]  |     3     | 1 3 12 3 12    |
    | - |   -    | fill [3..4]=0 |     3     | 1 3 12 0 0     |

    Invariant: ind <= i always, so the write cursor never overtakes the
    read cursor and no unread value is ever clobbered. That is why the
    stale duplicates in the tail are harmless.

    Step count: 5 reads plus 3 writes plus 2 fills = 10 operations, one
    pass and a partial second pass - O(n) time, O(1) extra space. Order
    is preserved because non-zeros are written in the same order they are
    read.
*/
