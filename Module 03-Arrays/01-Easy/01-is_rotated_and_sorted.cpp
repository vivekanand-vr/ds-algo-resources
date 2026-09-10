#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Check if the given array is sorted and rotated

/*
    Approach: Sorted and Rotated Array Check
    - A sorted-and-rotated array has at most one point where arr[i] > arr[i+1]
    - Find that violation point (if any)
    - The part before and after the point must individually be sorted
    - The last element must be <= the elements before the violation point
      (this is the wrap-around condition of a rotated sorted array)

    Algorithm Steps
    ----------------
    1. Traverse the array to find the first index where arr[i] > arr[i+1]
    2. If no such index exists, the array is already sorted -> true
    3. Check that the remaining part after that point is sorted
    4. Check that the last element is <= every element before the violation point

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
bool check(vector<int>& arr) {
    int point = -1;
    
    // Step 1: Find the first index where the order is violated (i.e., arr[i] > arr[i+1])
    // This marks the "rotation point"
    for (int i = 0; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            point = i;
            break;
        }
    }

    // Step 2: If no such point is found, array is already sorted (not rotated or rotated 0 times)
    if (point == -1) return true;

    // Step 3: Check if the remaining part after the point is sorted
    for (int i = point + 1; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return false; // not sorted
        }
    }

    // Step 4: Check if the last element is not greater than the first part before the point
    // i.e., max in second part ≤ min in first part for rotated sorted array
    for (int i = 0; i <= point; ++i) {
        if (arr[arr.size() - 1] > arr[i]) {
            return false;
        }
    }

    return true;
}

int main() {
    vector<int> arr1 = {3, 4, 5, 1, 2};
    cout << "Array: 3 4 5 1 2 -> " << (check(arr1) ? "Sorted and Rotated" : "Not Sorted and Rotated") << endl;

    vector<int> arr2 = {2, 1, 3, 4};
    cout << "Array: 2 1 3 4 -> " << (check(arr2) ? "Sorted and Rotated" : "Not Sorted and Rotated") << endl;

    vector<int> arr3 = {1, 2, 3, 4, 5};
    cout << "Array: 1 2 3 4 5 -> " << (check(arr3) ? "Sorted and Rotated" : "Not Sorted and Rotated") << endl;

    return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {3, 4, 5, 1, 2}      (n = 5, answer = true)
    ======================================================================

      index:   0   1   2   3   4
      value:   3   4   5   1   2

    Tracked state:
      point - index of the first place where arr[i] > arr[i+1], i.e. the
              rotation point. Stays -1 if the array never dips.
      i     - scan cursor, reused independently by all three loops

    Initial state: point = -1

    ----------------------------------------------------------------------
    STEP 1 - find the rotation point (loop i = 0 .. n-2 = 0 .. 3)

      i = 0   compare  arr[0]=3 > arr[1]=4 ?  no  -> keep scanning
      i = 1   compare  arr[1]=4 > arr[2]=5 ?  no  -> keep scanning
      i = 2   compare  arr[2]=5 > arr[3]=1 ?  YES -> point = 2, break

                   3   4   5 | 1   2
                           ^
                         point = 2      the array dips between 5 and 1

    ----------------------------------------------------------------------
    STEP 2 - point != -1, so this is not an already-sorted array.
             Do NOT return true here; the two halves still have to check
             out. Fall through to steps 3 and 4.

    ----------------------------------------------------------------------
    STEP 3 - is the tail after the dip sorted?  (loop i = 3 .. 3)

      i = 3   compare  arr[3]=1 > arr[4]=2 ?  no  -> tail still sorted
              loop ends (i = 4 is not < 4), no early false

    ----------------------------------------------------------------------
    STEP 4 - wrap-around check: the LAST element must be <= every element
             of the head arr[0..point].  (loop i = 0 .. 2, last = arr[4] = 2)

      i = 0   compare  2 > arr[0]=3 ?  no  -> ok
      i = 1   compare  2 > arr[1]=4 ?  no  -> ok
      i = 2   compare  2 > arr[2]=5 ?  no  -> ok

    ----------------------------------------------------------------------
    RETURN true     ("Sorted and Rotated")

    ======================================================================
    Summary table
    ======================================================================

    | step | i | comparison        | result | state after      |
    |------|---|-------------------|--------|------------------|
    |  1   | 0 | 3 > 4 ?           | no     | point = -1       |
    |  1   | 1 | 4 > 5 ?           | no     | point = -1       |
    |  1   | 2 | 5 > 1 ?           | yes    | point = 2, break |
    |  3   | 3 | 1 > 2 ?           | no     | tail sorted      |
    |  4   | 0 | last 2 > 3 ?      | no     | ok               |
    |  4   | 1 | last 2 > 4 ?      | no     | ok               |
    |  4   | 2 | last 2 > 5 ?      | no     | ok               |

    ======================================================================
    Contrast: why arr = {2, 1, 3, 4} is rejected
    ======================================================================

      index:   0   1   2   3
      value:   2   1   3   4

      step 1: i = 0  ->  arr[0]=2 > arr[1]=1  -> point = 0, break
      step 3: i = 1  ->  1 > 3 ? no
              i = 2  ->  3 > 4 ? no      tail 1,3,4 IS sorted
      step 4: i = 0  ->  last arr[3]=4 > arr[0]=2 ?  YES -> return false

    So step 3 alone is not enough. Both halves were internally sorted, and
    it is only the wrap-around test in step 4 that catches it: rotating a
    sorted array can never leave a tail element larger than a head element.
    That single check is the whole point of the algorithm.

    Step count: 3 + 1 + 3 = 7 comparisons for n = 5. Each of the three
    loops walks a disjoint-or-shorter slice of the array once, so the
    total stays O(n) despite there being three separate loops.
*/
