#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given two sorted arrays a[] and b[], where each array may contain duplicate elements, 
// the task is to return the elements in the union of the two arrays in sorted order.

vector<int> findUnion(vector<int>& a, vector<int>& b) {
    int i = 0, j = 0;
    int n = a.size(), m = b.size();
    vector<int> res;

    while (i < n && j < m) {
        // Skip duplicates in a
        while (i > 0 && i < n && a[i] == a[i - 1]) i++;
        // Skip duplicates in b
        while (j > 0 && j < m && b[j] == b[j - 1]) j++;
        
        if (i >= n || j >= m) break;

        if (a[i] < b[j]) {
            res.push_back(a[i++]);
        } else if (b[j] < a[i]) {
            res.push_back(b[j++]);
        } else {
            res.push_back(a[i]);
            i++; j++;
        }
    }

    // Add remaining elements from a
    while (i < n) {
        if (i == 0 || a[i] != a[i - 1])
            res.push_back(a[i]);
        i++;
    }

    // Add remaining elements from b
    while (j < m) {
        if (j == 0 || b[j] != b[j - 1])
            res.push_back(b[j]);
        j++;
    }

    return res;
}

// Time Complexity: O(n + m)
// Space Complexity: O(n + m) for the result vector


vector<int> findUnion(vector<int> &a, vector<int> &b) {
    set<int> vset;

    // Step 1: Insert all elements from the first array into the set
    for (int i : a) vset.insert(i);

    // Step 2: Insert all elements from the second array into the set
    for (int i : b) vset.insert(i);

    // Step 3: Convert set to vector (set ensures elements are unique and sorted)
    vector<int> res(begin(vset), end(vset));

    return res;
}

// Time Complexity: O((n + m) * log(n + m))
// - Inserting n + m elements into a set takes log time per insertion

// Space Complexity: O(n + m)
// - For storing unique elements in the set and result vector


