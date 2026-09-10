#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the contiguous subarray with the largest sum and return that sum

/*
    Approach: Kadane's Algorithm
    - Iteratively decide whether to extend the previous subarray
      or start a new one from the current element
    - Tracks the maximum subarray sum in a single pass

    Algorithm Steps
    ----------------
    1. Initialize sum and res with nums[0]
    2. For each subsequent element, set sum = max(nums[i], sum + nums[i])
    3. Update res with the maximum of res and sum
    4. Return res

    Time Complexity: O(n) - single pass through the array
    Space Complexity: O(1) - only uses a few integer variables
*/
int maxSubArray(vector<int>& nums) {
    int sum = nums[0];  // running sum (max subarray ending at current index)
    int res = nums[0];  // global maximum subarray sum found so far

    // Start from second element since we initialized with nums[0]
    for (int i = 1; i < nums.size(); ++i) {
        // Either extend the previous subarray or start fresh from current element
        sum = max(nums[i], sum + nums[i]);

        // Update global maximum
        res = max(res, sum);
    }

    return res;
}

int main() {
    vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Maximum subarray sum: " << maxSubArray(nums) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4}   (n = 9, answer = 6)
    ==========================================================================

      index:    0   1   2   3   4   5   6   7   8
      value:   -2   1  -3   4  -1   2   1  -5   4

    Tracked state:
      sum - best sum of a subarray that ENDS exactly at index i
      res - best sum of any subarray seen anywhere so far (the answer)

    The whole algorithm is one decision per element:
        sum = max(nums[i], sum + nums[i])
      RESTART  if nums[i] alone beats the extension  -> the old subarray was
               dragging us down (sum was negative), so drop it entirely
      EXTEND   otherwise -> keep the previous subarray and append nums[i]
      Equivalently: restart exactly when sum < 0 before adding.

    Initial state: sum = nums[0] = -2, res = -2
      (index 0 is consumed by the initialization; the loop starts at i = 1)

    --------------------------------------------------------------------------
    i = 1, nums[i] = 1
      compare      nums[i] = 1   vs   sum + nums[i] = -2 + 1 = -1
      decide       1 > -1 -> RESTART (the -2 prefix only hurts)
      sum          sum = 1                          subarray = [1]
      res          res = max(-2, 1) = 1              <-- new best

    --------------------------------------------------------------------------
    i = 2, nums[i] = -3
      compare      nums[i] = -3   vs   sum + nums[i] = 1 + (-3) = -2
      decide       -2 > -3 -> EXTEND (starting fresh at -3 is even worse)
      sum          sum = -2                         subarray = [1, -3]
      res          res = max(1, -2) = 1             (unchanged)

    --------------------------------------------------------------------------
    i = 3, nums[i] = 4
      compare      nums[i] = 4   vs   sum + nums[i] = -2 + 4 = 2
      decide       4 > 2 -> RESTART (sum was negative, so it can only
                   subtract from a fresh start)
      sum          sum = 4                          subarray = [4]
      res          res = max(1, 4) = 4              <-- new best

                       -2   1  -3 |  4  -1   2   1  -5   4
                       \--------/    ^
                        discarded    new subarray starts here

    --------------------------------------------------------------------------
    i = 4, nums[i] = -1
      compare      nums[i] = -1   vs   sum + nums[i] = 4 + (-1) = 3
      decide       3 > -1 -> EXTEND
      sum          sum = 3                          subarray = [4, -1]
      res          res = max(4, 3) = 4              (unchanged - a dip does
                   not lose the best; res remembers the peak)

    --------------------------------------------------------------------------
    i = 5, nums[i] = 2
      compare      nums[i] = 2   vs   sum + nums[i] = 3 + 2 = 5
      decide       5 > 2 -> EXTEND
      sum          sum = 5                          subarray = [4, -1, 2]
      res          res = max(4, 5) = 5              <-- new best

    --------------------------------------------------------------------------
    i = 6, nums[i] = 1
      compare      nums[i] = 1   vs   sum + nums[i] = 5 + 1 = 6
      decide       6 > 1 -> EXTEND
      sum          sum = 6                          subarray = [4, -1, 2, 1]
      res          res = max(5, 6) = 6              <-- new best (final)

    --------------------------------------------------------------------------
    i = 7, nums[i] = -5
      compare      nums[i] = -5   vs   sum + nums[i] = 6 - 5 = 1
      decide       1 > -5 -> EXTEND (sum is still POSITIVE, so it is worth
                   carrying even after a big loss - no restart yet)
      sum          sum = 1                     subarray = [4,-1,2,1,-5]
      res          res = max(6, 1) = 6              (unchanged)

    --------------------------------------------------------------------------
    i = 8, nums[i] = 4
      compare      nums[i] = 4   vs   sum + nums[i] = 1 + 4 = 5
      decide       5 > 4 -> EXTEND, only just: carrying the +1 remainder
                   beats a fresh start by exactly 1
      sum          sum = 5                   subarray = [4,-1,2,1,-5,4]
      res          res = max(6, 5) = 6              (unchanged)
      advance      i -> 9   ->   i == n, loop ends

    --------------------------------------------------------------------------
    RETURN res = 6        the subarray nums[3..6] = [4, -1, 2, 1]

    ==========================================================================
    Summary table
    ==========================================================================

    | i | nums[i] | sum+nums[i] | decision | sum | res | current subarray  |
    |---|---------|-------------|----------|-----|-----|-------------------|
    | 0 |   -2    |      -      | init     | -2  | -2  | [-2]              |
    | 1 |    1    |     -1      | restart  |  1  |  1  | [1]               |
    | 2 |   -3    |     -2      | extend   | -2  |  1  | [1,-3]            |
    | 3 |    4    |      2      | restart  |  4  |  4  | [4]               |
    | 4 |   -1    |      3      | extend   |  3  |  4  | [4,-1]            |
    | 5 |    2    |      5      | extend   |  5  |  5  | [4,-1,2]          |
    | 6 |    1    |      6      | extend   |  6  |  6  | [4,-1,2,1]        |
    | 7 |   -5    |      1      | extend   |  1  |  6  | [4,-1,2,1,-5]     |
    | 8 |    4    |      5      | extend   |  5  |  6  | [4,-1,2,1,-5,4]   |

    The subtlety a reader trips on:
      sum and res drift apart and that is intentional. After i = 8 the
      running subarray is worth only 5, yet the answer is 6 - the best window
      closed back at i = 6 and res is the only thing that still remembers it.
      Never return sum.

    Why res must be seeded with nums[0] and not 0:
      seeding res = 0 would return 0 on an all-negative array like {-3, -1},
      claiming an empty subarray. Seeding from nums[0] forces at least one
      element into the answer, correctly giving -1 there.

    Step count backing O(n):
      8 iterations for 9 elements (index 0 handled by the init), each doing
      2 comparisons and 0 extra allocations - 2 int variables of space.
*/
