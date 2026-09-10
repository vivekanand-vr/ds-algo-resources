#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the maximum sum of two minimum elements taken from any adjacent pair in the array

/*
    Approach: Adjacent Pair Scan
    - For every adjacent pair (arr[i], arr[i+1]), their sum represents the sum of
      the smallest two elements in some subarray that contains them
    - The maximum over all such adjacent sums is the answer

    Algorithm Steps
    ----------------
    1. Traverse the array and consider each adjacent pair (arr[i], arr[i+1])
    2. Compute the sum of the pair
    3. Track the maximum sum seen across all pairs
    4. Return the maximum sum

    Time Complexity: O(n) - single pass through array
    Space Complexity: O(1) - only extra variables
*/
int maxSum(vector<int> &arr) {
    int n = arr.size();
    int ans = INT_MIN;

    // Traverse the array and consider adjacent pairs
    for (int i = 0; i < n - 1; i++) {
        // In a subarray that includes arr[i] and arr[i+1],
        // the two smallest elements will be these two.
        // So, take their sum.
        int curr = arr[i] + arr[i + 1];

        // Update the maximum sum
        ans = max(ans, curr);
    }

    return ans;
}

int main() {
    vector<int> arr = {3, 7, 4, 2, 8};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Maximum sum of two min adjacent elements: " << maxSum(arr) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {3, 7, 4, 2, 8}   (n = 5, answer = 11)
    ==========================================================================

      index:   0   1   2   3   4
      value:   3   7   4   2   8

      pairs:  (0,1) (1,2) (2,3) (3,4)      -> exactly n - 1 = 4 pairs

    Tracked state:
      i     - left index of the adjacent pair currently under test
      curr  - arr[i] + arr[i+1], the sum of that single pair
      ans   - the largest curr seen so far (starts at INT_MIN)

    Initial state: i = 0, ans = INT_MIN

    --------------------------------------------------------------------------
    i = 0   pair (arr[0], arr[1]) = (3, 7)
      sum          curr = 3 + 7 = 10
      compare      max(INT_MIN, 10) = 10
      update       ans = 10                          <-- first real value

          3   7   4   2   8
          ^---^
          curr = 10

    --------------------------------------------------------------------------
    i = 1   pair (arr[1], arr[2]) = (7, 4)
      sum          curr = 7 + 4 = 11
      compare      max(10, 11) = 11
      update       ans = 11                          <-- new best

              7   4
              ^---^
              curr = 11

    --------------------------------------------------------------------------
    i = 2   pair (arr[2], arr[3]) = (4, 2)
      sum          curr = 4 + 2 = 6
      compare      max(11, 6) = 11
      update       ans = 11   (unchanged - 6 is worse)

    --------------------------------------------------------------------------
    i = 3   pair (arr[3], arr[4]) = (2, 8)
      sum          curr = 2 + 8 = 10
      compare      max(11, 10) = 11
      update       ans = 11   (unchanged)
      advance      i -> 4, and 4 < n - 1 = 4 is false -> loop ends

    --------------------------------------------------------------------------
    RETURN ans = 11        (from the pair (7, 4) at indices 1 and 2)

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i] | arr[i+1] | curr | ans before | ans after | new best? |
    |---|--------|----------|------|------------|-----------|-----------|
    | 0 |   3    |    7     |  10  |  INT_MIN   |    10     |    yes    |
    | 1 |   7    |    4     |  11  |     10     |    11     |    yes    |
    | 2 |   4    |    2     |   6  |     11     |    11     |    no     |
    | 3 |   2    |    8     |  10  |     11     |    11     |    no     |

    Step count behind the O(n) claim:
      4 additions and 4 comparisons for n = 5, i.e. exactly n - 1 of each.
      No inner loop, no sorting, no extra array.

    Two details worth not tripping on:
      1. The loop bound is i < n - 1, not i < n. At i = n - 1 the expression
         arr[i + 1] would read past the end of the vector.
      2. ans starts at INT_MIN rather than 0 so that an array of all negative
         numbers (say {-4, -9, -2}) still returns its true best (-6) instead
         of a bogus 0.

    Why scanning only ADJACENT pairs is enough:
      the question asks for the best "sum of the two smallest elements" over
      all subarrays of length >= 2. Take any such subarray and let a and b be
      its two smallest elements. Shrinking that subarray down to just some
      adjacent pair inside it can never lower the sum of its two smallest
      elements, and a subarray of length exactly 2 has its two smallest
      elements equal to its two members. So some adjacent pair always ties or
      beats every longer subarray - which is why one linear sweep suffices.
*/
