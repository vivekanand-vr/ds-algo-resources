#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Count the number of inversions in an array, i.e. pairs (i, j) such that
// i < j and arr[i] > arr[j]

/*
    Approach: Modified Merge Sort (Divide & Conquer)
    - Count inversions while performing a merge sort, avoiding the O(n^2)
      brute-force pair check
    - Recursively count inversions in the left half and the right half
    - While merging the two sorted halves, whenever an element from the right
      half is placed before a remaining element from the left half, every
      remaining element in the left half forms an inversion with it
    - Sum inversions from the left half, right half, and the merge step

    Algorithm Steps
    ----------------
    1. Recursively split the array into halves until subarrays of size 1
    2. Count inversions in the left half and the right half recursively
    3. Merge the two sorted halves; each time a right-half element is placed
       before a left-half element, add the count of remaining left elements
    4. Sum all three inversion counts and return the total

    Time Complexity: O(n log n) - standard merge sort recurrence
    Space Complexity: O(n) - temporary arrays used during merging
*/

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

int main() {
    vector<int> arr = {2, 4, 1, 3, 5};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    cout << "Inversion count: " << inversionCount(arr) << endl;

    return 0;
}
