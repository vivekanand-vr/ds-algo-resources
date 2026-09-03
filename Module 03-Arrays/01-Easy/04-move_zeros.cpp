#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an integer array nums, move all 0's to the end of it while maintaining the relative order

/*
    Approach: Two Pointer (Stable Partition)
    - Use a pointer "ind" to track the position where the next non-zero
      element should be placed
    - Scan through the array; whenever a non-zero element is found, place it
      at "ind" and advance ind
    - This preserves the relative order of non-zero elements
    - Finally, fill the rest of the array (from ind to end) with 0s

    Algorithm Steps
    ----------------
    1. Initialize ind = 0
    2. Traverse the array; copy each non-zero element to arr[ind] and increment ind
    3. Fill the remaining positions (ind to n-1) with 0

    Time Complexity: O(n)
    Space Complexity: O(1)
*/
void moveZeroes(vector<int>& arr) {
    int ind = 0;
    int n = arr.size();

    // Step 1: Move all non-zero elements to the front
    for (int i = 0; i < n; ++i) {
        if (arr[i] != 0) {
            arr[ind++] = arr[i]; // Place non-zero element at next available position
        }
    }

    // Step 2: Fill the rest of the array with 0s
    fill(arr.begin() + ind, arr.end(), 0);
}

int main() {
    vector<int> arr = {0, 1, 0, 3, 12};

    cout << "Before: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    moveZeroes(arr);

    cout << "After:  ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    return 0;
}
