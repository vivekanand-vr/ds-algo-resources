#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Rearrange arr into the next lexicographically greater permutation of numbers, in-place

/*
    Approach: Find Dip, Swap, Reverse Suffix
    - Find the first "dip" from the right where arr[i] < arr[i+1]
    - Swap that element with the smallest element to its right that is greater than it
    - Reverse the suffix after the dip to make it the smallest possible arrangement

    Algorithm Steps
    ----------------
    1. Find the first index 'ind' from the right such that arr[ind] < arr[ind+1]
    2. If no such index exists, the array is the last permutation, so reverse it entirely
    3. Otherwise, from the right, find the first element greater than arr[ind] and swap
    4. Reverse the suffix after 'ind' to get the next smallest arrangement

    Time Complexity: O(n) - a constant number of linear passes over the array
    Space Complexity: O(1) - in-place
*/
void nextPermutation(vector<int>& arr) {
    int n = arr.size(), ind = -1;

    // Step 1: Find the first index 'ind' from the right 
    // such that arr[ind] < arr[ind+1] (the "dip")
    for (int i = n - 2; i >= 0; --i) {
        if (arr[i] < arr[i + 1]) {
            ind = i;
            break;
        }
    }

    // If no such index exists → array is in descending order,
    // so just reverse to get the smallest permutation
    if (ind == -1) {
        reverse(begin(arr), end(arr));
        return;
    }

    // Step 2: From the right, find the first element greater than arr[ind]
    for (int i = n - 1; i > ind; --i) {
        if (arr[i] > arr[ind]) {
            swap(arr[i], arr[ind]);   // swap with next bigger element
            break;                    // important: break after swap
        }
    }

    // Step 3: Reverse the suffix after 'ind' 
    // to get the next lexicographically smallest arrangement
    reverse(arr.begin() + ind + 1, arr.end());
}

