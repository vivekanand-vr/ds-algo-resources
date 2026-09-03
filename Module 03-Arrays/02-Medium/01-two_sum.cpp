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
