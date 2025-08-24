#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int subarraysWithXorK(vector<int>& arr, int k) {
    int n = arr.size();
    int count = 0;

    // Hashmap to store frequency of prefix XORs
    unordered_map<int, int> freq;

    int xorSum = 0; // prefix XOR till current index

    for (int i = 0; i < n; i++) {
        xorSum ^= arr[i]; // update prefix XOR

        // Case 1: If prefix XOR itself equals k, then subarray [0..i] works
        if (xorSum == k) count++;

        // Case 2: Check if there exists a prefix XOR such that:
        // (xorSum ^ prevXor = k) → prevXor = xorSum ^ k
        int target = xorSum ^ k;
        if (freq.find(target) != freq.end()) {
            count += freq[target];
        }

        // Store/update frequency of current prefix XOR
        freq[xorSum]++;
    }

    return count;
}

/*
Logic:
------
- Maintain a prefix XOR (xorSum), which is the XOR of all elements from index 0 to i.
- A subarray (l..r) has XOR = k if:
       prefixXor[r] ^ prefixXor[l-1] = k
    ⇒ prefixXor[l-1] = prefixXor[r] ^ k
- So, while iterating:
- If xorSum == k, then subarray from start works.
- Else, check if (xorSum ^ k) exists in hashmap (meaning some earlier prefix can form a subarray).
- Add its frequency to result.
- Update frequency of current prefix XOR in hashmap.

Complexity:
-----------
  Time: O(n) (single pass)
  Space: O(n) (hashmap)
*/

// Another Varation
int subarraysWithXorK(vector<int> &arr, int t) {
    int res = 0, xr = 0, n = arr.size();
    map<int, int> vmap;

    vmap[0] = 1;  // base case: empty prefix has XOR = 0

    for(int i = 0; i < n; ++i) {
        xr = xr ^ arr[i];   // prefix XOR till index i
        int k = xr ^ t;     // target prefix that would give subarray XOR = t
        res += vmap[k];     // if target prefix exists, add its frequency
        vmap[xr]++;         // store/update current prefix XOR
    }
    return res;   
}
