#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Count the number of subarrays whose XOR of all elements equals k

/*
    Approach 1: Prefix XOR + Unordered Map
    - Maintain a running prefix XOR (xorSum) while scanning the array
    - A subarray (l..r) has XOR = k exactly when prefixXor[r] ^ prefixXor[l-1] = k,
      i.e. prefixXor[l-1] = prefixXor[r] ^ k
    - If xorSum == k, the subarray from the start works
    - Otherwise check if (xorSum ^ k) was seen before; its frequency tells how
      many valid earlier prefixes complete a subarray with XOR k ending here
    - Update the frequency of the current prefix XOR after each check

    Algorithm Steps
    ----------------
    1. Traverse the array, updating a running prefix XOR (xorSum)
    2. If xorSum == k, increment count (subarray from index 0 works)
    3. Compute target = xorSum ^ k; add freq[target] to count if present
    4. Increment freq[xorSum]
    5. Return count

    Time Complexity: O(n) - single pass, O(1) avg for hash map operations
    Space Complexity: O(n) - storing prefix XOR frequencies in hash map
*/
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
    Approach 2: Prefix XOR + Ordered Map (with base case seeded)
    - Same prefix XOR idea as Approach 1, but seeds vmap[0] = 1 upfront to
      represent the "empty prefix" (handles a subarray starting at index 0
      without a special-case branch)
    - For each index, compute target k = xr ^ t, where xr is the prefix XOR
      up to that index and t is the desired subarray XOR
    - Add the frequency of that target prefix directly to the result, then
      record the current prefix XOR
    - Renamed from the original overload (both were named subarraysWithXorK
      with equivalent signatures, which is a duplicate-definition compile
      error) to subarraysWithXorKOrderedMap

    Algorithm Steps
    ----------------
    1. Seed vmap[0] = 1 to account for the empty prefix
    2. Traverse the array, updating running prefix XOR (xr)
    3. Compute k = xr ^ t and add vmap[k] to the running result
    4. Increment vmap[xr]
    5. Return the result

    Time Complexity: O(n log n) - single pass with O(log n) ordered map operations
    Space Complexity: O(n) - storing prefix XOR frequencies in the map
*/
int subarraysWithXorKOrderedMap(vector<int> &arr, int t) {
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

int main() {
    vector<int> arr = {4, 2, 2, 6, 4};
    int k = 6;

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;
    cout << "k: " << k << endl;

    cout << "Subarrays with XOR k (unordered map): " << subarraysWithXorK(arr, k) << endl;
    cout << "Subarrays with XOR k (ordered map):   " << subarraysWithXorKOrderedMap(arr, k) << endl;

    return 0;
}
