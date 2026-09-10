#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array arr and a target k, return indices of the two numbers such that they add up to k

/*
    Approach: Hash Map (value -> index)
    - Traverse the array once, and for each element check if its complement
      (k - arr[i]) has already been seen
    - If found, those two indices form the answer
    - Otherwise, store the current element's value and index for future lookups

    Algorithm Steps
    ----------------
    1. Traverse the array from left to right
    2. For each element, check if (k - arr[i]) exists in the map
    3. If yes, return {index of complement, current index}
    4. Otherwise, insert current element's value and index into the map
    5. If no pair found, return {0, 1} (per problem constraints, shouldn't happen)

    Time Complexity: O(n log n) - single pass, each map lookup/insert is O(log n)
    Space Complexity: O(n) - to store up to n elements in the map
*/
vector<int> twoSum(vector<int>& arr, int k) {
    int n = arr.size();
    map<int, int> mp;  // stores {value -> index} of elements encountered so far

    // Traverse the array once
    for(int i = 0; i < n; ++i) {
        // Check if the complement (k - arr[i]) already exists in the map
        if(mp.find(k - arr[i]) != mp.end()) {
            // If yes, return indices of complement and current element
            return { mp[k - arr[i]], i };
        }
        // Otherwise, store current element with its index in the map
        mp[arr[i]] = i;
    }

    // If no pair is found (as per problem constraints, ideally won't happen)
    return { 0, 1 };
}

int main() {
    vector<int> arr = {2, 7, 11, 15};
    int k = 9;

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    vector<int> result = twoSum(arr, k);
    cout << "Indices summing to " << k << ": " << result[0] << ", " << result[1] << endl;

    return 0;
}

/*
    ==========================================================================
    DRY RUN: arr = {2, 7, 11, 15}, k = 9   (n = 4, answer = {0, 1})
    ==========================================================================

      index:   0   1   2   3
      value:   2   7  11  15

    Tracked state:
      mp   - value -> index, for every element ALREADY passed (left of i)
      i    - index currently being examined
      need - the complement k - arr[i]; the partner arr[i] is looking for

    Initial state: i = 0, mp = {}   (nothing seen yet)

    --------------------------------------------------------------------------
    i = 0, arr[i] = 2
      need         k - arr[i] = 9 - 2 = 7
      lookup       mp.find(7) in mp = {}   -> end(), not present
      store        mp[2] = 0               -> mp = {2:0}
      advance      i -> 1

    --------------------------------------------------------------------------
    i = 1, arr[i] = 7
      need         k - arr[i] = 9 - 7 = 2
      lookup       mp.find(2) in mp = {2:0} -> FOUND, mp[2] = 0
      return       { mp[2], i } = { 0, 1 }            <-- exits here

                       2    7   11   15
                       ^    ^
                    stored  current
                    idx 0   idx 1        2 + 7 = 9 = k

    --------------------------------------------------------------------------
    RETURN {0, 1}        arr[0] + arr[1] = 2 + 7 = 9

    Indices 2 and 3 are never even read - the loop returns on the second
    iteration.

    ==========================================================================
    Summary table
    ==========================================================================

    | i | arr[i] | need | in mp? | mp after step | action        |
    |---|--------|------|--------|---------------|---------------|
    | 0 |   2    |  7   | no     | {2:0}         | store 2 -> 0  |
    | 1 |   7    |  2   | yes(0) | {2:0}         | return {0,1}  |

    Why the lookup direction matters:
      the map only ever holds elements to the LEFT of i, so the pair returned
      is always {smaller index, larger index} and an element can never pair
      with itself. Storing arr[i] AFTER the lookup is what guarantees that -
      swapping those two lines would let arr[i] match itself whenever
      2 * arr[i] == k (e.g. arr = {4, 4}, k = 8 would wrongly return {0, 0}).

    Duplicate values:
      mp[arr[i]] = i OVERWRITES an earlier index for the same value, so the
      map keeps the RIGHTMOST occurrence seen so far. Harmless here, since a
      match returns immediately, but it means a value's stored index is not
      necessarily its first occurrence.

    Step count backing O(n log n):
      2 iterations, each doing one find and at most one insert. In the worst
      case (no early hit) that is n finds + n inserts = 2n map operations,
      each O(log n) on a std::map. An unordered_map would make it O(n).
*/
