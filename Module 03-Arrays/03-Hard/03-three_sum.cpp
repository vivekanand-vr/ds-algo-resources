#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find all unique triplets in the array that sum up to zero

/*
    Approach: Sort + Two Pointers
    - Sort the array so duplicates sit next to each other and two-pointer
      scanning becomes possible
    - Fix the first element of the triplet, then use two pointers (left/right)
      on the remaining subarray to find pairs summing to -nums[i]
    - Skip over duplicate values for all three positions to avoid duplicate
      triplets in the result

    Algorithm Steps
    ----------------
    1. Sort nums in ascending order
    2. For each index i, skip if it duplicates the previous fixed element
    3. Set l = i+1, r = n-1 and move pointers based on the triplet sum
    4. If sum == 0, record the triplet and skip duplicates for l and r
    5. If sum < 0, move l right; if sum > 0, move r left
    6. Return all collected triplets

    Time Complexity: O(n^2) - sorting O(n log n) plus O(n) two-pointer scan for each of n fixed elements
    Space Complexity: O(1) - excluding the space used for the output
*/
vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> res;
    int n = nums.size();

    // Step 1: Sort the array to handle duplicates and use two-pointer method
    sort(nums.begin(), nums.end());

    // Step 2: Fix one number and find the other two using two pointers
    for (int i = 0; i < n; i++) {
        // Skip duplicates for the first element
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        int l = i + 1;       // left pointer (next element)
        int r = n - 1;       // right pointer (last element)

        while (l < r) {
            long sum = (long)nums[i] + nums[l] + nums[r];  // current triplet sum

            if (sum == 0) {
                // Found a valid triplet
                res.push_back({nums[i], nums[l], nums[r]});
                l++;
                r--;

                // Skip duplicates for left pointer
                while (l < r && nums[l] == nums[l - 1]) l++;
                // Skip duplicates for right pointer
                while (l < r && nums[r] == nums[r + 1]) r--;
            }
            else if (sum < 0) {
                // If sum is too small, move left pointer to increase sum
                l++;
            }
            else {
                // If sum is too large, move right pointer to decrease sum
                r--;
            }
        }
    }
    return res;
}

