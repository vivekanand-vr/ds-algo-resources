#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Helper function to merge two sorted halves and count inversions
long long mergeAndCount(vector<int>& arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;
    long long invCount = 0;

    // Merge process
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
            // All remaining elements in left[] are greater → inversions
            invCount += (left.size() - i);
        }
    }

    // Copy leftovers
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];

    return invCount;
}

// Recursive function (Divide & Conquer)
long long mergeSortCount(vector<int>& arr, int l, int r) {
    if (l >= r) return 0;
    int m = l + (r - l) / 2;

    long long inv = 0;
    inv += mergeSortCount(arr, l, m);      // Count in left half
    inv += mergeSortCount(arr, m + 1, r);  // Count in right half
    inv += mergeAndCount(arr, l, m, r);    // Count across halves

    return inv;
}

int inversionCount(vector<int> &arr) {
    return (int) mergeSortCount(arr, 0, arr.size() - 1);
}

/*
Logic:
- Use merge sort to divide array into halves.
- While merging, if left[i] > right[j], then all elements from left[i..end] 
  form inversions with right[j].
- Count them efficiently instead of checking all pairs.

Time Complexity:  O(n log n) → merge sort
Space Complexity: O(n)       → temp arrays for merging
*/
