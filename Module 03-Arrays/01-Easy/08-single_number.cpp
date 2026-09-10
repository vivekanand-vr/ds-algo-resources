#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the element that appears only once in an array where all other elements appear twice

/*
    Approach: XOR
    - XOR-ing a number with itself gives 0, and XOR-ing with 0 gives the number back
    - XOR is commutative and associative, so order doesn't matter
    - XOR-ing every element together cancels out all pairs, leaving only the
      element that appears once

    Algorithm Steps
    ----------------
    1. Initialize answer with the first element
    2. XOR answer with every remaining element
    3. Return answer (the unique element)

    Time Complexity: O(n) - single pass through nums
    Space Complexity: O(1) - constant extra space
*/
int singleNumber(vector<int>& nums) {
    int n = nums.size();
    int answer = nums[0];

    // XOR properties:
    // x ^ x = 0, x ^ 0 = x, and XOR is commutative & associative
    // Pairs cancel out to 0, leaving only the unique number
    for (int i = 1; i < n; i++) {
        answer ^= nums[i];
    }

    return answer;
}

int main() {
    vector<int> nums = {4, 1, 2, 1, 2};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Single number: " << singleNumber(nums) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = {4, 1, 2, 1, 2}   (n = 5, answer = 4)
    ==========================================================================

      index:   0   1   2   3   4
      value:   4   1   2   1   2

    In 4-bit binary (only the low 4 bits ever matter here):
      4 = 0100      1 = 0001      2 = 0010

    Tracked state:
      answer - the XOR of every element consumed so far, seeded with nums[0]
               so the loop starts at i = 1

    Initial state: answer = nums[0] = 4 = 0100

    --------------------------------------------------------------------------
    i = 1, nums[i] = 1 = 0001
      xor              0100
                     ^ 0001
                     --------
                       0101
      answer       0101 = 5      (nothing cancelled - bits just merged)

    --------------------------------------------------------------------------
    i = 2, nums[i] = 2 = 0010
      xor              0101
                     ^ 0010
                     --------
                       0111
      answer       0111 = 7      (all three distinct bits now set)

    --------------------------------------------------------------------------
    i = 3, nums[i] = 1 = 0001      <-- the SECOND 1: cancellation happens
      xor              0111
                     ^ 0001
                     --------
                       0110
      answer       0110 = 6
      note         bit 0 was set by nums[1] = 1 and is now cleared again by
                   nums[3] = 1. The pair 1 ^ 1 = 0 has vanished from the
                   accumulator even though the two 1s were not adjacent.

    --------------------------------------------------------------------------
    i = 4, nums[i] = 2 = 0010      <-- the SECOND 2: cancellation happens
      xor              0110
                     ^ 0010
                     --------
                       0100
      answer       0100 = 4       -> loop ends (i reaches n = 5)

                     bit:    3 2 1 0
                     answer: 0 1 0 0
                               ^
                               only bit 2 survives -> that is 4, the lone
                               element; every other bit was set an even
                               number of times and flipped back to 0

    --------------------------------------------------------------------------
    RETURN answer = 4

    ==========================================================================
    Summary table
    ==========================================================================

    | i | nums[i] | binary | answer (bin) | answer (dec) | effect        |
    |---|---------|--------|--------------|--------------|---------------|
    | - |    -    |  0100  |    0100      |      4       | seed nums[0]  |
    | 1 |    1    |  0001  |    0101      |      5       | bit 0 set     |
    | 2 |    2    |  0010  |    0111      |      7       | bit 1 set     |
    | 3 |    1    |  0001  |    0110      |      6       | bit 0 cleared |
    | 4 |    2    |  0010  |    0100      |      4       | bit 1 cleared |

    The invariant that makes this correct:
      bit b of answer = parity (odd/even count) of the elements having bit b
      set. XOR is commutative and associative, so the array order is
      irrelevant - the accumulator wanders (4 -> 5 -> 7 -> 6 -> 4) but each
      duplicate's bits are guaranteed to be toggled exactly twice, so only
      the odd-count element's bits can remain at the end.

    One subtlety: answer is seeded with nums[0], so an EMPTY vector would
    read nums[0] out of bounds. The problem guarantees at least one element.
*/
