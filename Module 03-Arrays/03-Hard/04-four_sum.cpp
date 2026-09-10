#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find all unique quadruplets in the array that sum up to the given target

/*
    Approach: Sort + Nested Loops + Two Pointers
    - Sort the array so duplicates are adjacent and two-pointer scanning works
    - Fix the first two elements of the quadruplet with nested loops, then use
      two pointers on the remaining subarray to find the last two elements
    - Skip duplicate values at every position (i, j, l, r) to avoid duplicate
      quadruplets in the result
    - Use a wider integer type (long) for the running sum to avoid overflow

    Algorithm Steps
    ----------------
    1. Sort nums in ascending order
    2. For each index i, skip if it duplicates the previous fixed first element
    3. For each index j > i, skip if it duplicates the previous fixed second element
    4. Set l = j+1, r = n-1 and move pointers based on the quadruplet sum
    5. If sum == target, record the quadruplet and skip duplicates for l and r
    6. If sum < target, move l right; if sum > target, move r left
    7. Return all collected quadruplets

    Time Complexity: O(n^3) - sorting O(n log n) plus two nested loops (O(n^2)) each doing an O(n) two-pointer scan
    Space Complexity: O(1) - excluding the space used for the output
*/
vector<vector<int>> fourSum(vector<int>& nums, int target) {
    vector<vector<int>> res;
    int n = nums.size();

    // Step 1: Sort the array
    sort(nums.begin(), nums.end());

    // Step 2: Fix the first number
    for (int i = 0; i < n; i++) {
        // Skip duplicates for first number
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        // Step 3: Fix the second number
        for (int j = i + 1; j < n; j++) {
            // Skip duplicates for second number
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;

            int l = j + 1;       // left pointer
            int r = n - 1;       // right pointer

            // Step 4: Use two pointers for remaining two numbers
            while (l < r) {
                long sum = (long)nums[i] + nums[j] + nums[l] + nums[r]; // current sum

                if (sum == target) {
                    // Found a valid quadruplet
                    res.push_back({nums[i], nums[j], nums[l], nums[r]});
                    l++;
                    r--;

                    // Skip duplicates for left pointer
                    while (l < r && nums[l] == nums[l - 1]) l++;
                    // Skip duplicates for right pointer
                    while (l < r && nums[r] == nums[r + 1]) r--;
                }
                else if (sum < target) {
                    // Sum is too small → move left pointer
                    l++;
                }
                else {
                    // Sum is too large → move right pointer
                    r--;
                }
            }
        }
    }

    return res;
}

