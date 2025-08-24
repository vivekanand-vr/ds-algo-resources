#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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

// Time Complexity: O(n) — single pass through array, O(1) avg for hash map ops
// Space Complexity: O(n) — storing prefix sums in hash map

