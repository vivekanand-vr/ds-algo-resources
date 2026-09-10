#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array of size n, find the majority element (appears more than n/2 times)

/*
    Approach: Boyer-Moore Voting Algorithm
    - Keeps track of a candidate and a counter representing its "balance"
    - Majority element (appears > n/2 times) is guaranteed to survive the cancellation

    Algorithm Steps
    ----------------
    1. Initialize counter c = 0 and candidate curr = nums[0]
    2. Traverse the array; if c == 0, set curr to the current element
    3. Increment c if current element matches curr, else decrement c
    4. Return curr as the majority element

    Time Complexity: O(n) - single pass over the array
    Space Complexity: O(1) - uses only a couple of variables
*/
int majorityElement(vector<int>& nums) {
    int n = (int) nums.size();
    int c = 0;             // counter for tracking "balance" of candidate
    int curr = nums[0];    // candidate element

    // Traverse array to find potential majority element
    for (int i = 0; i < n; ++i) {
        if (c == 0) 
            curr = nums[i];    // set new candidate when counter is zero

        // If current element matches candidate, increment counter
        // Else, decrement (cancel out one occurrence)
        c += (curr == nums[i]) ? 1 : -1;
    }

    // At the end, 'curr' will be the majority element
    return curr;
}

int main() {
    vector<int> nums = {2, 2, 1, 1, 1, 2, 2};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Majority element: " << majorityElement(nums) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = {2, 2, 1, 1, 1, 2, 2}   (n = 7, answer = 2)
    ==========================================================================

      index:   0   1   2   3   4   5   6
      value:   2   2   1   1   1   2   2

      counts:  2 appears 4 times, 1 appears 3 times. 4 > 7/2 = 3, so 2 is
      the majority element (only just - this is the tightest possible case).

    Tracked state:
      curr - the current candidate for majority element
      c    - the candidate's "balance": how many unmatched copies of curr
             are left after cancelling one non-curr element against each
      the rule: c == 0 means the prefix seen so far cancels out perfectly,
             so nothing is known and the next element becomes the candidate

    Initial state: curr = nums[0] = 2, c = 0
      (curr is pre-seeded, but note c = 0 means iteration i = 0 will set it
       again anyway - the pre-seed only matters if the loop never runs)

    --------------------------------------------------------------------------
    i = 0, nums[i] = 2
      c == 0?      yes -> adopt   curr = nums[0] = 2
      vote         nums[i] == curr -> c += 1
      state        curr = 2, c = 1

    --------------------------------------------------------------------------
    i = 1, nums[i] = 2
      c == 0?      no (c = 1), keep curr = 2
      vote         2 == 2 -> c += 1
      state        curr = 2, c = 2

    --------------------------------------------------------------------------
    i = 2, nums[i] = 1
      c == 0?      no (c = 2), keep curr = 2
      vote         1 != 2 -> c -= 1     (one 1 cancels one 2)
      state        curr = 2, c = 1

    --------------------------------------------------------------------------
    i = 3, nums[i] = 1
      c == 0?      no (c = 1), keep curr = 2
      vote         1 != 2 -> c -= 1
      state        curr = 2, c = 0      <-- BALANCE HIT ZERO

                   The prefix nums[0..3] = {2,2,1,1} has two 2s and two 1s.
                   They cancel exactly, so no element can be the majority of
                   this prefix, and everything learned so far is discarded.
                   curr is still 2 in memory, but it is now meaningless -
                   the next iteration will overwrite it.

                       2   2   1   1 | 1   2   2
                       \-----------/   \-------/
                        cancels out    the rest still decides the answer

    --------------------------------------------------------------------------
    i = 4, nums[i] = 1
      c == 0?      YES -> adopt   curr = nums[4] = 1    <-- CANDIDATE REPLACED
      vote         1 == 1 -> c += 1
      state        curr = 1, c = 1

    --------------------------------------------------------------------------
    i = 5, nums[i] = 2
      c == 0?      no (c = 1), keep curr = 1
      vote         2 != 1 -> c -= 1
      state        curr = 1, c = 0      <-- BALANCE HIT ZERO AGAIN

                   nums[4..5] = {1,2} cancels too. Candidate 1 is discarded
                   with exactly one element left to scan.

    --------------------------------------------------------------------------
    i = 6, nums[i] = 2
      c == 0?      YES -> adopt   curr = nums[6] = 2    <-- CANDIDATE REPLACED
      vote         2 == 2 -> c += 1
      state        curr = 2, c = 1
      advance      i -> 7   ->   i == n, loop ends

    --------------------------------------------------------------------------
    RETURN curr = 2

    ==========================================================================
    Summary table
    ==========================================================================

    | i | nums[i] | c was 0? | curr adopted | vote | c after |
    |---|---------|----------|--------------|------|---------|
    | 0 |    2    | yes      | 2            |  +1  |    1    |
    | 1 |    2    | no       | 2 (kept)     |  +1  |    2    |
    | 2 |    1    | no       | 2 (kept)     |  -1  |    1    |
    | 3 |    1    | no       | 2 (kept)     |  -1  |    0    |
    | 4 |    1    | yes      | 1 (new)      |  +1  |    1    |
    | 5 |    2    | no       | 1 (kept)     |  -1  |    0    |
    | 6 |    2    | yes      | 2 (new)      |  +1  |    1    |

    The invariant that makes this correct:
      whenever c drops to 0 after processing nums[0..i], that prefix contains
      equal numbers of "curr" and "not curr", so a true majority element of
      the WHOLE array must still be a majority element of the remaining
      suffix nums[i+1..n-1]. Discarding a balanced prefix therefore cannot
      throw the answer away, and the same argument applies recursively to
      whatever prefix is discarded next. It happened twice here: {2,2,1,1}
      at i = 3 and {1,2} at i = 5, leaving just nums[6] = 2 to decide.

    The subtlety to watch for:
      the algorithm returns a CANDIDATE, not a proof. On nums = {1, 2, 3} it
      would return 3 even though no majority element exists. The problem
      statement here promises one exists, which is the only reason a second
      verification pass (count occurrences of curr, check > n/2) is omitted.

    Step count backing O(n):
      exactly 7 iterations, each doing one comparison against curr and one
      +/-1 on c. No nested loop, no extra memory - 2 variables total.
*/
