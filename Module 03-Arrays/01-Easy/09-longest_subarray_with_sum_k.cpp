#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the length of the longest subarray whose sum equals k (array may contain negative numbers)

/*
    Approach: Prefix Sum + Hash Map
    - Maintain a running prefix sum while scanning the array
    - If the prefix sum itself equals k, the subarray from index 0 is a candidate
    - If (prefixSum - k) was seen before at some index p, the subarray between
      p+1 and the current index sums to exactly k
    - Store only the FIRST occurrence of each prefix sum, since an earlier
      occurrence gives a longer subarray

    Algorithm Steps
    ----------------
    1. Traverse the array, updating a running prefix sum
    2. If sum == k, update maxLen with i + 1
    3. If (sum - k) exists in the map, update maxLen with i - prefixIndex[sum - k]
    4. Record the first occurrence of each prefix sum
    5. Return maxLen

    Time Complexity: O(n) - single pass through array, O(1) avg for hash map ops
    Space Complexity: O(n) - storing prefix sums in hash map
*/
int longestSubarray(vector<int>& arr, int k) {
    unordered_map<int, int> prefixIndex; // stores first occurrence of each prefix sum
    int sum = 0, maxLen = 0;

    for (int i = 0; i < arr.size(); i++) {
        sum += arr[i]; // update running prefix sum

        // Case 1: subarray from index 0 to i has sum = k
        if (sum == k) {
            maxLen = max(maxLen, i + 1);
        }

        // Case 2: check if there exists a prefix sum such that
        // currentSum - previousSum = k  => subarray in between has sum = k
        if (prefixIndex.find(sum - k) != prefixIndex.end()) {
            maxLen = max(maxLen, i - prefixIndex[sum - k]);
        }

        // Store first occurrence of current prefix sum
        // (we don't update because we want the longest length)
        if (prefixIndex.find(sum) == prefixIndex.end()) {
            prefixIndex[sum] = i;
        }
    }

    return maxLen;
}

