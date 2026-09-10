#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the length of the longest subarray whose sum equals zero

/*
    Approach: Prefix Sum + Hash Map (special case of sum = k)
    - This reuses the prefix sum + hash map idea from the Easy problem
      "longest subarray with sum k", with k fixed to 0
    - Maintain a running prefix sum while scanning the array
    - If the prefix sum itself is 0, the subarray from index 0 is a candidate
    - If this exact prefix sum was seen before at some index p, the subarray
      between p+1 and the current index sums to 0
    - Store only the FIRST occurrence of each prefix sum, since an earlier
      occurrence gives a longer subarray

    Algorithm Steps
    ----------------
    1. Traverse the array, updating a running prefix sum
    2. If sum == 0, update maxLen with i + 1
    3. If sum exists in the map, update maxLen with i - prefixIndex[sum]
    4. Record the first occurrence of each prefix sum
    5. Return maxLen

    Time Complexity: O(n) - single pass through array, O(1) avg for hash map ops
    Space Complexity: O(n) - storing prefix sums in hash map
*/
int maxLength(vector<int>& arr) {
    int k = 0; // Refering the problem 09 form Arrays-Easy list longest sub array with sum k.
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
    vector<int> arr = {15, -2, 2, -8, 1, 7, 10, 23};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Length of longest subarray with sum 0: " << maxLength(arr) << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {15, -2, 2, -8, 1, 7, 10, 23}   (n = 8, answer = 5)
    ==========================================================================

      index:    0    1    2    3    4    5    6    7
      arr:     15   -2    2   -8    1    7   10   23
      prefix:  15   13   15    7    8   15   25   48
               ^         ^              ^
               |         |              |
               +---------+--------------+  the SAME prefix sum 15 appears at
                                           indices 0, 2 and 5 - and every
                                           repeat marks a zero-sum stretch

    The array is not sorted and must not be: prefix sums only mean anything
    in the original order.

    Tracked state:
      k            - the target sum, hardcoded to 0 for this problem
      sum          - running prefix sum arr[0] + ... + arr[i]
      prefixIndex  - map: prefix sum -> the FIRST index at which it occurred
      maxLen       - best length found so far

    The core identity: sum(arr[p+1..i]) = prefix[i] - prefix[p]. That equals
    k = 0 exactly when prefix[i] == prefix[p]. So with k = 0, the lookup key
    `sum - k` is just `sum` itself - we are literally asking "have I seen this
    exact running total before?"

    Initial state: sum = 0, maxLen = 0, prefixIndex = {}

    --------------------------------------------------------------------------
    i = 0, arr[i] = 15
      sum        0 + 15 = 15
      case 1     sum == 0?  15 == 0?  NO
      case 2     lookup sum - k = 15 - 0 = 15 in prefixIndex
                 prefixIndex is EMPTY -> not found, no update
      store      is 15 already a key?  NO -> prefixIndex[15] = 0
      state      maxLen = 0, prefixIndex = {15:0}

    --------------------------------------------------------------------------
    i = 1, arr[i] = -2
      sum        15 + (-2) = 13
      case 1     13 == 0?  NO
      case 2     lookup 13 -> not found
      store      13 is new -> prefixIndex[13] = 1
      state      maxLen = 0, prefixIndex = {15:0, 13:1}

    --------------------------------------------------------------------------
    i = 2, arr[i] = 2
      sum        13 + 2 = 15
      case 1     15 == 0?  NO
      case 2     lookup 15 -> FOUND at index 0
                 HIT: maxLen = max(0, i - 0) = max(0, 2 - 0) = 2
                 meaning: prefix[0] = prefix[2] = 15, so the elements
                 STRICTLY AFTER index 0 up to index 2 cancel out:
                 arr[1..2] = {-2, 2}, sum 0, length 2. Note the length is
                 i - p, NOT i - p + 1, because index p itself is excluded.
      store      15 is ALREADY a key (-> 0) -> NOT overwritten
                 This is the "first occurrence only" rule. Overwriting to
                 prefixIndex[15] = 2 would have destroyed the information
                 needed at i = 5 below.
      state      maxLen = 2, prefixIndex = {15:0, 13:1}

                   15  -2   2  -8   1   7  10  23
                   |    \______/
                   p=0    len 2

    --------------------------------------------------------------------------
    i = 3, arr[i] = -8
      sum        15 + (-8) = 7
      case 1     7 == 0?  NO
      case 2     lookup 7 -> not found
      store      7 is new -> prefixIndex[7] = 3
      state      maxLen = 2, prefixIndex = {15:0, 13:1, 7:3}

    --------------------------------------------------------------------------
    i = 4, arr[i] = 1
      sum        7 + 1 = 8
      case 1     8 == 0?  NO
      case 2     lookup 8 -> not found
      store      8 is new -> prefixIndex[8] = 4
      state      maxLen = 2, prefixIndex = {15:0, 13:1, 7:3, 8:4}

    --------------------------------------------------------------------------
    i = 5, arr[i] = 7
      sum        8 + 7 = 15
      case 1     15 == 0?  NO
      case 2     lookup 15 -> FOUND at index 0        <-- THE ANSWER
                 HIT: maxLen = max(2, 5 - 0) = 5
                 arr[1..5] = {-2, 2, -8, 1, 7}
                 check: -2 + 2 = 0, then -8 + 1 + 7 = 0, total 0. Length 5.
      store      15 already a key -> not overwritten (stays 0)
      state      maxLen = 5, prefixIndex unchanged

                   15  -2   2  -8   1   7  10  23
                   |    \________________/
                   p=0        len 5

                 THIS is the payoff for keeping only the first occurrence.
                 Had i = 2 overwritten prefixIndex[15] to 2, this step would
                 have computed 5 - 2 = 3 and reported the shorter subarray
                 arr[3..5] = {-8, 1, 7} - also a valid zero-sum stretch, but
                 not the longest. Keeping the earliest p maximises i - p.

    --------------------------------------------------------------------------
    i = 6, arr[i] = 10
      sum        15 + 10 = 25
      case 1     25 == 0?  NO
      case 2     lookup 25 -> not found
      store      25 is new -> prefixIndex[25] = 6
      state      maxLen = 5

    --------------------------------------------------------------------------
    i = 7, arr[i] = 23
      sum        25 + 23 = 48
      case 1     48 == 0?  NO
      case 2     lookup 48 -> not found
      store      48 is new -> prefixIndex[48] = 7
      state      maxLen = 5
      loop ends (i = 8 is not < size 8)

    --------------------------------------------------------------------------
    RETURN maxLen = 5        (arr[1..5] = {-2, 2, -8, 1, 7})

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i] | sum | lookup | found at | maxLen | map change     |
    |---|--------|-----|--------|----------|--------|----------------|
    | 0 |   15   |  15 |   15   |    -     |   0    | store 15:0     |
    | 1 |   -2   |  13 |   13   |    -     |   0    | store 13:1     |
    | 2 |    2   |  15 |   15   |    0     |   2    | none (15 kept) |
    | 3 |   -8   |   7 |    7   |    -     |   2    | store 7:3      |
    | 4 |    1   |   8 |    8   |    -     |   2    | store 8:4      |
    | 5 |    7   |  15 |   15   |    0     |   5    | none (15 kept) |
    | 6 |   10   |  25 |   25   |    -     |   5    | store 25:6     |
    | 7 |   23   |  48 |   48   |    -     |   5    | store 48:7     |

    (sum == 0 never held on any iteration, so Case 1 never fired.)

    Final map: {15:0, 13:1, 7:3, 8:4, 25:6, 48:7} - 6 keys for 8 elements,
    because the two repeats of 15 were deliberately not re-stored.

    Step count behind the O(n): 8 iterations, each doing at most 2 hash
    lookups and 1 insert = 24 O(1)-average map operations. The brute force
    over all subarrays would be 8*9/2 = 36 subarrays, each needing its own
    sum - O(n^2) at best, O(n^3) if summed naively.

    The invariant, stated once:
      after processing index i, prefixIndex maps each prefix value seen so far
      to the SMALLEST index producing it, and maxLen is the length of the
      longest zero-sum subarray ending at or before i.

    Two subtleties worth noticing in this particular code:
      1. Case 1 and Case 2 are not redundant, even though k = 0 makes the
         lookup key equal to sum. Case 1 handles a prefix that is itself 0,
         i.e. a subarray starting at index 0 with no earlier prefix to pair
         with - the "empty prefix" that has value 0 at position -1. It gives
         i + 1 (inclusive of index 0), while Case 2 gives i - p (exclusive of
         index p). Neither fires as case 1 for this input, since no prefix
         here is ever 0. Seeding prefixIndex[0] = -1 up front would collapse
         both branches into one: i - (-1) = i + 1.
      2. Both branches can fire on the same iteration (if sum == 0 AND 0 was
         already stored). That is harmless - i + 1 is always the larger of
         the two, and maxLen takes a max.
*/
