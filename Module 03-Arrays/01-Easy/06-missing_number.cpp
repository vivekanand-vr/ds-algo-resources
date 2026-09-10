#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array nums containing n distinct numbers in the range [0, n],
// return the only number in the range that is missing from the array.

/*
    Approach: Sum Formula
    - The numbers 0 to n should sum to n*(n+1)/2 if none were missing
    - Subtract every present number from this expected sum
    - What remains is exactly the missing number

    Algorithm Steps
    ----------------
    1. Compute expected sum = n * (n + 1) / 2
    2. Subtract each element of nums from expected sum
    3. Return the remaining value (the missing number)

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
int missingNumber(vector<int>& nums) {
    int n = nums.size();

    // Step 1: Calculate expected sum of first n natural numbers (0 to n)
    int sum = n * (n + 1) / 2;

    // Step 2: Subtract each number in the array from the expected sum
    for (int i : nums) {
        sum -= i;
    }

    // Step 3: The result is the missing number
    return sum;
}

int main() {
    vector<int> nums = {3, 0, 1};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Missing number: " << missingNumber(nums) << endl;

    return 0;
}

/*
    ======================================================================
    DRY RUN: nums = {3, 0, 1}      (n = 3, answer = 2)
    ======================================================================

      index:   0   1   2
      value:   3   0   1

    The range is [0, n] = [0, 3], which is 4 candidate values for only
    3 array slots - so exactly one of 0,1,2,3 is absent. Here it is 2.

    Tracked state:
      sum - starts as the total of 0..n, then has each present element
            subtracted out. Whatever survives is the missing value.

    Initial state: sum = n*(n+1)/2 = 3*4/2 = 6
                   (that is 0+1+2+3 = 6, the total if nothing were missing)

    ----------------------------------------------------------------------
    i = 3    subtract   sum = 6 - 3 = 3
    i = 0    subtract   sum = 3 - 0 = 3
    i = 1    subtract   sum = 3 - 1 = 2
    ----------------------------------------------------------------------
    RETURN 2

    ======================================================================
    Summary table
    ======================================================================

    | element | sum before | sum after |
    |---------|------------|-----------|
    |    -    |     -      |     6     |
    |    3    |     6      |     3     |
    |    0    |     3      |     3     |
    |    1    |     3      |     2     |

    Why it works: sum(0..n) - sum(present) = sum(absent), and since the
    problem guarantees the values are distinct and exactly one is absent,
    that difference IS the missing number. Order does not matter - the
    loop is a plain accumulation, so any permutation of {3,0,1} gives 2.

    One pass, 3 subtractions plus one multiply - O(n) time, one int of
    state, O(1) space. (The XOR variant avoids the n*(n+1)/2 product,
    which can overflow int for very large n.)
*/