int main() {
    vector<int> arr = {1, 2, 3};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    nextPermutation(arr);

    cout << "Next permutation: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {1, 3, 5, 4, 2}   (n = 5, answer = {1, 4, 2, 3, 5})
    ==========================================================================

      A richer input than main()'s {1,2,3} is used here so that all four
      phases actually do visible work; main()'s own tiny case is traced at
      the bottom of this block.

      index:   0   1   2   3   4
      value:   1   3   5   4   2

    Tracked state:
      ind - index of the "dip": the rightmost i with arr[i] < arr[i+1].
            Everything strictly right of ind is non-increasing, i.e. it is
            already the LARGEST arrangement of those values, so no next
            permutation can be made by touching the suffix alone.
      i   - scan index, always moving right-to-left in both search phases

    Initial state: ind = -1, arr = {1, 3, 5, 4, 2}

    ==========================================================================
    PHASE 1 - find the dip, scanning from the right
    ==========================================================================

      loop i = n-2 = 3 down to 0, testing arr[i] < arr[i+1]

    --------------------------------------------------------------------------
    i = 3
      compare      arr[3] = 4  <  arr[4] = 2 ?   ->  NO (4 > 2)
      continue     suffix {4, 2} is descending, still no dip

    --------------------------------------------------------------------------
    i = 2
      compare      arr[2] = 5  <  arr[3] = 4 ?   ->  NO (5 > 4)
      continue     suffix {5, 4, 2} is descending, still no dip

    --------------------------------------------------------------------------
    i = 1
      compare      arr[1] = 3  <  arr[2] = 5 ?   ->  YES
      set          ind = 1  and BREAK

                       1   3 | 5   4   2
                           ^   \-------/
                          ind   descending suffix (already maximal)

      Reading of this: the suffix {5,4,2} cannot be advanced, so the value
      at ind = 1 must increase. It has to increase by the SMALLEST possible
      amount to stay "next", which is what phase 2 finds.

      arr after phase 1: {1, 3, 5, 4, 2}   (unchanged - phase 1 only reads)

      (if the loop had finished with ind still -1, the whole array would be
       descending, i.e. the last permutation; the code would then reverse
       everything and return early. Not taken here.)

    ==========================================================================
    PHASE 2 - find the successor to swap with, again from the right
    ==========================================================================

      loop i = n-1 = 4 down to ind+1 = 2, testing arr[i] > arr[ind] = 3

    --------------------------------------------------------------------------
    i = 4
      compare      arr[4] = 2  >  arr[ind] = 3 ?   ->  NO
      continue

    --------------------------------------------------------------------------
    i = 3
      compare      arr[3] = 4  >  arr[ind] = 3 ?   ->  YES  -> swap, break

      Why the RIGHTMOST such element is exactly the right one: the suffix is
      non-increasing, so scanning from the right the first value above 3 is
      also the SMALLEST value above 3 in that suffix. 5 is greater than 3
      too, but swapping in 5 would overshoot to 15432 > 14235.

    ==========================================================================
    PHASE 3 - swap
    ==========================================================================

      swap         arr[3] <-> arr[1]   (4 <-> 3)
      arr          {1, 3, 5, 4, 2}  ->  {1, 4, 5, 3, 2}

                       1   4 | 5   3   2
                           ^   \-------/
                          ind   suffix, still non-increasing after the swap

      The prefix {1, 4} is now correct and final. But the suffix {5, 3, 2}
      is still the LARGEST arrangement of those three values, and we want
      the smallest tail behind the freshly raised prefix.

    ==========================================================================
    PHASE 4 - reverse the suffix after ind
    ==========================================================================

      reverse      reverse(arr.begin() + ind + 1, arr.end())
                   = reverse over indices 2 .. 4 -> the block {5, 3, 2}

      step 1:      swap arr[2] <-> arr[4]   (5 <-> 2)
                   arr = {1, 4, 2, 3, 5}
      step 2:      pointers meet at arr[3] = 3, nothing to do -> done

      A reverse (not a sort) suffices precisely because the suffix was
      non-increasing: reversing a non-increasing block yields a
      non-decreasing block, which is its smallest arrangement, in O(n)
      instead of O(n log n).

      arr after phase 4: {1, 4, 2, 3, 5}

    --------------------------------------------------------------------------
    FINAL ARRAY  arr = {1, 4, 2, 3, 5}      (in-place, void return)

      Sanity check: 13542 -> 14235. The permutations of {1,2,3,4,5} between
      them, 13542 being the largest starting "135", and 14235 the smallest
      starting "14". Nothing sits in between.

    ==========================================================================
    Summary table
    ==========================================================================

    | phase | what it does           | key value  | arr after phase   |
    |-------|------------------------|------------|-------------------|
    |   1   | find dip from right    | ind = 1    | {1, 3, 5, 4, 2}   |
    |   2   | find successor > arr[1]| i = 3 (4)  | {1, 3, 5, 4, 2}   |
    |   3   | swap arr[1], arr[3]    | 3 <-> 4    | {1, 4, 5, 3, 2}   |
    |   4   | reverse arr[2 .. 4]    | 5,3,2->2,3,5| {1, 4, 2, 3, 5}  |

    --------------------------------------------------------------------------
    The case main() actually runs: arr = {1, 2, 3}
    --------------------------------------------------------------------------

      phase 1   i = 1: arr[1] = 2 < arr[2] = 3 -> ind = 1, break immediately
                (the dip is the very first index tested - nothing descends)
      phase 2   i = 2: arr[2] = 3 > arr[ind] = 2 -> swap, break
      phase 3   swap arr[1] <-> arr[2]  ->  {1, 3, 2}
      phase 4   reverse over indices 2 .. 2 - a single element, a NO-OP
      printed   "Next permutation: 1 3 2"

      This is why the bigger input above is worth tracing: with {1,2,3} two
      of the four phases do nothing observable.

    Step count backing O(n):
      phase 1 read 3 pairs, phase 2 read 2 elements, phase 3 was 1 swap,
      phase 4 was 1 swap. 7 element touches for n = 5 - at most about 2n
      overall, and no allocation, so O(1) space.

    The subtlety to trip on:
      the `break` after the swap in phase 2 is load-bearing. Without it the
      loop keeps scanning with a MUTATED arr[ind] (now 4) and would swap
      again at i = 2, since arr[2] = 5 > 4, giving {1,5,4,3,2} before the
      reverse and {1,5,2,3,4} after it - a valid permutation, but far past
      the next one.
*/
