#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the length of the longest consecutive elements sequence in an unsorted array

/*
    Approach: Hash Set
    - Insert all numbers into a set for O(1) lookup
    - For each number, only start expanding if it is the "start" of a sequence
      (i.e., num - 1 is not present in the set)
    - Expand forward until the sequence ends, counting its length
    - Track the longest sequence length found

    Algorithm Steps
    ----------------
    1. Insert all elements of nums into an unordered_set
    2. For each number in the set, check if num - 1 exists; if not, it's a sequence start
    3. From the start, keep incrementing and checking presence of the next number
    4. Track the length of each sequence and update the longest streak
    5. Return the longest streak found

    Time Complexity: O(n) - each element is visited at most twice
    Space Complexity: O(n) - set storage
*/
int longestConsecutive(vector<int>& nums) {
    if (nums.empty()) return 0;

    // Put all numbers in a set for O(1) lookup
    unordered_set<int> s(nums.begin(), nums.end());
    int longest = 0;

    // Traverse through each number
    for (int num : s) {
        // Check if this is the start of a sequence
        // (i.e., num-1 is not present)
        if (s.find(num - 1) == s.end()) {
            int currNum = num;
            int streak = 1;

            // Count how long the consecutive sequence goes
            while (s.find(currNum + 1) != s.end()) {
                currNum++;
                streak++;
            }

            // Update maximum streak
            longest = max(longest, streak);
        }
    }

    return longest;
}

int main() {
    vector<int> nums = {100, 4, 200, 1, 3, 2};

    cout << "Array: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    cout << "Longest consecutive sequence length: " << longestConsecutive(nums) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: nums = {100, 4, 200, 1, 3, 2}   (n = 6, answer = 4)
    ==========================================================================

      index:    0    1    2    3    4    5
      value:  100    4  200    1    3    2

      after  unordered_set s(nums.begin(), nums.end()):
        s = {1, 2, 3, 4, 100, 200}      (6 distinct values)

      NOTE: an unordered_set has no defined iteration order, so the range-for
      may hand the values over in any sequence. The trace below walks them in
      ascending order purely for readability - the final answer does not
      depend on the order, only on which values are present.

    Tracked state:
      s        - the hash set of all distinct values, used for O(1) lookups
      num      - the candidate currently pulled out of the set
      currNum  - walking cursor while extending a sequence forward
      streak   - length of the sequence being walked right now
      longest  - best streak seen so far

    Initial state: longest = 0

    --------------------------------------------------------------------------
    num = 1
      guard        lookup 1 - 1 = 0 in s -> ABSENT
                   -> 1 IS the start of a sequence, walk forward
      init         currNum = 1, streak = 1
      walk         step 1: lookup 2 present -> currNum = 2, streak = 2
                   step 2: lookup 3 present -> currNum = 3, streak = 3
                   step 3: lookup 4 present -> currNum = 4, streak = 4
                   step 4: lookup 5 ABSENT  -> stop walking
      measure      longest = max(0, 4) = 4                  <-- new best

                     1 -> 2 -> 3 -> 4 -> (5 missing)
                     ^                   streak = 4
                     start (0 missing)

    --------------------------------------------------------------------------
    num = 2
      guard        lookup 2 - 1 = 1 in s -> PRESENT
                   -> 2 is in the MIDDLE of a sequence, SKIP entirely
      walk         not entered - no lookups, no work
      measure      longest = 4 (untouched)

    --------------------------------------------------------------------------
    num = 3
      guard        lookup 2 in s -> PRESENT -> SKIP
      measure      longest = 4

    --------------------------------------------------------------------------
    num = 4
      guard        lookup 3 in s -> PRESENT -> SKIP
      measure      longest = 4

      The three skips above are the whole reason this stays linear. Values
      2, 3 and 4 were already crossed once, by the walk that started at 1:

           1   2   3   4          walked once, starting from 1
           ^   x   x   x          x = skipped later by the num-1 guard

    --------------------------------------------------------------------------
    num = 100
      guard        lookup 99 in s -> ABSENT -> start of a sequence
      init         currNum = 100, streak = 1
      walk         step 1: lookup 101 ABSENT -> stop immediately
      measure      longest = max(4, 1) = 4   (unchanged)

    --------------------------------------------------------------------------
    num = 200
      guard        lookup 199 in s -> ABSENT -> start of a sequence
      init         currNum = 200, streak = 1
      walk         step 1: lookup 201 ABSENT -> stop immediately
      measure      longest = max(4, 1) = 4   (unchanged)
      set exhausted -> loop ends

    --------------------------------------------------------------------------
    RETURN longest = 4        (the run 1, 2, 3, 4)

    ==========================================================================
    Summary table
    ==========================================================================

    | num | lookup num-1 | start? | forward walk    | streak | longest |
    |-----|--------------|--------|-----------------|--------|---------|
    | 1   | 0  absent    | yes    | 2, 3, 4, stop 5 |   4    |    4    |
    | 2   | 1  present   | no     | skipped         |   -    |    4    |
    | 3   | 2  present   | no     | skipped         |   -    |    4    |
    | 4   | 3  present   | no     | skipped         |   -    |    4    |
    | 100 | 99 absent    | yes    | stop at 101     |   1    |    4    |
    | 200 | 199 absent   | yes    | stop at 201     |   1    |    4    |

    Step count behind the O(n) claim:
      6 guard lookups (one per value in the set) plus 4 + 1 + 1 = 6 walk
      lookups = 12 hash lookups in total for 6 values, i.e. about 2n. That
      matches the "each element is visited at most twice" claim above: once
      as a guard, and at most once as a step of the single walk belonging to
      its own sequence.

    The subtlety that makes it linear:
      without the "num - 1 is absent" guard, the algorithm would also walk
      forward from 2 (3 steps), from 3 (2 steps) and from 4 (1 step). On one
      run of length L that is L + (L-1) + ... + 1 = O(L^2) work, so an input
      of 1, 2, ..., n would degrade to quadratic. The guard makes each
      sequence walked exactly once, from its true left end.

    One more thing the set buys:
      duplicates collapse on insertion. If nums were {1, 2, 2, 3} the set is
      still {1, 2, 3} and the answer is 3 - no special-case code needed.
*/
