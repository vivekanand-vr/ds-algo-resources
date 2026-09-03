#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Check if the given array is sorted and rotated

/*
    Approach: Sorted and Rotated Array Check
    - A sorted-and-rotated array has at most one point where arr[i] > arr[i+1]
    - Find that violation point (if any)
    - The part before and after the point must individually be sorted
    - The last element must be <= the elements before the violation point
      (this is the wrap-around condition of a rotated sorted array)

    Algorithm Steps
    ----------------
    1. Traverse the array to find the first index where arr[i] > arr[i+1]
    2. If no such index exists, the array is already sorted -> true
    3. Check that the remaining part after that point is sorted
    4. Check that the last element is <= every element before the violation point

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
bool check(vector<int>& arr) {
    int point = -1;
    
    // Step 1: Find the first index where the order is violated (i.e., arr[i] > arr[i+1])
    // This marks the "rotation point"
    for (int i = 0; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            point = i;
            break;
        }
    }

    // Step 2: If no such point is found, array is already sorted (not rotated or rotated 0 times)
    if (point == -1) return true;

    // Step 3: Check if the remaining part after the point is sorted
    for (int i = point + 1; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return false; // not sorted
        }
    }

    // Step 4: Check if the last element is not greater than the first part before the point
    // i.e., max in second part ≤ min in first part for rotated sorted array
    for (int i = 0; i <= point; ++i) {
        if (arr[arr.size() - 1] > arr[i]) {
            return false;
        }
    }

    return true;
}

int main() {
    vector<int> arr1 = {3, 4, 5, 1, 2};
    cout << "Array: 3 4 5 1 2 -> " << (check(arr1) ? "Sorted and Rotated" : "Not Sorted and Rotated") << endl;

    vector<int> arr2 = {2, 1, 3, 4};
    cout << "Array: 2 1 3 4 -> " << (check(arr2) ? "Sorted and Rotated" : "Not Sorted and Rotated") << endl;

    vector<int> arr3 = {1, 2, 3, 4, 5};
    cout << "Array: 1 2 3 4 5 -> " << (check(arr3) ? "Sorted and Rotated" : "Not Sorted and Rotated") << endl;

    return 0;
}
