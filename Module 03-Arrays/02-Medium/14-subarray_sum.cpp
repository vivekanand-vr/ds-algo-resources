#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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

/*
Logic:
------
- Maintain prefix sum: curr = arr[0] + arr[1] + ... + arr[i].
- For each index i, check if (curr - k) appeared before.
  If yes → some subarray sums to k.
- Use hashmap to count how many times each prefix sum appeared.

Time Complexity:  O(n) → single pass
Space Complexity: O(n) → hashmap storage
*/