int main() {
    vector<int> nums = {-1, 0, 1, 2, -1, -4};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    vector<vector<int>> result = threeSum(nums);

    cout << "Triplets that sum to zero:" << endl;
    for (auto& triplet : result) {
        for (int x : triplet) cout << x << " ";
        cout << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = {-1, 0, 1, 2, -1, -4}
             (n = 6, answer = {{-1, -1, 2}, {-1, 0, 1}})
    ==========================================================================

    BEFORE the sort (the order main() passes in):

      index:   0   1   2   3   4   5
      nums:   -1   0   1   2  -1  -4

    AFTER sort(nums.begin(), nums.end())  -- every index below refers to THIS
    layout, not the one above. Sorting is what puts the two -1s side by side,
    which is the only reason the duplicate skips can be a simple neighbour
    comparison:

      index:   0   1   2   3   4   5
      nums:   -4  -1  -1   0   1   2
               ^   ^   ^
               |   |   +-- duplicate of index 1
               |   +------ first -1
               +---------- i starts here

    Tracked state:
      i    - index of the FIXED first element of the triplet
      l    - left pointer, starts at i+1, only moves right
      r    - right pointer, starts at n-1, only moves left
      sum  - nums[i] + nums[l] + nums[r]; we want sum == 0
      res  - the collected triplets

    Initial state: res = {}, n = 6

    --------------------------------------------------------------------------
    i = 0, nums[i] = -4      (i == 0, so the i-level dup check is skipped)
      l = 1, r = 5

        -4  -1  -1   0   1   2
         ^   ^               ^
         i   l               r

      step 1: sum = -4 + nums[1] + nums[5] = -4 + (-1) + 2 = -3  < 0 -> l = 2
      step 2: sum = -4 + nums[2] + nums[5] = -4 + (-1) + 2 = -3  < 0 -> l = 3
              (nums[2] duplicates nums[1], but there is NO dup skip on the
               sum<0 / sum>0 branches - only after a hit. Re-testing the same
               value is harmless here, it just fails again.)
      step 3: sum = -4 + nums[3] + nums[5] = -4 + 0 + 2 = -2     < 0 -> l = 4
      step 4: sum = -4 + nums[4] + nums[5] = -4 + 1 + 2 = -1     < 0 -> l = 5
              now l = 5, r = 5 -> l < r is false, inner loop ends
      found: nothing. -4 is too small for any pair here to complete it.

    --------------------------------------------------------------------------
    i = 1, nums[i] = -1
      dup check: i > 0 and nums[1] == nums[0]?  -1 == -4?  NO -> proceed
      l = 2, r = 5

        -4  -1  -1   0   1   2
             ^   ^           ^
             i   l           r

      step 1: sum = -1 + nums[2] + nums[5] = -1 + (-1) + 2 = 0   HIT
              record {-1, -1, 2}
              l -> 3, r -> 4
              skip dup l: l < r (3 < 4) and nums[3] == nums[2]?  0 == -1? NO
                          -> l stays 3
              skip dup r: l < r (3 < 4) and nums[4] == nums[5]?  1 == 2?  NO
                          -> r stays 4
              (Nothing to skip this time - the values just consumed, -1 and 2,
               have no further copies next to the new pointer positions.)

      step 2: sum = -1 + nums[3] + nums[4] = -1 + 0 + 1 = 0      HIT
              record {-1, 0, 1}
              l -> 4, r -> 3
              skip dup l: l < r (4 < 3)? NO -> loop body never runs
              skip dup r: l < r (4 < 3)? NO -> loop body never runs
              pointers have crossed, inner loop ends

      found: {-1, -1, 2} and {-1, 0, 1}

    --------------------------------------------------------------------------
    i = 2, nums[i] = -1      <-- SKIPPED, and this is the important one
      dup check: i > 0 and nums[2] == nums[1]?  -1 == -1?  YES -> continue

      Why this skip is required, not just an optimisation:
        Had i = 2 run, l would start at 3 and r at 5, giving
        sum = -1 + 0 + 1 = 0 -> the triplet {-1, 0, 1} AGAIN, a byte-for-byte
        duplicate of what i = 1 already recorded. Any triplet whose smallest
        element is -1 was already fully enumerated when i pointed at the FIRST
        -1, because that pass had a strictly larger window [i+1, n-1] to
        search. So the second -1 as the fixed element can only re-find old
        answers.

      Note the test is nums[i] == nums[i-1] (the PREVIOUS index), not
      nums[i] == nums[i+1]. Skipping on the next-index test would skip the
      first copy and keep the last, which searches the SMALLER window and
      would miss triplets like {-1, -1, 2} that need both copies available.

    --------------------------------------------------------------------------
    i = 3, nums[i] = 0
      dup check: nums[3] == nums[2]?  0 == -1?  NO -> proceed
      l = 4, r = 5
      step 1: sum = 0 + nums[4] + nums[5] = 0 + 1 + 2 = 3        > 0 -> r = 4
              l = 4, r = 4 -> l < r false, inner loop ends
      found: nothing

    --------------------------------------------------------------------------
    i = 4, nums[i] = 1
      dup check: 1 == 0? NO -> proceed
      l = 5, r = 5 -> l < r false immediately, inner loop never runs
      found: nothing

    --------------------------------------------------------------------------
    i = 5, nums[i] = 2
      dup check: 2 == 1? NO -> proceed
      l = 6, r = 5 -> l < r false, inner loop never runs
      found: nothing

      (i = 4 and i = 5 are pure overhead: fewer than 3 elements remain from i
       onward. A loop bound of i < n - 2 would drop them, but the l < r test
       already makes them no-ops.)

    --------------------------------------------------------------------------
    RETURN res = { {-1, -1, 2}, {-1, 0, 1} }

    ==========================================================================
    Summary table
    ==========================================================================

    | i | nums[i] | dup-skip? | l,r walk             | recorded            |
    |---|---------|-----------|----------------------|---------------------|
    | 0 |   -4    | n/a (i=0) | l: 1->2->3->4->5     | none                |
    | 1 |   -1    | no        | (2,5) hit, (3,4) hit | {-1,-1,2}, {-1,0,1} |
    | 2 |   -1    | YES       | never entered        | none (would dup)    |
    | 3 |    0    | no        | (4,5) sum 3, r->4    | none                |
    | 4 |    1    | no        | l=5 = r, no loop     | none                |
    | 5 |    2    | no        | l=6 > r, no loop     | none                |

    The three levels of duplicate handling, in one place:
      i-level   if (i > 0 && nums[i] == nums[i-1]) continue;
                fires once here, at i = 2. Prevents re-running an entire
                two-pointer sweep for a repeated smallest element.
      l-level   while (l < r && nums[l] == nums[l-1]) l++;
                runs ONLY after a hit, and only because l was already
                incremented - so nums[l-1] is the value just consumed. Never
                fired for this input; it would for input like
                {-2, 0, 0, 0, 2} where three 0s sit between the pointers.
      r-level   while (l < r && nums[r] == nums[r+1]) r--;
                mirror image: r was already decremented, so nums[r+1] is the
                value just consumed. Also never fired here.

    Step count behind the O(n^2):
      6 outer iterations, of which 1 was skipped outright and 2 were no-ops.
      The inner two-pointer loops ran 4 + 2 + 1 + 0 + 0 = 7 times in total.
      For each i the two pointers together traverse at most n - i - 1 slots
      and never revisit one, so each sweep is O(n) and the whole thing is
      O(n^2) - not O(n^3) like the naive triple loop, which would have tested
      6*5*4/6 = 20 candidate triplets.

    Key subtlety: the l/r skips must come AFTER the push_back, never before.
    Recording the triplet first and then skipping means each distinct VALUE
    triple is emitted exactly once; skipping first would let a genuine triplet
    that needs two equal values (like {-1, -1, 2}) be lost.
*/
