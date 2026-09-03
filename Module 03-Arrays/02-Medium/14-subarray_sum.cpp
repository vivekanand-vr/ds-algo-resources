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
