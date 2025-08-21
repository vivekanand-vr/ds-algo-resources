#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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

/*
Time Complexity:  
O(n) → we traverse the array once, and each map lookup/insert takes O(log n).  
So overall: O(n log n).  

Space Complexity:  
O(n) → to store up to 'n' elements in the map.  
*/