int main() {
    vector<int> nums = {1, 0, -1, 0, -2, 2};
    int target = 0;

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;
    cout << "Target: " << target << endl;

    vector<vector<int>> result = fourSum(nums, target);

    cout << "Quadruplets that sum to target:" << endl;
    for (auto& quad : result) {
        for (int x : quad) cout << x << " ";
        cout << endl;
    }

    return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = {1, 0, -1, 0, -2, 2}, target = 0
             (n = 6, answer = {{-2,-1,1,2}, {-2,0,0,2}, {-1,0,0,1}})
    ==========================================================================

    BEFORE the sort (the order main() passes in):

      index:   0   1   2   3   4   5
      nums:    1   0  -1   0  -2   2

    AFTER sort(nums.begin(), nums.end())  -- all indices below refer to THIS
    layout. Sorting is what brings the two 0s together, which is the only
    reason every duplicate check can be a plain neighbour comparison:

      index:   0   1   2   3   4   5
      nums:   -2  -1   0   0   1   2
                       ^   ^
                       +---+-- the duplicate pair that drives every skip

    Tracked state:
      i    - index of the FIXED first element
      j    - index of the FIXED second element, always > i
      l    - left pointer, starts at j+1, only moves right
      r    - right pointer, starts at n-1, only moves left
      sum  - (long)nums[i] + nums[j] + nums[l] + nums[r], compared to target
      res  - the collected quadruplets

    Initial state: res = {}, n = 6, target = 0

    ==========================================================================
    i = 0, nums[i] = -2         (i == 0, i-level dup check skipped)
    ==========================================================================

    --------------------------------------------------------------------------
    i = 0, j = 1, nums[j] = -1
      j-level dup check: j > i+1?  1 > 1?  NO -> check short-circuits, no skip
        (the FIRST j of every i is always exempt - there is no previous j
         inside this i to duplicate)
      l = 2, r = 5      fixed pair sums to -3, so we need a pair summing to +3

        -2  -1   0   0   1   2
         ^   ^   ^           ^
         i   j   l           r

      step 1: sum = -2 + (-1) + nums[2] + nums[5] = -3 + 0 + 2 = -1 < 0 -> l=3
      step 2: sum = -3 + nums[3] + nums[5] = -3 + 0 + 2 = -1        < 0 -> l=4
              (nums[3] duplicates nums[2]; the l-level skip does NOT run on
               the sum<target branch, so the same value is simply re-tested
               and fails again. Harmless, just not free.)
      step 3: sum = -3 + nums[4] + nums[5] = -3 + 1 + 2 = 0         HIT
              record {-2, -1, 1, 2}
              l -> 5, r -> 4
              skip dup l: l < r (5 < 4)? NO -> body never runs
              skip dup r: l < r (5 < 4)? NO -> body never runs
              pointers crossed, inner loop ends

    --------------------------------------------------------------------------
    i = 0, j = 2, nums[j] = 0
      j-level dup check: j > i+1 (2 > 1, true) and nums[2] == nums[1]?
                         0 == -1?  NO -> proceed
      l = 3, r = 5      fixed pair sums to -2, so we need a pair summing to +2

      step 1: sum = -2 + 0 + nums[3] + nums[5] = -2 + 0 + 2 = 0     HIT
              record {-2, 0, 0, 2}
              l -> 4, r -> 4
              skip dup l: l < r (4 < 4)? NO
              skip dup r: l < r (4 < 4)? NO
              l == r, inner loop ends

    --------------------------------------------------------------------------
    i = 0, j = 3, nums[j] = 0        <-- SKIPPED by the j-level check
      j-level dup check: j > i+1 (3 > 1) and nums[3] == nums[2]?  0 == 0?
                         YES -> continue

      Why: with i = 0 and the second element fixed at value 0, every valid
      quadruplet was already enumerated by j = 2, whose two-pointer window
      [3, 5] strictly CONTAINS this one's window [4, 5]. Running j = 3 would
      give l = 4, r = 5, sum = -2 + 0 + 1 + 2 = 1 != 0 - no hit here, but in
      general it would re-emit j = 2's answers verbatim.

    --------------------------------------------------------------------------
    i = 0, j = 4, nums[j] = 1
      j-level dup check: 1 == nums[3] = 0?  NO -> proceed
      l = 5, r = 5 -> l < r false immediately, inner loop never runs

    --------------------------------------------------------------------------
    i = 0, j = 5, nums[j] = 2
      j-level dup check: 2 == nums[4] = 1?  NO -> proceed
      l = 6, r = 5 -> l < r false, inner loop never runs

    ==========================================================================
    i = 1, nums[i] = -1
      i-level dup check: i > 0 and nums[1] == nums[0]?  -1 == -2?  NO
    ==========================================================================

    --------------------------------------------------------------------------
    i = 1, j = 2, nums[j] = 0
      j-level dup check: j > i+1?  2 > 2?  NO -> exempt (first j of this i)

      The guard is `j > i + 1`, not `j > 0`, precisely so this first j always
      gets through: nums[j] may legitimately equal nums[i] (i and j are two
      different slots of the quadruplet), and comparing against nums[j-1]
      when j-1 == i would be comparing against the fixed element itself.
      See (i=2, j=3) below, where that distinction actually bites.

      l = 3, r = 5      fixed pair sums to -1, so we need a pair summing to +1

        -2  -1   0   0   1   2
             ^   ^   ^       ^
             i   j   l       r

      step 1: sum = -1 + 0 + nums[3] + nums[5] = -1 + 0 + 2 = 1     > 0 -> r=4
      step 2: sum = -1 + 0 + nums[3] + nums[4] = -1 + 0 + 1 = 0     HIT
              record {-1, 0, 0, 1}
              l -> 4, r -> 3
              skip dup l: l < r (4 < 3)? NO
              skip dup r: l < r (4 < 3)? NO
              pointers crossed, inner loop ends

    --------------------------------------------------------------------------
    i = 1, j = 3, nums[j] = 0        <-- SKIPPED
      j-level dup check: j > i+1 (3 > 2) and nums[3] == nums[2]?  0 == 0?
                         YES -> continue
      Had it run: l = 4, r = 5, sum = -1 + 0 + 1 + 2 = 2, no hit. But the
      window [4,5] is a subset of j = 2's window [3,5], so nothing new could
      ever be found - only repeats.

    --------------------------------------------------------------------------
    i = 1, j = 4, nums[j] = 1
      dup check: 1 == 0? NO. l = 5, r = 5 -> l < r false, no inner loop
    --------------------------------------------------------------------------
    i = 1, j = 5, nums[j] = 2
      dup check: 2 == 1? NO. l = 6, r = 5 -> l < r false, no inner loop

    ==========================================================================
    i = 2, nums[i] = 0
      i-level dup check: nums[2] == nums[1]?  0 == -1?  NO -> proceed
    ==========================================================================

    --------------------------------------------------------------------------
    i = 2, j = 3, nums[j] = 0
      j-level dup check: j > i+1?  3 > 3?  NO -> exempt.
      This is the case the exemption exists for: nums[j] == nums[i] == 0, and
      the quadruplet is ALLOWED to use both 0s. A `j > 0` guard would have
      wrongly skipped here.
      l = 4, r = 5
      step 1: sum = 0 + 0 + nums[4] + nums[5] = 0 + 1 + 2 = 3       > 0 -> r=4
              l = 4, r = 4 -> l < r false, inner loop ends
      found: nothing (the two remaining values are both positive)

    --------------------------------------------------------------------------
    i = 2, j = 4, nums[j] = 1
      dup check: j > 3 and nums[4] == nums[3]?  1 == 0?  NO -> proceed
      l = 5, r = 5 -> l < r false, no inner loop
    --------------------------------------------------------------------------
    i = 2, j = 5, nums[j] = 2
      dup check: 2 == 1? NO. l = 6, r = 5 -> no inner loop

    ==========================================================================
    i = 3, nums[i] = 0          <-- SKIPPED by the i-level check
      i-level dup check: i > 0 and nums[3] == nums[2]?  0 == 0?  YES
                         -> continue, the entire j loop is never entered
      All quadruplets whose smallest element is 0 were enumerated at i = 2,
      which had the larger remaining window [3, 5] to work with.
    ==========================================================================

    ==========================================================================
    i = 4, nums[i] = 1     dup check: 1 == 0? NO -> proceed
      j = 5: exempt (j > i+1 -> 5 > 5 false). l = 6, r = 5 -> no inner loop
    i = 5, nums[i] = 2     dup check: 2 == 1? NO -> proceed
      j loop starts at j = 6 which is not < n, so it never runs at all
    ==========================================================================

    --------------------------------------------------------------------------
    RETURN res = { {-2, -1, 1, 2}, {-2, 0, 0, 2}, {-1, 0, 0, 1} }

    ==========================================================================
    Summary table
    ==========================================================================

    | i | j | nums[i],[j] | j-skip? | l,r result            | recorded      |
    |---|---|-------------|---------|-----------------------|---------------|
    | 0 | 1 |  -2, -1     | exempt  | l 2->3->4, hit (4,5)  | {-2,-1,1,2}   |
    | 0 | 2 |  -2,  0     | no      | hit (3,5)             | {-2,0,0,2}    |
    | 0 | 3 |  -2,  0     | YES     | never entered         | -             |
    | 0 | 4 |  -2,  1     | no      | l=5 = r, no loop      | -             |
    | 0 | 5 |  -2,  2     | no      | l=6 > r, no loop      | -             |
    | 1 | 2 |  -1,  0     | exempt  | r 5->4, hit (3,4)     | {-1,0,0,1}    |
    | 1 | 3 |  -1,  0     | YES     | never entered         | -             |
    | 1 | 4 |  -1,  1     | no      | no loop               | -             |
    | 1 | 5 |  -1,  2     | no      | no loop               | -             |
    | 2 | 3 |   0,  0     | exempt  | sum 3, r->4, ends     | -             |
    | 2 | 4 |   0,  1     | no      | no loop               | -             |
    | 2 | 5 |   0,  2     | no      | no loop               | -             |
    | 3 | - |   0,  -     | i-SKIP  | j loop never entered  | -             |
    | 4 | 5 |   1,  2     | exempt  | no loop               | -             |
    | 5 | - |   2,  -     | no      | j loop never runs     | -             |

    All four levels of duplicate handling, and what each one fired on:
      i-level   if (i > 0 && nums[i] == nums[i-1]) continue;
                fired once, at i = 3 (the second 0).
      j-level   if (j > i + 1 && nums[j] == nums[j-1]) continue;
                fired twice, at (i=0, j=3) and (i=1, j=3).
                The `j > i + 1` half is the piece people get wrong: the first
                j of each i must be exempt, otherwise (i=2, j=3) - which is
                the only pass that can ever use BOTH 0s as the first two
                elements - would be thrown away.
      l-level   while (l < r && nums[l] == nums[l-1]) l++;
      r-level   while (l < r && nums[r] == nums[r+1]) r--;
                Neither fired for this input: every hit left the pointers
                already crossed or touching, so the l < r guard short-
                circuited first. They matter for inputs like
                {0,0,0,0,0,0} with target 0, where after the first hit l
                would otherwise re-find the identical quadruplet.
                Both read the value JUST CONSUMED (nums[l-1] after l++,
                nums[r+1] after r--), which only works because the skip runs
                after the increment, not before.

    Step count behind the O(n^3):
      14 (i, j) pairs were reachable; 3 were skipped as duplicates or had an
      empty j range, and the two-pointer loop bodies ran
      3 + 1 + 0 + 0 + 0 + 2 + 0 + 0 + 0 + 1 + 0 + 0 + 0 = 7 times total.
      Each (i, j) sweep is O(n) because l and r only ever move toward each
      other, so with O(n^2) fixed pairs the total is O(n^3) - versus
      C(6,4) = 15 candidate quadruplets for the naive quadruple loop at this
      tiny n, but n^3 vs n^4 as n grows.

    Why the (long) cast on line `long sum = (long)nums[i] + ...`:
      four ints near INT_MAX would overflow a 32-bit sum before the
      comparison. Casting only the FIRST operand is enough - it promotes the
      whole right-hand expression to long, so each subsequent addition is
      done in the wider type.
*/
