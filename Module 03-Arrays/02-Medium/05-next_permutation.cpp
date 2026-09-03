#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Rearrange arr into the next lexicographically greater permutation of numbers, in-place

/*
    Approach: Find Dip, Swap, Reverse Suffix
    - Find the first "dip" from the right where arr[i] < arr[i+1]
    - Swap that element with the smallest element to its right that is greater than it
    - Reverse the suffix after the dip to make it the smallest possible arrangement

    Algorithm Steps
    ----------------
    1. Find the first index 'ind' from the right such that arr[ind] < arr[ind+1]
    2. If no such index exists, the array is the last permutation, so reverse it entirely
    3. Otherwise, from the right, find the first element greater than arr[ind] and swap
    4. Reverse the suffix after 'ind' to get the next smallest arrangement

    Time Complexity: O(n) - a constant number of linear passes over the array
    Space Complexity: O(1) - in-place
*/
void nextPermutation(vector<int>& arr) {
    int n = arr.size(), ind = -1;

    // Step 1: Find the first index 'ind' from the right 
    // such that arr[ind] < arr[ind+1] (the "dip")
    for (int i = n - 2; i >= 0; --i) {
        if (arr[i] < arr[i + 1]) {
            ind = i;
            break;
        }
    }

    // If no such index exists → array is in descending order,
    // so just reverse to get the smallest permutation
    if (ind == -1) {
        reverse(begin(arr), end(arr));
        return;
    }

    // Step 2: From the right, find the first element greater than arr[ind]
    for (int i = n - 1; i > ind; --i) {
        if (arr[i] > arr[ind]) {
            swap(arr[i], arr[ind]);   // swap with next bigger element
            break;                    // important: break after swap
        }
    }

    // Step 3: Reverse the suffix after 'ind' 
    // to get the next lexicographically smallest arrangement
    reverse(arr.begin() + ind + 1, arr.end());
}

int main() {
    vector<int> arr = {1, 2, 3};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    nextPermutation(arr);

    cout << "Next permutation: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    return 0;
}
