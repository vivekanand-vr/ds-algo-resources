#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array with only 0s, 1s, and 2s, sort it in-place (Dutch National Flag problem)

/*
    Approach: Counting
    - Count the number of 0s, 1s, and 2s present in the array
    - Overwrite the array by filling in that many 0s, then 1s, then 2s

    Algorithm Steps
    ----------------
    1. Traverse the array once and count occurrences of 0, 1, and 2
    2. Overwrite the first 'z' positions with 0
    3. Overwrite the next 'o' positions with 1
    4. Overwrite the remaining 't' positions with 2

    Time Complexity: O(n) - first pass to count, second pass to overwrite
    Space Complexity: O(1) - only uses a few integer variables
*/
void sortColors(vector<int>& arr) {
    int z = 0, o = 0, t = 0;  

    // Count the frequency of 0s, 1s, and 2s
    for (int i : arr) {
        switch (i) {
            case 0: z++; break;
            case 1: o++; break;
            case 2: t++; break;
        }
    }

    // Debug print (can be removed in final solution)
    cout << z << o << t;

    // Overwrite the array with the correct number of 0s, then 1s, then 2s
    for (int i = 0; i < z; ++i) arr[i] = 0;           // fill 0s
    for (int i = 0; i < o; ++i) arr[i + z] = 1;       // fill 1s after 0s
    for (int i = 0; i < t; ++i) arr[i + z + o] = 2;   // fill 2s after 1s
}

int main() {
    vector<int> arr = {2, 0, 2, 1, 1, 0};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    sortColors(arr);

    cout << "Sorted colors: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {2, 0, 2, 1, 1, 0}   (n = 6, answer = {0,0,1,1,2,2})
    ==========================================================================

      index:   0   1   2   3   4   5
      value:   2   0   2   1   1   0

    NOTE: this file implements the COUNTING solution (two passes), not the
    one-pass Dutch National Flag three-pointer version, so there is no
    low/mid/high pointer to trace - only three counters and three fills.

    Tracked state:
      z - how many 0s have been counted so far
      o - how many 1s have been counted so far
      t - how many 2s have been counted so far
      (the array itself is untouched during the whole counting pass)

    Initial state: z = 0, o = 0, t = 0, arr = {2,0,2,1,1,0}

    ==========================================================================
    PASS 1 - count the three values
    ==========================================================================

    --------------------------------------------------------------------------
    i = arr[0] = 2
      switch       case 2 -> t++
      counters     z = 0, o = 0, t = 1

    --------------------------------------------------------------------------
    i = arr[1] = 0
      switch       case 0 -> z++
      counters     z = 1, o = 0, t = 1

    --------------------------------------------------------------------------
    i = arr[2] = 2
      switch       case 2 -> t++
      counters     z = 1, o = 0, t = 2

    --------------------------------------------------------------------------
    i = arr[3] = 1
      switch       case 1 -> o++
      counters     z = 1, o = 1, t = 2

    --------------------------------------------------------------------------
    i = arr[4] = 1
      switch       case 1 -> o++
      counters     z = 1, o = 2, t = 2

    --------------------------------------------------------------------------
    i = arr[5] = 0
      switch       case 0 -> z++
      counters     z = 2, o = 2, t = 2      <-- final counts, 2+2+2 = 6 = n

    --------------------------------------------------------------------------
    debug print   cout << z << o << t;  ->  prints "222"

    ==========================================================================
    PASS 2 - overwrite in three blocks
    ==========================================================================

    The counts fix the block boundaries before a single write happens:

        0s occupy [0 .. z-1]        = [0 .. 1]
        1s occupy [z .. z+o-1]      = [2 .. 3]
        2s occupy [z+o .. z+o+t-1]  = [4 .. 5]

           0   0 | 1   1 | 2   2
         |-------|-------|-------|
          z = 2    o = 2   t = 2
          start 0  start 2 start 4

    --------------------------------------------------------------------------
    fill 0s      loop i = 0 .. z-1 = 0 .. 1, writing arr[i] = 0
      step 1:    arr[0] = 0     ->  {0, 0, 2, 1, 1, 0}
      step 2:    arr[1] = 0     ->  {0, 0, 2, 1, 1, 0}   (arr[1] was already 0)
      after      arr = {0, 0, 2, 1, 1, 0}

    --------------------------------------------------------------------------
    fill 1s      loop i = 0 .. o-1 = 0 .. 1, writing arr[i + z] = arr[i + 2]
      step 1:    arr[0 + 2] = arr[2] = 1  ->  {0, 0, 1, 1, 1, 0}
      step 2:    arr[1 + 2] = arr[3] = 1  ->  {0, 0, 1, 1, 1, 0}
      after      arr = {0, 0, 1, 1, 1, 0}

                 <-- this intermediate state is NOT a permutation of the
                     input (three 1s, one 2 lost). That is fine and is the
                     one thing to notice about this approach: the original
                     values are destroyed as soon as writing starts, and
                     correctness rests entirely on the counters taken in
                     pass 1. If pass 1 missed a value (say the array also
                     held a 3, which no case label catches), pass 2 would
                     leave the tail of the array holding stale garbage.

    --------------------------------------------------------------------------
    fill 2s      loop i = 0 .. t-1 = 0 .. 1, writing arr[i + z + o] = arr[i + 4]
      step 1:    arr[0 + 4] = arr[4] = 2  ->  {0, 0, 1, 1, 2, 0}
      step 2:    arr[1 + 4] = arr[5] = 2  ->  {0, 0, 1, 1, 2, 2}
      after      arr = {0, 0, 1, 1, 2, 2}

    --------------------------------------------------------------------------
    FINAL ARRAY  arr = {0, 0, 1, 1, 2, 2}      (in-place, nothing returned)

    ==========================================================================
    Summary table
    ==========================================================================

    | phase      | writes            | arr after phase       |
    |------------|-------------------|-----------------------|
    | count      | none              | {2, 0, 2, 1, 1, 0}    |
    | fill 0s    | arr[0], arr[1]    | {0, 0, 2, 1, 1, 0}    |
    | fill 1s    | arr[2], arr[3]    | {0, 0, 1, 1, 1, 0}    |
    | fill 2s    | arr[4], arr[5]    | {0, 0, 1, 1, 2, 2}    |

    Step count backing O(n):
      pass 1 reads each of the 6 elements once = 6 steps. Pass 2 writes each
      slot exactly once, z + o + t = 2 + 2 + 2 = 6 writes. 12 steps total,
      i.e. 2n - the "two-pass" cost. The DNF variant does it in one pass of
      at most n swaps; both are O(n), the difference is only the constant.
*/
