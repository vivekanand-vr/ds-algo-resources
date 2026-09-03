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
