#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array left rotate it by k places

/*
    Approach: Reversal Algorithm
    - Left rotating by k is equivalent to:
      reverse the first k elements, reverse the rest, then reverse the whole array
    - This achieves the rotation in-place without extra space

    Algorithm Steps
    ----------------
    1. Reduce k modulo n to handle k > n
    2. Reverse the first k elements
    3. Reverse the remaining n-k elements
    4. Reverse the entire array

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void rotateLeft(vector<int>& nums, int k) {
  int n = nums.size();
  k %= n;  // Handle cases where k > n

  // Step 1: Reverse the first k elements
  reverse(nums.begin(), nums.begin() + k);

  // Step 2: Reverse the remaining n-k elements
  reverse(nums.begin() + k, nums.end());

  // Step 3: Reverse the entire array
  reverse(nums.begin(), nums.end());
}

int main() {
  vector<int> nums = {1, 2, 3, 4, 5, 6, 7};
  int k = 2;

  cout << "Original array: ";
  for (int x : nums) cout << x << " ";
  cout << endl;

  rotateLeft(nums, k);

  cout << "Array after left rotating by " << k << " places: ";
  for (int x : nums) cout << x << " ";
  cout << endl;

  return 0;
}

/*
    ======================================================================
    DRY RUN: nums = {1,2,3,4,5,6,7}, k = 2   (n = 7, answer = 3 4 5 6 7 1 2)
    ======================================================================

      index:   0   1   2   3   4   5   6
      value:   1   2   3   4   5   6   7

    Tracked state:
      n     - nums.size() = 7
      k     - rotation amount after k %= n; here 2 % 7 = 2
      nums  - the array itself, mutated in place three times

    Initial state: n = 7, k = 2 % 7 = 2, nums = 1 2 3 4 5 6 7

    Why three reversals work: a left rotation by k splits the array into
    a head A = nums[0..k-1] and a tail B = nums[k..n-1], and we want B
    then A. Reversing each part on its own gives revA revB, and reversing
    the whole thing turns revA revB into B A - because reversing a
    concatenation reverses the order of the parts AND un-reverses each.

              A = 1 2        B = 3 4 5 6 7
              want:  B A  =  3 4 5 6 7 | 1 2

    ----------------------------------------------------------------------
    STEP 1 - reverse(begin, begin + k) : reverse nums[0..1] (the head A)

      slice        nums[0..1] = 1 2        (k = 2 elements)
      swap         step 1: swap nums[0] and nums[1]  ->  2 1
                   (a 2-element reverse is exactly one swap; the two
                    cursors meet immediately and the loop stops)

      nums after:
          index:   0   1   2   3   4   5   6
          value:   2   1   3   4   5   6   7
                   ^---^
                   revA

    ----------------------------------------------------------------------
    STEP 2 - reverse(begin + k, end) : reverse nums[2..6] (the tail B)

      slice        nums[2..6] = 3 4 5 6 7        (n - k = 5 elements)
      An odd-length reverse leaves the middle element untouched. Broken
      out swap by swap, with cursors lo and hi inside the slice:

                   step 1: lo=2, hi=6  swap 3 <-> 7  -> 7 4 5 6 3
                   step 2: lo=3, hi=5  swap 4 <-> 6  -> 7 6 5 4 3
                   step 3: lo=4, hi=4  cursors met   -> stop
                           nums[4] = 5 is the middle, it stays put

      nums after:
          index:   0   1   2   3   4   5   6
          value:   2   1   7   6   5   4   3
                           ^---------------^
                                revB

    ----------------------------------------------------------------------
    STEP 3 - reverse(begin, end) : reverse the whole array

      slice        nums[0..6] = 2 1 7 6 5 4 3        (7 elements, odd)

                   step 1: lo=0, hi=6  swap 2 <-> 3  -> 3 1 7 6 5 4 2
                   step 2: lo=1, hi=5  swap 1 <-> 4  -> 3 4 7 6 5 1 2
                   step 3: lo=2, hi=4  swap 7 <-> 5  -> 3 4 5 6 7 1 2
                   step 4: lo=3, hi=3  cursors met   -> stop
                           nums[3] = 6 is the middle, it stays put

      nums after:
          index:   0   1   2   3   4   5   6
          value:   3   4   5   6   7   1   2
                   ^-----------^   ^---^
                        B            A       exactly the target B A

    ----------------------------------------------------------------------
    FINAL ARRAY: 3 4 5 6 7 1 2        (rotateLeft returns void; nums is
                                       mutated in place)

    ======================================================================
    Summary table
    ======================================================================

    | step | range reversed | swaps | array after           |
    |------|----------------|-------|-----------------------|
    | init | -              |   0   | 1 2 3 4 5 6 7         |
    |  1   | [0..1]  head   |   1   | 2 1 3 4 5 6 7         |
    |  2   | [2..6]  tail   |   2   | 2 1 7 6 5 4 3         |
    |  3   | [0..6]  whole  |   3   | 3 4 5 6 7 1 2         |

    Step count backing O(n): 1 + 2 + 3 = 6 swaps, i.e. 12 element writes
    for n = 7. Each reversal touches each of its elements once, and the
    three ranges cover the array at most twice in total (head+tail = n,
    then the whole = n), so the work is ~n swaps, never n*k as the naive
    "rotate by one, k times" approach would be. Space is O(1): every
    swap happens inside the caller's own vector.

    The one subtlety: k %= n must come FIRST. Without it, k = 9 on a
    7-element array would make begin() + k point 2 past end(), and
    reverse would run off the array. After the modulo, k is in [0, n-1],
    so both split points are valid - and k = 0 degenerates gracefully:
    an empty head reverse, then two full-array reverses that cancel out.
*/
