#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Q: Given an array with an equal number of positive and negative elements, rearrange it so
// positive and negative numbers alternate, starting with a positive number

/*
    Approach: Two Pointer Placement
    - Place positive numbers at even indices (0, 2, 4, ...)
    - Place negative numbers at odd indices (1, 3, 5, ...)
    - Preserves the relative order of positives and negatives from the original array

    Algorithm Steps
    ----------------
    1. Initialize pos = 0 and neg = 1 as placement pointers
    2. Traverse the array; if element is positive, place it at res[pos] and pos += 2
    3. If element is negative, place it at res[neg] and neg += 2
    4. Return the result array

    Time Complexity: O(n) - single pass through array
    Space Complexity: O(n) - extra result array
*/
vector<int> rearrangeArray(vector<int>& arr) {
    int pos = 0, neg = 1;         // positions to place positives & negatives
    int n = arr.size();
    vector<int> res(n);           // result array of same size

    // Place positive numbers at even indices (0,2,4,..)
    // Place negative numbers at odd indices (1,3,5,..)
    for (int i : arr) {
        if (i > 0) {
            res[pos] = i;
            pos += 2;
        } else {
            res[neg] = i;
            neg += 2;
        }
    }

    return res;
}

int main() {
    vector<int> arr = {3, 1, -2, -5, 2, -4};

    cout << "Array: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    vector<int> res = rearrangeArray(arr);

    cout << "Rearranged array: ";
    for (int x : res) cout << x << " ";
    cout << endl;

    return 0;
}
