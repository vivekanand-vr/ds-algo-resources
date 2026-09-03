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