int main() {
    vector<int> arr = {10, 5, 2, 7, 1, -10};
    int k = 15;

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Longest subarray with sum " << k << ": " << longestSubarray(arr, k) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {10, 5, 2, 7, 1, -10}, k = 15   (n = 6, answer = 6)
    ==========================================================================

      index:   0   1   2   3   4   5
      value:  10   5   2   7   1 -10
      prefix: 10  15  17  24  25  15      <-- running sum after each index

    Tracked state:
      sum         - prefix sum arr[0] + ... + arr[i]
      prefixIndex - prefix sum -> the FIRST index at which it occurred
                    (an entry is written only if the key is absent)
      maxLen      - best subarray length found so far

    Key identity: sum(arr[p+1 .. i]) = prefix[i] - prefix[p].
    So to get a window summing to k we want prefix[p] = sum - k.

    Initial state: sum = 0, maxLen = 0, prefixIndex = {}
    (note: prefixIndex is NOT pre-seeded with {0: -1}; the code handles the
     "window starts at index 0" case with a separate sum == k check)

    --------------------------------------------------------------------------
    i = 0, arr[i] = 10
      accumulate   sum = 0 + 10 = 10
      case 1       sum == k ?  10 == 15 -> no
      case 2       lookup sum - k = 10 - 15 = -5 in map -> absent, no hit
      store        key 10 absent -> prefixIndex[10] = 0
      state        maxLen = 0, prefixIndex = {10:0}

    --------------------------------------------------------------------------
    i = 1, arr[i] = 5
      accumulate   sum = 10 + 5 = 15
      case 1       sum == k ?  15 == 15 -> YES, window arr[0..1]
                   measure  maxLen = max(0, i + 1) = max(0, 2) = 2
      case 2       lookup sum - k = 0 in map -> absent, no hit
                   (this is exactly the gap the case-1 check covers: a
                    window anchored at index 0 needs prefix 0, which was
                    never stored)
      store        key 15 absent -> prefixIndex[15] = 1
      state        maxLen = 2, prefixIndex = {10:0, 15:1}

    --------------------------------------------------------------------------
    i = 2, arr[i] = 2
      accumulate   sum = 15 + 2 = 17
      case 1       17 == 15 -> no
      case 2       lookup 17 - 15 = 2 -> absent, no hit
      store        prefixIndex[17] = 2
      state        maxLen = 2, prefixIndex = {10:0, 15:1, 17:2}

    --------------------------------------------------------------------------
    i = 3, arr[i] = 7
      accumulate   sum = 17 + 7 = 24
      case 1       24 == 15 -> no
      case 2       lookup 24 - 15 = 9 -> absent, no hit
      store        prefixIndex[24] = 3
      state        maxLen = 2, prefixIndex = {10:0, 15:1, 17:2, 24:3}

    --------------------------------------------------------------------------
    i = 4, arr[i] = 1
      accumulate   sum = 24 + 1 = 25
      case 1       25 == 15 -> no
      case 2       lookup 25 - 15 = 10 -> FOUND at index p = 0
                   step 1: p = prefixIndex[10] = 0
                   step 2: the matching window is arr[p+1 .. i] = arr[1..4]
                           = {5, 2, 7, 1}, sum = 15  (check: 25 - 10 = 15)
                   step 3: its length is i - p = 4 - 0 = 4
                           (NOT i - p + 1: p itself is excluded, the window
                            begins at p + 1)
                   measure maxLen = max(2, 4) = 4              <-- new best

                     10   5   2   7   1  -10
                      p   |___________|
                     ^         sum = 15, len 4
                     prefix 10 ends HERE, so the window starts after it

      store        prefixIndex[25] = 4
      state        maxLen = 4, prefixIndex = {10:0, 15:1, 17:2, 24:3, 25:4}

    --------------------------------------------------------------------------
    i = 5, arr[i] = -10       <-- the negative number makes prefix REPEAT
      accumulate   sum = 25 + (-10) = 15      (same prefix as at i = 1)
      case 1       sum == k ? 15 == 15 -> YES, window arr[0..5]
                   measure  maxLen = max(4, i + 1) = max(4, 6) = 6  <-- best
      case 2       lookup 15 - 15 = 0 -> absent, no hit
      store        key 15 is ALREADY present (mapped to 1) -> NOT overwritten
                   step 1: find(15) != end() -> skip the write
                   step 2: prefixIndex[15] stays 1, not 5
                   why: a later index would only ever yield SHORTER windows.
                        Keeping the first occurrence is what guarantees the
                        maximum length.
      state        maxLen = 6, prefixIndex unchanged -> loop ends

    --------------------------------------------------------------------------
    RETURN maxLen = 6        (the whole array: 10+5+2+7+1-10 = 15)

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i] | sum | sum==k? | sum-k | found at | len | maxLen | stored |
    |---|--------|-----|---------|-------|----------|-----|--------|--------|
    | 0 |   10   | 10  |   no    |  -5   |    -     |  -  |   0    | 10:0   |
    | 1 |    5   | 15  |  yes    |   0   |    -     |  2  |   2    | 15:1   |
    | 2 |    2   | 17  |   no    |   2   |    -     |  -  |   2    | 17:2   |
    | 3 |    7   | 24  |   no    |   9   |    -     |  -  |   2    | 24:3   |
    | 4 |    1   | 25  |   no    |  10   |    0     |  4  |   4    | 25:4   |
    | 5 |  -10   | 15  |  yes    |   0   |    -     |  6  |   6    | none   |

    (prefixIndex is an unordered_map, so its real internal order is
     unspecified - it is shown here as a plain set of key:value pairs.)

    The subtlety a reader trips on:
      prefix 15 appears TWICE (i = 1 and i = 5). Because negatives are
      allowed, prefix sums are not monotonic and repeats are normal. The
      "store only the first occurrence" rule is therefore load-bearing: had
      i = 5 overwritten prefixIndex[15] = 5, a later index needing prefix 15
      would compute a shorter window. This is also why the sliding-window
      shrink trick used for all-positive arrays does not work here.
*/
