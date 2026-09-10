#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Find the total number of subarrays whose sum equals k

/*
    Approach: Prefix Sum + Hash Map
    - Maintain a running prefix sum: curr = arr[0] + arr[1] + ... + arr[i]
    - For each index i, check if (curr - k) appeared before; if yes, that many
      subarrays ending at i sum to exactly k
    - Use a hashmap to count how many times each prefix sum has appeared

    Algorithm Steps
    ----------------
    1. Initialize a map with mp[0] = 1 to account for subarrays starting at index 0
    2. Traverse the array, updating a running prefix sum 'curr'
    3. If (curr - k) exists in the map, add its count to the answer
    4. Increment the count of the current prefix sum in the map
    5. Return the total count

    Time Complexity: O(n) - single pass
    Space Complexity: O(n) - hashmap storage
*/
int subarraySum(vector<int>& arr, int k) {
    int n = arr.size();
    unordered_map<int, int> mp;   // prefix sum → count of occurrences
    mp[0] = 1;                    // base case: sum from start

    int curr = 0, c = 0;

    for (int x : arr) {
        curr += x;                // update prefix sum

        // If curr - k exists, then there’s a subarray ending here with sum = k
        if (mp.find(curr - k) != mp.end()) {
            c += mp[curr - k];
        }

        // Store/update current prefix sum in map
        mp[curr]++;
    }

    return c;
}

int main() {
    vector<int> arr = {1, 1, 1};
    int k = 2;

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Number of subarrays with sum " << k << ": " << subarraySum(arr, k) << endl;

    return 0;
}

/*
    ======================================================================
    DRY RUN: arr = {1,-1,0}, k = 0   (n = 3, answer = 3)

    Note: main() uses arr = {1,1,1}, k = 2 (which gives 2). That input
    has all-distinct prefix sums, so every lookup that hits contributes
    just 1 and the map's COUNT never matters. This trace uses
    {1,-1,0} with k = 0 - a negative, a zero, and a repeated prefix sum -
    because that is where the counting map earns its keep.
    ======================================================================

      index:   0    1    2
      value:   1   -1    0

      prefix:  1    0    0        curr after each step

    Tracked state:
      curr  - running prefix sum arr[0] + ... + arr[i]
      mp    - prefix sum -> how many indices have produced that sum,
              seeded with mp[0] = 1 for the empty prefix
      c     - number of subarrays with sum k found so far

    Initial state: curr = 0, c = 0, mp = {0:1}

    The identity being exploited: sum(arr[j+1..i]) = curr_i - curr_j.
    That equals k exactly when curr_j == curr_i - k. So "how many
    subarrays with sum k END at i" is just "how many earlier prefix sums
    equal curr - k" - which is a single map lookup, and the map stores
    COUNTS because the same prefix sum can occur at several indices.

    ----------------------------------------------------------------------
    x = 1        (index 0)
      prefix   curr = 0 + 1 = 1
      lookup   curr - k = 1 - 0 = 1
               mp.find(1) -> not present in {0:1}
               nothing added, c stays 0
      record   mp[1]++      -> mp = {0:1, 1:1}
      state    curr = 1, c = 0

    ----------------------------------------------------------------------
    x = -1       (index 1)     <-- a NEGATIVE pulls curr back down
      prefix   curr = 1 + (-1) = 0
      lookup   curr - k = 0 - 0 = 0
               mp.find(0) -> FOUND, mp[0] = 1
               c += 1  ->  c = 1
               the one match is the empty prefix (mp[0] = 1 seed),
               meaning the subarray starts at index 0:
                     arr[0..1] = 1 + (-1) = 0   <-- sum = k
      record   mp[0]++      -> mp = {0:2, 1:1}
      state    curr = 0, c = 1

               Note the prefix sum 0 has now occurred TWICE: once before
               the array (the seed) and once at index 1. This is exactly
               why mp stores counts and not a boolean - a set would lose
               the second occurrence and undercount later.

                   prefix:   0     1     0
                   at:     (pre)   i=0   i=1
                             ^___________^
                             equal -> the span between them sums to 0

    ----------------------------------------------------------------------
    x = 0        (index 2)     <-- a ZERO leaves curr unchanged
      prefix   curr = 0 + 0 = 0
      lookup   curr - k = 0 - 0 = 0
               mp.find(0) -> FOUND, mp[0] = 2
               c += 2  ->  c = 3            <-- TWO subarrays at once
               step 1: the match from the empty prefix  -> arr[0..2]
                       = 1 + (-1) + 0 = 0
               step 2: the match recorded at index 1    -> arr[2..2]
                       = 0
      record   mp[0]++      -> mp = {0:3, 1:1}
      state    curr = 0, c = 3   ->   range-for ends

               This is the payoff step: one lookup returned 2 and both
               subarrays were counted without ever being enumerated.

    ----------------------------------------------------------------------
    RETURN c = 3

      the three subarrays, verified directly:
        arr[0..1] =  1 + (-1)      =  0
        arr[0..2] =  1 + (-1) + 0  =  0
        arr[2..2] =  0             =  0
      (arr[0..0] = 1, arr[1..1] = -1, arr[1..2] = -1 do not qualify)

    ======================================================================
    Summary table
    ======================================================================

    | i | x  | curr | curr - k | mp[curr-k] | c | mp after record   |
    |---|----|------|----------|------------|---|-------------------|
    | - | -  |  0   |    -     |     -      | 0 | {0:1}             |
    | 0 |  1 |  1   |    1     |  absent    | 0 | {0:1, 1:1}        |
    | 1 | -1 |  0   |    0     |     1      | 1 | {0:2, 1:1}        |
    | 2 |  0 |  0   |    0     |     2      | 3 | {0:3, 1:1}        |

    Step count backing O(n): 3 iterations, each doing one find and one
    operator[] on an unordered_map - 6 hash operations, O(1) each on
    average, so ~n map touches total. The brute force would sum every
    subarray: 3*4/2 = 6 subarrays here and n^2/2 in general. Space is
    O(n) for the map, which holds at most n+1 distinct prefix sums.

    The one subtlety - the lookup must happen BEFORE mp[curr]++:
      with k = 0 the value being looked up IS curr itself. If the map
      were updated first, index 2 would read mp[0] = 3 instead of 2 and
      report 4 subarrays, the extra one being the empty subarray
      arr[3..2]. Querying first keeps the map strictly a record of
      prefixes that ENDED EARLIER than the current index.

    Why sliding window does not work here:
      the negative at index 1 makes the prefix sums non-monotonic
      (1, 0, 0), so growing the window does not reliably grow the sum
      and there is no correct "shrink when too big" rule. The prefix-sum
      map does not care about sign at all - it only compares values.
*/
