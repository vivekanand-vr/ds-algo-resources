#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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

/*
Time Complexity:  O(n) → single pass to find 'ind', 
                   another pass to find swap candidate,
                   and one reverse (O(n)).
Space Complexity: O(1) → in-place.
*/
