#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array with an equal number of positive and negative elements, rearrange it so
// positive and negative numbers alternate, starting with a positive number

/*
    Approach: Two Pointer Placement
    - Place positive numbers at even indices (0, 2, 4, ...)
    - Place negative numbers at odd indices (1, 3, 5, ...)
    - Preserves the relative order of positives and negatives from the original array

    Algorithm Steps
    ----------------
    1. Initialize pos = 0 and neg = 1 as placement pointers
    2. Traverse the array; if element is positive, place it at res[pos] and pos += 2
    3. If element is negative, place it at res[neg] and neg += 2
    4. Return the result array

    Time Complexity: O(n) - single pass through array
    Space Complexity: O(n) - extra result array
*/
vector<int> rearrangeArray(vector<int>& arr) {
    int pos = 0, neg = 1;         // positions to place positives & negatives
    int n = arr.size();
    vector<int> res(n);           // result array of same size

    // Place positive numbers at even indices (0,2,4,..)
    // Place negative numbers at odd indices (1,3,5,..)
    for (int i : arr) {
        if (i > 0) {
            res[pos] = i;
            pos += 2;
        } else {
            res[neg] = i;
            neg += 2;
        }
    }

    return res;
}

int main() {
    vector<int> arr = {3, 1, -2, -5, 2, -4};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    vector<int> res = rearrangeArray(arr);

    cout << "Rearranged array: ";
    for (int x : res) cout << x << " ";
    cout << endl;

    return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {3,1,-2,-5,2,-4}
    (n = 6, answer = 3 -2 1 -5 2 -4)
    ======================================================================

      index:   0   1   2   3   4   5
      value:   3   1  -2  -5   2  -4
               +   +   -   -   +   -      3 positives, 3 negatives

    Tracked state:
      pos  - next EVEN slot in res waiting for a positive  (0,2,4,...)
      neg  - next ODD  slot in res waiting for a negative  (1,3,5,...)
      res  - the output array, size n, pre-filled with zeros by
             vector<int> res(n) and then overwritten slot by slot

    Initial state: n = 6, pos = 0, neg = 1
                   res = [0, 0, 0, 0, 0, 0]      (all default-constructed)

    Key idea: the two cursors never collide because pos only ever lands
    on even indices and neg only ever on odd indices. Each cursor
    advances by 2, so they interleave without any bookkeeping - no
    shifting, no swapping, no second pass.

    ----------------------------------------------------------------------
    i = 3        (arr[0], positive)
      branch   3 > 0  -> positive branch
      write    res[pos = 0] = 3
      bump     pos: 0 -> 2
      res      [3, 0, 0, 0, 0, 0]        pos = 2, neg = 1

    ----------------------------------------------------------------------
    i = 1        (arr[1], positive)
      branch   1 > 0  -> positive branch
      write    res[pos = 2] = 1          <-- skips index 1, reserved for
                                             a negative that has not been
                                             seen yet
      bump     pos: 2 -> 4
      res      [3, 0, 1, 0, 0, 0]        pos = 4, neg = 1

               Two positives in a row leave a HOLE at index 1. It is not
               a bug - neg still points there and will fill it later:

                   res:   3   _   1   _   _   _
                          ^   ^   ^
                        pos0 neg1 pos2

    ----------------------------------------------------------------------
    i = -2       (arr[2], negative)
      branch   -2 > 0 is false  -> else branch
      write    res[neg = 1] = -2         <-- fills the hole from above
      bump     neg: 1 -> 3
      res      [3, -2, 1, 0, 0, 0]       pos = 4, neg = 3

    ----------------------------------------------------------------------
    i = -5       (arr[3], negative)
      branch   -5 > 0 is false  -> else branch
      write    res[neg = 3] = -5
      bump     neg: 3 -> 5
      res      [3, -2, 1, -5, 0, 0]      pos = 4, neg = 5

    ----------------------------------------------------------------------
    i = 2        (arr[4], positive)
      branch   2 > 0  -> positive branch
      write    res[pos = 4] = 2
      bump     pos: 4 -> 6               (now past the end, and that is
                                          fine - no positive is left)
      res      [3, -2, 1, -5, 2, 0]      pos = 6, neg = 5

    ----------------------------------------------------------------------
    i = -4       (arr[5], negative)
      branch   -4 > 0 is false  -> else branch
      write    res[neg = 5] = -4
      bump     neg: 5 -> 7               (also past the end, unused)
      res      [3, -2, 1, -5, 2, -4]     pos = 6, neg = 7
                                         range-for ends

    ----------------------------------------------------------------------
    RETURN res = 3 -2 1 -5 2 -4

      index:   0   1   2   3   4   5
      value:   3  -2   1  -5   2  -4
               +   -   +   -   +   -      alternating, starts positive

      relative order preserved:
        positives  3, 1, 2   appear in that order at 0, 2, 4
        negatives -2,-5,-4   appear in that order at 1, 3, 5

    ======================================================================
    Summary table
    ======================================================================

    | arr[i]  | sign | slot written | pos | neg | res so far       |
    |---------|------|--------------|-----|-----|------------------|
    | init    |  -   | -            |  0  |  1  | 0 0 0 0 0 0      |
    | 0 ->  3 |  +   | res[0] = 3   |  2  |  1  | 3 0 0 0 0 0      |
    | 1 ->  1 |  +   | res[2] = 1   |  4  |  1  | 3 0 1 0 0 0      |
    | 2 -> -2 |  -   | res[1] = -2  |  4  |  3  | 3 -2 1 0 0 0     |
    | 3 -> -5 |  -   | res[3] = -5  |  4  |  5  | 3 -2 1 -5 0 0    |
    | 4 ->  2 |  +   | res[4] = 2   |  6  |  5  | 3 -2 1 -5 2 0    |
    | 5 -> -4 |  -   | res[5] = -4  |  6  |  7  | 3 -2 1 -5 2 -4   |

    Step count backing O(n): 6 iterations, one comparison and one store
    each - 6 writes for 6 elements, every slot of res written exactly
    once. Space is O(n) for res, which is what buys the single pass: the
    O(1)-space variants need rotations or a swap-based partition and give
    up the "relative order preserved" guarantee.

    The invariant: after processing the first t elements, pos = 2*(number
    of positives seen) and neg = 1 + 2*(number of negatives seen). With
    equal counts of each sign, both cursors finish exactly one step past
    the array (6 and 7), so no slot is left at its default 0 and no write
    goes out of bounds.

    The one subtlety: the branch is `if (i > 0) ... else ...`, so a ZERO
    falls into the else branch and is treated as a negative. Also, the
    "equal number of positives and negatives" precondition is never
    checked - with 4 positives and 2 negatives, pos would reach 6 and
    then res[6] would be an out-of-bounds write. The guarantee lives in
    the problem statement, not in the code.
*/
